// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenUI.h"
#include "../Player/PlayerCharacter.h"
#include "../Actors/Interactable.h"

UScreenUI::UScreenUI(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {
}

void UScreenUI::NativeConstruct() {
	Super::NativeConstruct();
}

void UScreenUI::InteractionEnable(bool enable, AActor* Interactable) {
	bInteract = enable;
	if (enable)	InteractButton->SetRenderOpacity(1.0f);
	else InteractButton->SetRenderOpacity(0.5f);
}

void UScreenUI::BagTouchEvent() {
	if (InventoryWidget != nullptr) {
		if (!bInventoryOpen) {
			InventoryWidget->AddToViewport();
			bInventoryOpen = true;
		}
		else {
			InventoryWidget->RemoveFromParent();
			bInventoryOpen = false;
		}
	}
}

void UScreenUI::DoInteraction() {
	auto Target = IPlayer->InteractTarget;
	if (bInteract && IsValid(Target)) {
		auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		IPlayer->LookTarget();
		IPlayer->InteractTarget->Interact(Player);
	}
}
