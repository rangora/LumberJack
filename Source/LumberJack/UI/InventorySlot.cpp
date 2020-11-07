// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Engine.h"
#include "Components/Image.h"
#include "../System/MainGameState.h"
#include "../System/MainGameInstance.h"


UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
}

void UInventorySlot::NativeConstruct() {
	Super::NativeConstruct();
	Thumbnail->SetBrushFromTexture(ThumbnailTexture);
}

void UInventorySlot::SetThisItem(FName ID, int32 _Count) {
	auto IInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	auto ItemTable = IInstance->BaseItemDB;
	auto ItemToAdd = ItemTable->FindRow<FBaseItemTable>(ID, "");
	
	ItemID = ItemToAdd->ItemID;
	ItemName = ItemToAdd->ItemName;
	ThumbnailTexture = ItemToAdd->Thumbnail;
	bEmpty = false;
	Count = _Count;
}

void UInventorySlot::SyncItemDB(int32 _Idx) {
	auto IInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	auto ItemTable = IInstance->BaseItemDB;
	auto PlayerItemData = IInstance->PlayerItems;

	// From PlayerItemData
	ItemID = PlayerItemData[_Idx].ItemID;
	Count = PlayerItemData[_Idx].Count;


	// From BaseDB
	auto BaseItemData = ItemTable->FindRow<FBaseItemTable>(ItemID, "");
	ItemName = BaseItemData->ItemName;
	ThumbnailTexture = BaseItemData->Thumbnail;
	bEmpty = false;
}
