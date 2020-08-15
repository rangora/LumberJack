// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "MainGameState.h"
#include "../LumberJackPlayerController.h"
#include "../LumberJackCharacter.h"
#include "UObject/ConstructorHelpers.h"


AMainGameMode::AMainGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Player.BP_Player_C"));

	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;

	PlayerControllerClass = ALumberJackPlayerController::StaticClass();
	GameStateClass = AMainGameState::StaticClass();

	// UI.
	static ConstructorHelpers::FClassFinder<UScreenUI> MainScreenWidgetClass_C(
		TEXT("/Game/UI/BP_ScreenUI.BP_ScreenUI_C"));
	static ConstructorHelpers::FClassFinder<UInventoryUI> InventoryWidgetClass_C(
		TEXT("/Game/UI/BP_InventoryUI.BP_InventoryUI_C"));
	static ConstructorHelpers::FClassFinder<UInventorySlot> SlotWidgetClass_C(
		TEXT("/Game/UI/BP_InventorySlot.BP_InventorySlot_C"));

	if (MainScreenWidgetClass_C.Succeeded())
		MainScreenWidgetClass = MainScreenWidgetClass_C.Class;	
	if (InventoryWidgetClass_C.Succeeded())
		InventoryWidgetClass = InventoryWidgetClass_C.Class;
	if (SlotWidgetClass_C.Succeeded())
		SlotWidgetClass = SlotWidgetClass_C.Class;
}

void AMainGameMode::BeginPlay() {
	Super::BeginPlay();

	MainScreenWidget = CreateWidget<UScreenUI>(GetWorld(), MainScreenWidgetClass);
	InventoryWidget = CreateWidget<UInventoryUI>(GetWorld(), InventoryWidgetClass);
	SlotWidget = CreateWidget<UInventorySlot>(GetWorld(), SlotWidgetClass);

	if (MainScreenWidget != nullptr)
		MainScreenWidget->AddToViewport();
	else {
		UE_LOG(LogTemp, Warning, TEXT("NoMainScrrenWIdget"));
	}
	if (InventoryWidget != nullptr) {
		MainScreenWidget->InventoryWidget = InventoryWidget;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NoInventoryWIdget"));
	}
	if (SlotWidget != nullptr) {
		MainScreenWidget->InventoryWidget->BaseSlot = SlotWidget;
	}
}

void AMainGameMode::ChangeScreenWidget(TSubclassOf<UUserWidget> NewWidgetClass) {
	if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
			CurrentWidget->AddToViewport();
	}
}
