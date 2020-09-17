// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"


AInteractable::AInteractable() {
	PrimaryActorTick.bCanEverTick = true;
	bInteractable = true;
	Name = "Interactable";
	Action = "interact";
}

void AInteractable::Interact(ACharacter* ACharacter) {}
