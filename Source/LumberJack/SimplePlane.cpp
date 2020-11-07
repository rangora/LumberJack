// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplePlane.h"
#include "RHI.h"


ASimplePlane::ASimplePlane()
	: m_wholeTextureReagion(0, 0, 0, 0, m_textureSize, m_textureSize) {
	
	m_coverSize = 5000;
	PrimaryActorTick.bCanEverTick = false;

	m_squarePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SquarePlaneMesh"));
	RootComponent = m_squarePlane;
	m_squarePlane->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(
		TEXT("/Engine/ArtTools/RenderToTexture/Meshes/S_1_Unit_Plane.S_1_Unit_Plane"));

	if (PlaneMesh.Succeeded()) {
		m_squarePlane->SetStaticMesh(PlaneMesh.Object);
	}

	
	
	// 불투명할 때, 우선순위를 조정한다.
	// 맨 위에 그려져야 하니 높게 설정 한다.
	m_squarePlane->TranslucencySortPriority = 100;
	m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));

	// Materials.
	static ConstructorHelpers::FObjectFinder<UMaterial> M_Fow(
		TEXT("/Game/Fow/FowMat.FowMat"));

	if (M_Fow.Succeeded()) {
		m_dynamicMaterial = M_Fow.Object;
	}

	if (!m_dynamicTexture) {
		// 텍스쳐를 새롭게 생성한다. (CreateDefaultSubobject()같은거..)
		m_dynamicTexture = UTexture2D::CreateTransient(m_textureSize, m_textureSize, PF_G8);
		m_dynamicTexture->CompressionSettings = TextureCompressionSettings::TC_Grayscale;
		m_dynamicTexture->SRGB = 0;
		m_dynamicTexture->UpdateResource();
		m_dynamicTexture->MipGenSettings = TMGS_NoMipmaps;
	}

	// Initialise array to all black.
	for (int i = 0; i < m_textureSize; i++)
		for (int j = 0; j < m_textureSize; j++)
			m_pixelArray[j * m_textureSize + i] = 255;

	if (m_dynamicTexture) {
		UpdateTextureRegions(m_dynamicTexture, 0, 1, &m_wholeTextureReagion, m_textureSize, 1, m_pixelArray, false);
	}

}

void ASimplePlane::PostInitializeComponents() {
	Super::PostInitializeComponents();

	if (m_dynamicMaterial) {
		m_dynamicMaterialInstance = UMaterialInstanceDynamic::Create(m_dynamicMaterial, this);
		m_dynamicMaterialInstance->SetTextureParameterValue("FowTexture", m_dynamicTexture);
	}

	if (m_dynamicMaterialInstance)
		m_squarePlane->SetMaterial(0, m_dynamicMaterialInstance);
}

void ASimplePlane::SetSize(float val) {
	m_coverSize = val;
	m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));
}

void ASimplePlane::RevealSmoothCircle(const FVector2D& pos, float radius) {
	// Calcuate the where circle center is inside texture space.
	FVector2D texel = pos - FVector2D(GetActorLocation().X, GetActorLocation().Y);
	texel = texel * m_textureSize / m_coverSize;
	texel += FVector2D(m_textureSize / 2, m_textureSize / 2);

	// Calculate radius in texel unit (1 is 1 pixel).
	float texelRadius = radius * m_textureSize / m_coverSize;

	DrawDebugSphere(GetWorld(), FVector(texel.X, texel.Y, 400.f), 5.f, 16, FColor::Black);
	DrawDebugSphere(GetWorld(), FVector(texel.X+texelRadius, texel.Y, 400.f), 5.f, 16, FColor::Black);

	UE_LOG(LogTemp, Warning, TEXT("texel:%s"),*texel.ToString());

	// The square area to update.
	int minX = FMath::Clamp<int>(texel.X - texelRadius, 0, m_textureSize - 1);
	int minY = FMath::Clamp<int>(texel.Y - texelRadius, 0, m_textureSize - 1);
	int maxX = FMath::Clamp<int>(texel.X + texelRadius, 0, m_textureSize - 1);
	int maxY = FMath::Clamp<int>(texel.Y + texelRadius, 0, m_textureSize - 1);

	// Debug
	DrawDebugSphere(GetWorld(), FVector(minX, minY, 400.f),5.f, 16, FColor::Blue);
	DrawDebugSphere(GetWorld(), FVector(maxX, maxY, 400.f),5.f, 16, FColor::Blue);

	uint8 theVal = 0;

	// Update our array of fow value in memory.
	bool dirty = false;
	for (int i = 0; i < m_textureSize; i++) {
		for (int j = 0; j < m_textureSize; j++) {
			
			m_pixelArray[j * m_textureSize + i] = 255;

			if (i >= minX && i < maxX &&
				j >= minY && j < maxY) {
				float distance = FVector2D::Distance(texel, FVector2D(i, j));
				uint8 oldVal = m_pixelArray[j * m_textureSize + i];
				static float smoothPct = 0.7f;
				float lerp = FMath::GetMappedRangeValueClamped(FVector2D(smoothPct, 1.0f), FVector2D(0, 1), distance / texelRadius);

				//m_dynamicMaterial->GetMaterial().tran

				if (distance < texelRadius) {
					uint8 newVal = lerp * 255;
					newVal = FMath::Min(newVal, oldVal);
					m_pixelArray[j * m_textureSize + i] = newVal;
					dirty = dirty || oldVal != newVal;
				}
			}
		}
	}

	if (dirty) {
		UpdateTextureRegions(m_dynamicTexture, 0, 1,
			&m_wholeTextureReagion, m_textureSize, 1, m_pixelArray, false);
	}
}

void ASimplePlane::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData) {
	if (Texture->Resource) {
		struct FUpdateTextureRegionsData {
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;


		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData, {
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex) {
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip) {
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
							);
					}
				}
		if (bFreeData) {
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}

}
