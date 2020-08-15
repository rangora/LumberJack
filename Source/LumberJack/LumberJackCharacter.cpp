// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LumberJackCharacter.h"
#include "System/MainGameMode.h"
#include "LumberJackPlayerController.h"
#include "Actors/BasicTree.h"
#include "Actors/AutoPickup/AutoPickup.h"
#include "Kismet/KismetMathLibrary.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Engine.h"


ALumberJackCharacter::ALumberJackCharacter() {
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Interaction.
	chopDamage = 100.f;
	InteractTarget = nullptr;

	// Collection.
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(300.f);

	// Coliision.
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ALumberJackCharacter::PickupItem);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ALumberJackCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr) {
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) {
			if (UWorld* World = GetWorld()) {
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = CameraComponent->GetComponentLocation();
				FVector EndLocation = CameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController())) {
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	if (bNeedRotation) {
		FRotator Ro{this->GetActorRotation().Pitch, TargetRotation.Yaw, this->GetActorRotation().Roll};
		auto NewRotation = FMath::RInterpTo(this->GetActorRotation(), TargetRotation, DeltaSeconds, 0.2f);
		SetActorRotation(Ro);
		bNeedRotation = false;
	}

	CollectAutoPickup();
}

void ALumberJackCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim != nullptr) {
		Anim->OnChopHit.BindUFunction(this, FName("ChopTheTree"));
		//Anim->OnChopHit.BindUObject(this, &ALumberJackCharacter::ChopTheTree);
	}
}

void ALumberJackCharacter::BeginPlay() {
	Super::BeginPlay();

	auto CurrentMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Inventory = Cast<UInventoryUI>(CurrentMode->InventoryWidget);
}

void ALumberJackCharacter::LookTarget() {
	if (IsValid(InteractTarget) && InteractTarget->bInteractable) {
		bNeedRotation = true;
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), InteractTarget->GetActorLocation());
	}
}

void ALumberJackCharacter::CollectAutoPickup() {
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);
	for (auto _actor : CollectedActors) {
		AAutoPickup* const _pickup = Cast<AAutoPickup>(_actor);
		if (_pickup && !_pickup->IsPendingKill())
			_pickup->Collect(this);
	}
}

void ALumberJackCharacter::ChopTheTree() {
	auto Tree = Cast<ABasicTree>(InteractTarget);

	if (IsValid(Tree)) {
		Tree->health -= 100.f;

		if (Tree->health <= 0.f) {
			Tree->CuttingDown();
			Anim->StopAnimation();
			bInteracting = false;
		}
	}
}

void ALumberJackCharacter::PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AAutoPickup* Item = Cast<AAutoPickup>(OtherActor);

	if (IsValid(Item)) {
		if(Inventory->AddItem(Item->ItemID))
			Item->Destroy();
	}
}
