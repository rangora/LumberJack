// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "InventorySlot.h"
#include <UMG/Public/Blueprint/WidgetTree.h>
#include <UMG/Public/Components/UniformGridPanel.h>
#include <UMG/Public/Components/GridPanel.h>
#include "InventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class LUMBERJACK_API UInventoryUI : public UBaseUI {
	GENERATED_BODY()
	
public:
	UInventoryUI(const FObjectInitializer& ObjectInitializer);
	void NativeConstruct() override;

	void InitItemData();
	
	bool AddItem(FName ItemID, int32 Count = 1);
	void RefreshSlots();
	void RefreshPanel();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UGridPanel* Panel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	 UInventorySlot* BaseSlot;

	TArray<UInventorySlot*> ItemSlots;

private:
	enum {
		column = 5,
		maxSize = 50
	};
};
