// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Player/PlayerAnimInstance.h"
#include "Actors/Interactable.h"
#include "UI/InventoryUI.h"
#include "GameFramework/Character.h"
#include "LumberJackCharacter.generated.h"

UCLASS(Blueprintable)
class ALumberJackCharacter : public ACharacter {
	GENERATED_BODY()

public:
	ALumberJackCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	void LookTarget();
	void CollectAutoPickup();

	UFUNCTION() // Delegate.
		void ChopTheTree();

public:
	float chopDamage;
	bool bInteracting = false;

	FVector HitVector;
	FVector InteractPoint;
	AInteractable* InteractTarget;
	UInventoryUI* Inventory;
	
	// For mouse intput.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UDecalComponent* CursorToWorld;
	UPROPERTY()
		UPlayerAnimInstance* Anim;
	

private:
	UFUNCTION()
		void PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bNeedRotation = false;

	FRotator TargetRotation;
	USphereComponent* CollectionSphere;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;
	

	
};

