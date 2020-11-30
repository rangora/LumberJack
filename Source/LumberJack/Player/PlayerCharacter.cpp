// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Engine.h"
#include "../System/MainGameInstance.h"
#include "../System/MainGameMode.h"
#include "../System/MainGameState.h"
#include "../System/NetClient.h"
#include "../LumberJackPlayerController.h"
#include "../Actors/BasicTree.h"
#include "../Actors/AutoPickup/AutoPickup.h"
#include "Kismet/KismetMathLibrary.h"
#include "HeadMountedDisplayFunctionLibrary.h"


APlayerCharacter::APlayerCharacter() {
	// Interaction.
	chopDamage = 100.f;
	InteractTarget = nullptr;

	// Collection.
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(300.f);
	InteractionSphere->SetSphereRadius(interactionRadius);

	// Coliision.
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::PickupItem);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Weapon Socket. (TEST)
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ST_Weapon(
		TEXT("/Game/Resources/Item/ST_SampleAxe.ST_SampleAxe"));
	if (ST_Weapon.Succeeded())
		Weapon->SetStaticMesh(ST_Weapon.Object);
	//
}

void APlayerCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (bNeedRotation) {
		FRotator Ro{ this->GetActorRotation().Pitch, TargetRotation.Yaw, this->GetActorRotation().Roll };
		auto NewRotation = FMath::RInterpTo(this->GetActorRotation(), Ro, DeltaSeconds, 0.4f);
		SetActorRotation(Ro);
		bNeedRotation = false;
	}

	CollectAutoPickup();
	CollectInteractable();

	//GetActorForwardVector();
	float radius = 600.f;
	for (int i = 1; i <= 24; i++) {
		float _y = FMath::Sin(float(i) * 15.f) * radius;
		float _x = FMath::Cos(float(i) * 15.f) * radius;

		FVector Ray = { GetActorLocation().X + _x,
						GetActorLocation().Y + _y,
						GetActorLocation().Z};
		//DrawDebugLine(GetWorld(), GetActorLocation(), Ray, FColor::Red);

	}
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorForwardVector(), FColor::Red);
}

void APlayerCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim != nullptr) {
		Anim->OnChopHit.BindUFunction(this, FName("ChopTheTree"));
		//Anim->OnChopHit.BindUObject(this, &APlayerCharacter::ChopTheTree);
	}
}

void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	auto CurrentMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Inventory = Cast<UInventoryUI>(CurrentMode->InventoryWidget);

	FName Socket = TEXT("LH_Socket");
	if (GetMesh()->DoesSocketExist(Socket)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DoesSocketExist")));
		
		// (TEST)
		Weapon->SetRelativeScale3D(FVector(4.f, 4.f, 4.f));
		Weapon->SetCollisionProfileName(TEXT("OverlapAll"));
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
		//
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NO_DoesSocketExist")));
	}

	// [TEMP] for multi users..
	Client = NewObject<UNetClient>();
	Client->connect("127.0.0.1", 999);
	Client->login(p_uid);
	FPlatformProcess::Sleep(1.f);
	FString m_data = Client->retrieveMessage();
	
	/*
	auto IState = Cast<AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));
	IState->Client = NewObject<UNetClient>();
	IState->uid = p_uid;
	IState->Client->connect("127.0.0.1", 999);
	IState->Client->login(p_uid);
	FPlatformProcess::Sleep(1.f);

	FString m_data = IState->Client->retrieveMessage();
	*/


	if (!m_data.IsEmpty()) {
		if (m_data[0] - '0' == LoginOperation::ACCEPT) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(
				TEXT("Login Accepted!!")));
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(
		TEXT("m_data: %s"), *m_data));
	/*********/

	ItemInit();
}

void APlayerCharacter::ItemInit() {
	auto IInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto IState = Cast<AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (Client->syncPlayerItem()) {
		FPlatformProcess::Sleep(1.f);
		FString m_data = Client->retrieveMessage();

		if (!m_data.IsEmpty()) {
			int num = FCString::Atoi(*m_data.Mid(0, 3));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			//	FString::Printf(TEXT("num: %d"), num));

			for (int i = 0; i < num; i++) {
				int pivot = 3 + (8 * i);
				int j = pivot;
				int k = pivot + 4;
				while (m_data[j] == '0') j++;
				while (m_data[k] == '0') k++;

				auto itemcode = m_data.Mid(j, (pivot+4) - j);
				auto itemcount = m_data.Mid(k, (pivot+8) - k); //k=8

				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, 
				//	FString::Printf(TEXT("itemcode: %s"), *itemcode));
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				//	FString::Printf(TEXT("itemcount: %s"), *itemcount));
				
				FName _itemCode = FName(*itemcode);
				int32_t _itemCount = FCString::Atoi(*itemcount);
				Inventory->AddItem(_itemCode, _itemCount);
			}
		}
	}


}

void APlayerCharacter::LookTarget() {
	if (IsValid(InteractTarget) && InteractTarget->bInteractable) {
		bNeedRotation = true;
		TargetRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), InteractTarget->GetActorLocation());
	}
}

void APlayerCharacter::CollectAutoPickup() {
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);
	for (auto _actor : CollectedActors) {
		AAutoPickup* const _pickup = Cast<AAutoPickup>(_actor);
		if (_pickup && !_pickup->IsPendingKill())
			_pickup->Collect(this);
	}
}

void APlayerCharacter::CollectInteractable() {
	TArray<AActor*> CollectedActors;
	bool isInteract = false;
	auto IGameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());

	InteractionSphere->GetOverlappingActors(CollectedActors);
	for (auto actor : CollectedActors) {
		AInteractable* inActor = Cast<AInteractable>(actor);

		if (inActor && !inActor->IsPendingKill() && inActor->bInteractable) {
			isInteract = true;
			IGameMode->MainScreenWidget->InteractionEnable(true, inActor);
			InteractTarget = inActor;
			break;
		}
	}

	if (!isInteract) {
		IGameMode->MainScreenWidget->InteractionEnable(false, nullptr);
		InteractTarget = nullptr;
	}
}

void APlayerCharacter::ChopTheTree() {
	auto Tree = Cast<ABasicTree>(InteractTarget);

	if (IsValid(Tree)) {
		Tree->health -= 100.f;

		if (Tree->health <= 0.f) {
			Tree->Body->SetSimulatePhysics(true);
			Tree->Leaf->SetSimulatePhysics(true);
			auto _socket = Weapon->GetSocketByName(TEXT("AxeHit"));
			auto _AxeVector = GetMesh()->GetSocketLocation(TEXT("LH_Socket")) + _socket->RelativeLocation;
			auto _treeVector = FVector(Tree->GetActorLocation().X , Tree->GetActorLocation().Y, _AxeVector.Z);

			// Debug
			//DrawDebugLine(GetWorld(), _AxeVector, _treeVector, FColor::Red, true, 5.f, 0);

			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);


			bool bResult = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				_AxeVector,
				_treeVector,
				ECollisionChannel::ECC_Visibility);
			
			FVector _endVector;
			if (bResult) {
				if (HitResult.Actor.IsValid()) {
					FVector _hitVector = HitResult.Location;
					_hitVector.Z += Tree->offsetZ;

					FVector _treePush = FVector(_treeVector.X, _treeVector.Y, _hitVector.Z);
					_endVector = (_treePush - _hitVector) * Tree->force + _hitVector;
					Tree->Body->AddForceAtLocation(_endVector, _hitVector);
					
					//// Debug.					
					//DrawDebugLine(GetWorld(), _hitVector, _endVector, FColor::Red, true);
					//DrawDebugSphere(GetWorld(),
					//	_hitVector, 8.f, 12, FColor::Green, true);
					//DrawDebugSphere(GetWorld(),
					//	_endVector, 8.f, 12, FColor::White, true);
				}
			
			}
	/*		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(
				TEXT("_endVector:%s"), *_endVector.ToString()));*/
			
			Tree->CuttingDown();
			Anim->StopAnimation();
			bInteracting = false;
		}
	}
}

void APlayerCharacter::PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AAutoPickup* Item = Cast<AAutoPickup>(OtherActor);
	auto IIstate = Cast<AMainGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (IsValid(Item)) {
		Client->passItemInfo(Item->ItemID);
		if (Inventory->AddItem(Item->ItemID))
			Item->Destroy();
	}
}
