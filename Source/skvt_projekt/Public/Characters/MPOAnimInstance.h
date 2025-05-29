// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MPOAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SKVT_PROJEKT_API UMPOAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class ACharacter* MPOCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UCharacterMovementComponent* MPOCharacterMovementComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float VerticalSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsCrouching;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;
	
	
};
