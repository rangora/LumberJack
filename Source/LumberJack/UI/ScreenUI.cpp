// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenUI.h"


UScreenUI::UScreenUI(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {
}


void UScreenUI::NativeConstruct() {
	Super::NativeConstruct();
}

void UScreenUI::BagTouchEvent() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BagTouchEvent")));

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
