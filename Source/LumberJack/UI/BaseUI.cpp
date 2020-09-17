// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUI.h"
#include "LumberJack/Player/PlayerCharacter.h"
#include "LumberJack/LumberJackPlayerController.h"

UBaseUI::UBaseUI(const FObjectInitializer& ObjectIntializer) 
	: Super(ObjectIntializer) {
	IPlayer     = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	IController = Cast<ALumberJackPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}