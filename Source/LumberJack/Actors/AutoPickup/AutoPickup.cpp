// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoPickup.h"
#include "LumberJack/LumberJackCharacter.h"

AAutoPickup::AAutoPickup() {
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetCollisionProfileName(TEXT("PickupActor"));
	RootComponent = Cast<USceneComponent>(PickupMesh);

	Rotation = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation"));
	
	ItemID = FName("None");
}

void AAutoPickup::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AAutoPickup::Collect(ACharacter* Player) {
	float _speed = 0.03;
	auto IPlayer = Cast<ALumberJackCharacter>(Player);
	
	FVector _playerLocation = IPlayer->GetActorLocation();
	FVector _direction		= _playerLocation - GetActorLocation();
	FVector _newLocation	= GetActorLocation() + (_direction * _speed);

	bIsCollected = true;
	PickupMesh->SetEnableGravity(false);
	_playerLocation.Z += 55.f;
	SetActorLocation(_newLocation);
}
