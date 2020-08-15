// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine.h"
#include "InventorySlot.h"
#include "Components/Image.h"
#include "../System/MainGameState.h"


UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
}

void UInventorySlot::NativeConstruct() {
	Super::NativeConstruct();
	Thumbnail->SetBrushFromTexture(ThumbnailTexture);
}

void UInventorySlot::SetThisItem(FName ID) {
	auto currentState = Cast<AMainGameState>(GetWorld()->GetGameState());
	UDataTable* ItemTable = currentState->BaseItemDB;
	FBaseItemTable* ItemToAdd = ItemTable->FindRow<FBaseItemTable>(ID, "");
	
	ItemID = ItemToAdd->ItemID;
	ItemName = ItemToAdd->ItemName;
	ThumbnailTexture = ItemToAdd->Thumbnail;
	bEmpty = false;
	count = 1;
}
