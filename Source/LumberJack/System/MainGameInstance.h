// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/GameInstance.h"
#include "../DataTable/ABasicItemTable.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */



class PlayerItemData {
public:
	PlayerItemData(FName _ItemID, int32 _Count = 0)
		: ItemID(_ItemID), Count(_Count) {}

	// Common Info..
	FName ItemID;
	FName ItemName;
	UTexture2D* ThumbnailTexture;
	int32 Count;

};


UCLASS()
class LUMBERJACK_API UMainGameInstance : public UGameInstance {
	GENERATED_BODY()
	
public:
	UMainGameInstance();
	virtual void Init() override;

	// return --> True(New item), False(Already exist).. 
	int AddItem(FName _ItemID, int32 _Count = 1);

	// Item Data Table..
	UPROPERTY(EditDefaultsOnly)
		UDataTable* BaseItemDB;

	// Player Item Data Array..
	TArray<PlayerItemData> PlayerItems;
};
