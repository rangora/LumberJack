// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "AutoPickup.generated.h"


UCLASS()
class LUMBERJACK_API AAutoPickup : public AActor {
	GENERATED_BODY()
	
public:	
	AAutoPickup();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void Collect(ACharacter* Player);

	void SetItemProperty(int code);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PickupMesh;
	UPROPERTY(VisibleAnywhere)
		URotatingMovementComponent* Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	bool bIsCollected = false;
};
