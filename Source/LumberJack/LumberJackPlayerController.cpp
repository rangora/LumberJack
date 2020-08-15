// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LumberJackPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/BasicTree.h"
#include "Actors/Interactable.h"
#include "EngineGlobals.h"
#include "LumberJackCharacter.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "LumberJackCharacter.h"
#include "Engine/World.h"


ALumberJackPlayerController::ALumberJackPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ALumberJackPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	//if (bMoveToMouseCursor)
	//	MoveToMouseCursor();
}

void ALumberJackPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	// mouse
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ALumberJackPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ALumberJackPlayerController::OnSetDestinationReleased);
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ALumberJackPlayerController::Interact);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ALumberJackPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ALumberJackPlayerController::MoveToTouchLocation);
}

void ALumberJackPlayerController::BeginPlay() {
	Super::BeginPlay();
	
	/*auto CurrentMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Inventory = CurrentMode->InventoryWidget;*/
	//SetInputMode(FInputModeGameAndUI());
}

void ALumberJackPlayerController::MoveToMouseCursor() {
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()) 	{
		if (ALumberJackCharacter* MyPawn = Cast<ALumberJackCharacter>(GetPawn()))
			if (MyPawn->CursorToWorld)
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->CursorToWorld->GetComponentLocation());
	}
	else {
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit) {	
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ALumberJackPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location) {
	FVector2D ScreenSpaceLocation(Location);

	FHitResult Hit;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, Hit);
	if (Hit.bBlockingHit) 	{

		// Same with Interact();
		
		auto Target = Cast<AInteractable>(Hit.GetActor());

		if (IsValid(Target)) {
			auto IPlayer = Cast<ALumberJackCharacter>(GetPawn());
			FVector _playerLocation = IPlayer->GetActorLocation();
			FVector _targetLocation = Target->GetActorLocation();

			float _distance = FVector::Distance(_playerLocation, _targetLocation);

			if (_distance < float(_interactDistant)) {
				// Can interact..
				IPlayer->InteractPoint = Hit.Location;
				IPlayer->InteractTarget = Target;
				IPlayer->LookTarget();
				Target->Interact(IPlayer);
			}
			else {
				// Too far to interact..
				SetNewMoveDestination(Hit.ImpactPoint);
			}
		}
		else {
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ALumberJackPlayerController::SetNewMoveDestination(const FVector DestLocation) {
	APawn* const MyPawn = GetPawn();
	if (MyPawn) {
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		if ((Distance > 120.0f)) {
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ALumberJackPlayerController::Interact() {
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit) {
		auto Target = Cast<AInteractable>(Hit.GetActor());
		auto IPlayer =Cast<ALumberJackCharacter>(GetPawn());
		IPlayer->HitVector = Hit.Location;

		if (IsValid(Target)) {
			FVector _playerLocation = IPlayer->GetActorLocation();
			FVector _targetLocation = Target->GetActorLocation();

			float _distance = FVector::Distance(_playerLocation, _targetLocation);

			if (_distance < float(_interactDistant)) {
				// Can interact..
				IPlayer->InteractPoint = Hit.Location;
				IPlayer->InteractTarget = Target;
				//IPlayer->bStopNow = false;
				IPlayer->LookTarget();
				Target->Interact(IPlayer);
			}
			else {
				// Too far to interact..
				SetNewMoveDestination(Hit.ImpactPoint);
			}
		}
		else {
			// We hit something, move there
			if (IPlayer->bInteracting) {
				IPlayer->Anim->StopAnimation();
				IPlayer->bInteracting = false;
			}
			else 
				SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ALumberJackPlayerController::OnSetDestinationPressed() {
	bMoveToMouseCursor = true;
}

void ALumberJackPlayerController::OnSetDestinationReleased() {
	bMoveToMouseCursor = false;
}

