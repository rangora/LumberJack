// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "../Plugins/Runtime/ApexDestruction/Source/ApexDestruction/Public/DestructibleComponent.h"
#include "../Plugins/Runtime/ApexDestruction/Source/ApexDestruction/Public/DestructibleMesh.h"
#include "GameFramework/Actor.h"
#include "BasicTree.generated.h"

UCLASS()
class LUMBERJACK_API ABasicTree : public AInteractable {
	GENERATED_BODY()
	
public:	
	ABasicTree();

	virtual void Tick(float DeltaTime) override;
	virtual void Interact(ACharacter* ACharacter);

	void CuttingDown();

	float health = 500.f;

private:
	bool bDestructed = false;

	class UStaticMeshComponent* Stump;
	class UDestructibleComponent* Destructible;
	class UStaticMeshComponent* Body;

	class UParticleSystemComponent* ParticleComponent;
};
