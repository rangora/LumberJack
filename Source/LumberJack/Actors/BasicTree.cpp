// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTree.h"
#include "Engine.h"
#include "Wood.h"
#include "../Player/PlayerCharacter.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


ABasicTree::ABasicTree() {
	Name = "BasicTree";
	Action = "Cut";

	ConstructorHelpers::FObjectFinder<UStaticMesh> StumpMesh(
		TEXT("/Game/Maps/ST_Stump.ST_Stump"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(
		TEXT("/Game/Maps/ST_Body.ST_Body"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleMesh(
		TEXT("/Game/A_External/Particles_Wind_Control_System/Particles/P_Fireflies.P_Fireflies"));

	Stump = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StumpMesh"));
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleMesh"));

	RootComponent = Stump;
	Stump->SetStaticMesh(StumpMesh.Object);
	Body->SetStaticMesh(BodyMesh.Object);

	if (ParticleMesh.Succeeded()) {
		ParticleComponent->SetTemplate(ParticleMesh.Object);
		ParticleComponent->bAutoActivate = false;
		ParticleComponent->SetupAttachment(Body);
		ParticleComponent->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	}
	
	Body->SetCollisionProfileName("BlockAll");
	Stump->SetCollisionProfileName("BlockAll");

	Body->SetupAttachment(Stump);
	Body->SetRelativeLocation(FVector(0.f, -2.f, 195.f));
}

void ABasicTree::Tick(float DeltaTime) {
}

void ABasicTree::BeginPlay() {
	Super::BeginPlay();
}

void ABasicTree::Interact(ACharacter* ACharacter) {
	auto IPlayer = Cast<APlayerCharacter>(ACharacter);

	if (!bDestructed) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Interact")));

		IPlayer->bInteracting = true;
		IPlayer->Anim->PlayChopAnimation();
	}
}

void ABasicTree::CuttingDown() {
	bDestructed = true;
	bInteractable = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CUTTED")));
	ParticleComponent->Activate(true);
	
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ABasicTree::DestroyBody, 6.f, false);
}

void ABasicTree::DestroyBody() {
	ParticleComponent->DestroyComponent();
	Body->DestroyComponent();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tree::Destroy")));
}
