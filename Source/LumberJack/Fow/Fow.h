// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RHI.h"
#include "GameFramework/Actor.h"
#include "Fow.generated.h"

UCLASS()
class LUMBERJACK_API AFow : public AActor {
	GENERATED_BODY()
	
public:	
	AFow();
	virtual void PostInitializeComponents() override;
	void SetSize(float val);
	void RevealSmoothCircle(const FVector2D& pos, float radius);

private:
	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, 
		FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);
	
	// Fow texture size.
	static const int m_textureSize = 512;

	UPROPERTY()
		UStaticMeshComponent* m_squarePlane;
	UPROPERTY()
		UTexture2D* m_dynamicTexture;
	UPROPERTY()
		UMaterialInterface* m_dynamicMaterial;
	UPROPERTY()
		UMaterialInstanceDynamic* m_dynamicMaterialInstance;

	uint8 m_pixelArray[m_textureSize * m_textureSize];
	FUpdateTextureRegion2D m_wholeTextureReagion;
	float m_coverSize;
};
