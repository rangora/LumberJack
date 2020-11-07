// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "BasicTree.generated.h"

UCLASS()
class LUMBERJACK_API ABasicTree : public AInteractable {
	GENERATED_BODY()
	
public:	
	ABasicTree();
	virtual void BeginPlay() override;
	virtual void Interact(ACharacter* ACharacter);

	void CuttingDown();
	void DestroyBody();
	void DropItem();

	bool bDestructed = false;
	bool bDoRotation = false;

	// Variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float health = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float force = 1000000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float offsetZ = 65.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int itemCode = -1;
	
	// Meshes.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Leaf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Stump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Body;

	class UParticleSystemComponent* ParticleComponent;

	FTimerHandle DestroyTimer;
};

