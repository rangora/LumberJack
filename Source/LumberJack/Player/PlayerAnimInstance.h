// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

DECLARE_DELEGATE(FChopHitDelegate);

class ALumberJackCharacter;

UCLASS()
class LUMBERJACK_API UPlayerAnimInstance : public UAnimInstance {
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayChopAnimation();
	void StopAnimation();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* ChopAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float speed;

	FChopHitDelegate OnChopHit;
	
private:
	UFUNCTION()
		void AnimNotify_ChopHit();


};
