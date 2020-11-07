// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTree.h"
#include "Engine.h"
#include "AutoPickup/AutoPickup.h"
#include "../Player/PlayerCharacter.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


ABasicTree::ABasicTree() {
	PrimaryActorTick.bCanEverTick = false;
	Name = "BasicTree";
	Action = "Cut";

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleMesh(
		TEXT("/Game/A_External/Particles_Wind_Control_System/Particles/P_Fireflies.P_Fireflies"));

	Leaf = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeafMesh"));
	Stump = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StumpMesh"));
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleMesh"));

	RootComponent = Stump;
	Body->SetupAttachment(RootComponent);
	Leaf->SetupAttachment(Body);

	if (ParticleMesh.Succeeded()) {
		ParticleComponent->SetTemplate(ParticleMesh.Object);
		ParticleComponent->bAutoActivate = false;
		ParticleComponent->SetupAttachment(Body);
		ParticleComponent->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	}
	Leaf->SetCollisionProfileName("NoCollision");
	Body->SetCollisionProfileName("BlockAll");
	Stump->SetCollisionProfileName("BlockAll");


	SetActorTickEnabled(false);
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
	DropItem();
	ParticleComponent->DestroyComponent();
	Body->DestroyComponent();
	Leaf->DestroyComponent();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tree::Destroy")));
}

void ABasicTree::DropItem() {
	FString BP_PickupPath = "/Game/Blueprints/Pickup/SamplePickup.SamplePickup_C";
	UClass* BP_Pickup = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_PickupPath));

	auto Socket = Body->GetSocketByName(TEXT("DropSocket"));
	if (IsValid(Socket)) {
		auto Item = GetWorld()->SpawnActor<AAutoPickup>(BP_Pickup, Body->GetSocketLocation(TEXT("DropSocket")), FRotator::ZeroRotator);
		Item->SetItemProperty(1);
	}
}
