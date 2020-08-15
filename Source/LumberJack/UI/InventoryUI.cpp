// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"
#include "../System/MainGameState.h"
#include "Engine.h"

UInventoryUI::UInventoryUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	
}

void UInventoryUI::NativeConstruct() {
	Super::NativeConstruct();

	Panel = this->WidgetTree->FindWidget<UGridPanel>("InventoryPanel");
	RefreshPanel();
}

bool UInventoryUI::AddItem(FName ItemID) {
	auto NewSlot = NewObject<UInventorySlot>(BaseSlot, BaseSlot->GetClass());
	NewSlot->SetThisItem(ItemID);
	ItemArray.Add(NewSlot);
	
	return true;
}

void UInventoryUI::AddNewItem() {
}

void UInventoryUI::RefreshPanel() {
	for (int i = 0; i < ItemArray.Num(); i++) {
		Panel->AddChildToGrid(ItemArray[i], i / column, i % column);
	}
}
