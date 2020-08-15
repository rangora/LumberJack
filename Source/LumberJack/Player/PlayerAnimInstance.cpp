// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "Engine.h"

UPlayerAnimInstance::UPlayerAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ChopMontage(
		TEXT("/Game/Player/Animation/Montage_Chop.Montage_Chop"));
	if (ChopMontage.Succeeded()) 
		ChopAnim = ChopMontage.Object;
	
	speed = 0.f;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto IPlayer = TryGetPawnOwner();
	if (IsValid(IPlayer)) {
		speed = IPlayer->GetVelocity().Size();
	}
}

void UPlayerAnimInstance::PlayChopAnimation() {
	Montage_Play(ChopAnim, 1.0f);
}

void UPlayerAnimInstance::StopAnimation() {
	StopAllMontages(0.5f);
}

void UPlayerAnimInstance::AnimNotify_ChopHit() {
	if (OnChopHit.IsBound())
		OnChopHit.Execute();
}
