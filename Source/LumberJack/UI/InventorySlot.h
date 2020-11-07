// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/BaseUI.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class LUMBERJACK_API UInventorySlot : public UBaseUI {
	GENERATED_BODY()
	
public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;

	void SetThisItem(FName ID, int32 _Count = 1);
	void SyncItemDB(int32 _Idx);

	// Item data..
	UPROPERTY(VisibleAnywhere)
		FName ItemID;
	UPROPERTY(VisibleAnywhere)
		FName ItemName;
	UPROPERTY(VisibleAnywhere)
		UTexture2D* ThumbnailTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bEmpty = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ItemCount;
	

};
