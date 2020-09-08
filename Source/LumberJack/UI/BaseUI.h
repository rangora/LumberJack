// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUI.generated.h"


class APlayerCharacter;
class ALumberJackPlayerController;

UCLASS()
class LUMBERJACK_API UBaseUI : public UUserWidget {
	GENERATED_BODY()
	
public:
	UBaseUI(const FObjectInitializer& ObjectIntializer);

	APlayerCharacter*  IPlayer;
	ALumberJackPlayerController* IController;
};
