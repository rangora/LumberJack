// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DataTable/ABasicItemTable.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"

/**
 * 
 */
UCLASS()
class LUMBERJACK_API AMainGameState : public AGameStateBase {
	GENERATED_BODY()
	
public:
	AMainGameState();

	UPROPERTY(EditDefaultsOnly)
		UDataTable* BaseItemDB;
};
