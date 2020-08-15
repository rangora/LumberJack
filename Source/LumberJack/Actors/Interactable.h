// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class LUMBERJACK_API AInteractable : public AActor {
	GENERATED_BODY()
	
public:	
	AInteractable();
	virtual void Interact(ACharacter* ACharacter);
	 
	bool bInteractable;
	FString Name;
	FString Action;
};
