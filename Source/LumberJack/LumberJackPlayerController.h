// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LumberJackPlayerController.generated.h"

UCLASS()
class ALumberJackPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	ALumberJackPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
};


