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
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void Interact(ACharacter* ACharacter);

	void CuttingDown();
	void DestroyBody();

	float health = 100.f;
	bool bDestructed = false;

	class UStaticMeshComponent* Stump;
	class UStaticMeshComponent* Body;

	class UParticleSystemComponent* ParticleComponent;

	FTimerHandle DestroyTimer;
};

