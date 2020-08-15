// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "BaseUI.h"
#include "InventoryUI.h"
#include <UMG/Public/Blueprint/WidgetTree.h>
#include <UMG/Public/Components/Button.h>
#include <UMG/Public/Components/UniformGridPanel.h>
#include "ScreenUI.generated.h"

/**
 * 
 */
UCLASS()
class LUMBERJACK_API UScreenUI : public UBaseUI {
	GENERATED_BODY()

public:
	UScreenUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	

	UFUNCTION(BlueprintCallable)
		void BagTouchEvent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UButton* BagButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		UInventoryUI* InventoryWidget;

	bool bInventoryOpen = false;
};
