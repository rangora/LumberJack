// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"
#include "../System/MainGameState.h"
#include "../System/MainGameInstance.h"
#include "Engine.h"

UInventoryUI::UInventoryUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	
}

void UInventoryUI::NativeConstruct() {
	Super::NativeConstruct();

	Panel = this->WidgetTree->FindWidget<UGridPanel>("InventoryPanel");
	RefreshPanel();
}

void UInventoryUI::InitItemData() {
	auto IInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	for(int i=0; i<IInstance->PlayerItems.Num(); i++) {
		auto NewSlot = NewObject<UInventorySlot>(BaseSlot, BaseSlot->GetClass());
		NewSlot->SyncItemDB(i);
		ItemSlots.Add(NewSlot);
	}
}

bool UInventoryUI::AddItem(FName ItemID, int32 Count) {
	auto IInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	int idx = IInstance->AddItem(ItemID);

	if (idx < 0) {
		// Get in when new item..
		auto NewSlot = NewObject<UInventorySlot>(BaseSlot, BaseSlot->GetClass());
		NewSlot->SetThisItem(ItemID, Count);
		ItemSlots.Add(NewSlot);
	}
	else {
		if (ItemSlots[idx]->ItemID.Compare(ItemID) == 0) {
			ItemSlots[idx]->Count += Count;
		}
	}
	return true;
}

void UInventoryUI::RefreshSlots() {
}

void UInventoryUI::RefreshPanel() {
	for (int i = 0; i < ItemSlots.Num(); i++) {
		Panel->AddChildToGrid(ItemSlots[i], i / column, i % column);
	}
}
