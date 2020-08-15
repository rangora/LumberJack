// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../UI/ScreenUI.h"
#include "../UI/InventoryUI.h"
#include "../UI/InventorySlot.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LUMBERJACK_API AMainGameMode : public AGameModeBase {
	GENERATED_BODY()
	
public:
	AMainGameMode();
	virtual void BeginPlay() override;

	void ChangeScreenWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<class UScreenUI> MainScreenWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<class UInventoryUI> InventoryWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<class UInventorySlot> SlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		UScreenUI* MainScreenWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		UInventoryUI* InventoryWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		UInventorySlot* SlotWidget;

	UUserWidget* CurrentWidget;
};
