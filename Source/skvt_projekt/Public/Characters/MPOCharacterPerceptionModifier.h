// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MPOCharacterPerceptionModifier.generated.h"


class UCharacterMovementComponent;
class UAIPerceptionStimuliSourceComponent;
class AActor;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnPerceptionChanged, UMPOCharacterPerceptionModifier, OnPerceptionChanged, bool, bIsPerceptionActive);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKVT_PROJEKT_API UMPOCharacterPerceptionModifier : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMPOCharacterPerceptionModifier();

	UFUNCTION(BlueprintCallable, Category = "MPO|AI Sensing")
	void DisablePerception();
	
	UFUNCTION(BlueprintCallable, Category = "MPO|AI Sensing")
	void EnablePerception();

	UPROPERTY(BlueprintAssignable, Category = "MPO|AI Sensing")
	FOnPerceptionChanged OnPerceptionChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MPO|AI Sensing")
	bool bIsPerceptionActive = true;
	bool bEvaluatingPerception = false;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MPO|AI Sensing", meta = (AllowPrivateAccess = "true"))
	ACharacter* Owner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MPO|AI Sensing", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* CharacterMovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MPO|AI Sensing", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliComponent;

	void ChangePerceptionTo(bool newIsPerceptionActive);

protected:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void EvaluatePerception();

		
};
