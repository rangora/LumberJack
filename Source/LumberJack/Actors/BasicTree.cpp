// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTree.h"
#include "Engine.h"
#include "../LumberJackCharacter.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


ABasicTree::ABasicTree() {
	Name = "BasicTree";
	Action = "Cut";

	ConstructorHelpers::FObjectFinder<UStaticMesh> StumpMesh(TEXT("/Game/Resources/Tree/Stump/tree_Stump.tree_Stump"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(TEXT("/Game/Resources/Tree/Body/tree_Log.tree_Log"));
	ConstructorHelpers::FObjectFinder<UDestructibleMesh> DestructibleMesh(TEXT("/Game/Resources/Tree/Destructible/tree_Destructible_DM.tree_Destructible_DM"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleMesh(TEXT("/Game/A_External/Particles_Wind_Control_System/Particles/P_Fireflies.P_Fireflies"));

	Stump = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StumpMesh"));
	Destructible = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DesturctibleMesh"));
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
	if (DestructibleMesh.Succeeded()) {
		Destructible->SetDestructibleMesh(DestructibleMesh.Object);
		UE_LOG(LogTemp, Warning, TEXT("Destructible Mesh loaded."));
	}
	
	Destructible->SetupAttachment(Stump);
	Body->SetupAttachment(Stump);
	Body->SetCollisionProfileName("BlockAll");
	

	Stump->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	//Stump->SetRelativeScale3D(FVector(10.3f, 10.3f, 10.3f));
	//Destructible->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	//Body->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
}

void ABasicTree::Tick(float DeltaTime) {
}

void ABasicTree::Interact(ACharacter* ACharacter) {
	auto IPlayer = Cast<ALumberJackCharacter>(ACharacter);

	if (!bDestructed) {
		IPlayer->bInteracting = true;
		IPlayer->Anim->PlayChopAnimation();
	}
}

void ABasicTree::CuttingDown() {
	bDestructed = true;
	bInteractable = false;
	auto IPlayer = Cast<ALumberJackCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	Destructible->ApplyRadiusDamage(600.f, IPlayer->InteractPoint, 300.f, 500.f, true);
	Body->SetSimulatePhysics(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CUTTED")));

	//ParticleComponent->Activate(true);
}
