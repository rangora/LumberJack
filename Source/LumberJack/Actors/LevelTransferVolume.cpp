// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransferVolume.h"

// Sets default values
ALevelTransferVolume::ALevelTransferVolume() {
	PrimaryActorTick.bCanEverTick = false;
	
	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferVolume"));
	RootComponent = TransferVolume;
	TransferVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void ALevelTransferVolume::NotifyActorBeginOverlap(AActor* OtherActor) {
	APawn* IPawn = Cast<APawn>(OtherActor);
	if (IPawn != nullptr) {
		UGameplayStatics::OpenLevel(this, TransferLevelName);
	}
}
