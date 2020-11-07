// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LumberJackPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/BasicTree.h"
#include "Actors/Interactable.h"
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Engine/World.h"


ALumberJackPlayerController::ALumberJackPlayerController() {
}

void ALumberJackPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	APlayerCharacter* IPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FVector2D Location2D{ IPlayer->GetActorLocation().X, IPlayer->GetActorLocation().Y };
	//PlayerFow->RevealSmoothCircle(Location2D, 1000);
	PlayerPlane->SetActorLocation(FVector(0.f, 0.f, IPlayer->GetActorLocation().Z));
	PlayerPlane->RevealSmoothCircle(Location2D, 600);

	//Location2D.ToString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(
	//	TEXT("2D:%s"), *Location2D.ToString()));


}

void ALumberJackPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	// Keyboard & Touch interface.
	//InputComponent->BindAxis("MoveForward", this, &ALumberJackPlayerController::MoveForward);
	//InputComponent->BindAxis("MoveRight", this, &ALumberJackPlayerController::MoveRight);

	// mouse
	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &ALumberJackPlayerController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &ALumberJackPlayerController::OnSetDestinationReleased);
	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &ALumberJackPlayerController::Interact);

	//// support touch devices 
	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ALumberJackPlayerController::MoveToTouchLocation);
	//InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ALumberJackPlayerController::MoveToTouchLocation);
}

void ALumberJackPlayerController::BeginPlay() {
	Super::BeginPlay();
	
	//PlayerFow = GetWorld()->SpawnActor<AFow>(AFow::StaticClass());
	PlayerPlane = GetWorld()->SpawnActor<ASimplePlane>(ASimplePlane::StaticClass());
	
	FString TouchInterfacePath = "/Game/UI/MainTouchInterface.MainTouchInterface";
	UTouchInterface* MainTouchInterface = Cast<UTouchInterface>(StaticLoadObject(UTouchInterface::StaticClass(), NULL, *TouchInterfacePath));
	if (IsValid(MainTouchInterface)) 
		ActivateTouchInterface(MainTouchInterface);
	/*auto CurrentMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Inventory = CurrentMode->InventoryWidget;*/
	//SetInputMode(FInputModeGameAndUI());
}