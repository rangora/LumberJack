// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "LevelTransferVolume.generated.h"

UCLASS()
class LUMBERJACK_API ALevelTransferVolume : public AActor {
	GENERATED_BODY()
	
public:	
	ALevelTransferVolume();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
		FName TransferLevelName;
	UBoxComponent* TransferVolume;
};
