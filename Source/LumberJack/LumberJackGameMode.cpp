// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LumberJackGameMode.h"
#include "LumberJackPlayerController.h"
#include "LumberJackCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALumberJackGameMode::ALumberJackGameMode() {
	PlayerControllerClass = ALumberJackPlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Player.BP_Player_C"));

	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;
}