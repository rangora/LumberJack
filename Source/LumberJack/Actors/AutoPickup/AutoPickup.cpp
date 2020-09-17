// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoPickup.h"
#include "../../Player/PlayerCharacter.h"

AAutoPickup::AAutoPickup() {
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetCollisionProfileName(TEXT("PickupActor"));
	PickupMesh->SetSimulatePhysics(true);
	RootComponent = Cast<USceneComponent>(PickupMesh);

	Rotation = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation"));
	ItemID = FName("None");
}

void AAutoPickup::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AAutoPickup::BeginPlay() {
	Super::BeginPlay();
	PickupMesh->SetEnableGravity(false);
}

void AAutoPickup::Collect(ACharacter* Player) {
	float _speed = 0.03;
	auto IPlayer = Cast<APlayerCharacter>(Player);
	
	FVector _playerLocation = IPlayer->GetActorLocation();
	FVector _direction		= _playerLocation - GetActorLocation();
	FVector _newLocation	= GetActorLocation() + (_direction * _speed);

	bIsCollected = true;
	_playerLocation.Z += 55.f;
	SetActorLocation(_newLocation);
}

void AAutoPickup::SetItemProperty(int code) {
}
