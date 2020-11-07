// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "MainGameState.h"

UMainGameInstance::UMainGameInstance() {
	static ConstructorHelpers::FObjectFinder<UDataTable> BaseItemDB_Class(
		TEXT("/Game/DataTable/BaseItemTable.BaseItemTable"));

	if (BaseItemDB_Class.Succeeded())
		BaseItemDB = BaseItemDB_Class.Object;
}

void UMainGameInstance::Init() {
	Super::Init();
}

int UMainGameInstance::AddItem(FName _ItemID, int32 _Count) {
	auto AddtoItem = BaseItemDB->FindRow<FBaseItemTable>(_ItemID, "");
	
	// Already exist..
	for (int i = 0; i < PlayerItems.Num(); i++) {
		if (PlayerItems[i].ItemID.Compare(_ItemID) == 0) {
			PlayerItems[i].Count += _Count;
			return i;
		}
	}
	
	// New item..
	auto NewItem = new PlayerItemData{ _ItemID, _Count };
	PlayerItems.Add(*NewItem);
	return -1;
}


