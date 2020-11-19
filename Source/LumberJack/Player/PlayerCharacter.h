// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerAnimInstance.h"
#include "../Actors/Interactable.h"
#include "../UI/InventoryUI.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LUMBERJACK_API APlayerCharacter : public ABaseCharacter {
	GENERATED_BODY()


public:
	APlayerCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	void ItemInit();

	void LookTarget();
	void CollectAutoPickup();
	void CollectInteractable();

	UFUNCTION() // Delegate.
		void ChopTheTree();

public:
	float chopDamage;
	float interactionRadius = 100.f;
	bool bInteracting = false;

	FVector HitVector;
	AInteractable* InteractTarget;
	UInventoryUI* Inventory;

	UPROPERTY()
		UPlayerAnimInstance* Anim;
	UPROPERTY(Editanywhere, BlueprintReadwrite)
		int p_uid;


private:
	UFUNCTION()
		void PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bNeedRotation = false;

	FRotator TargetRotation;
	USphereComponent* CollectionSphere;
	USphereComponent* InteractionSphere;
};
