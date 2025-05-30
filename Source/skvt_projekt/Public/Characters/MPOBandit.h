// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "MPOBandit.generated.h"

class ADetourCrowdAIController;
class UAIPerceptionComponent;
class AAIController;

UENUM(BlueprintType)
enum class EPatrolPointSelection : uint8
{
	Random,
	Forward,
	Backward
};


UCLASS()
class SKVT_PROJEKT_API AMPOBandit : public ACharacter
{
	GENERATED_BODY()

public:
	AMPOBandit();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category = "MPO Bandit|AI Patroling")
	void MoveToPatrolTarget();
	int32 GetNextPatrolPointIndex() const;
	UFUNCTION(BlueprintCallable, Category = "MPO Bandit|AI Patroling")
	float GetDistanceToPatrolTarget() const;

	UFUNCTION(BlueprintCallable, Category = "MPO Bandit|AI Patroling")
	AActor* GetNextPatrolPoint();
	UFUNCTION(BlueprintCallable, Category = "MPO Bandit|AI Patroling")
	void SelectNextPatrolPoint();
	void DrawPathPoints(FNavPathSharedPtr navPath);

	/***
	 * Patroling
	 * 
	 * This is the target actor that the bandit will patrol to.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "MPO Bandit|AI Patroling", meta = (AllowPrivateAccess = "true"))
	AAIController* BanditAIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MPO Bandit|AI Patroling", meta = (AllowPrivateAccess = "true"))
	ADetourCrowdAIController* BanditStateAIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MPO Bandit|AI Patroling", meta = (AllowPrivateAccess = "true"))
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MPO Bandit|AI Patroling", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MPO Bandit|AI Patroling", meta = (AllowPrivateAccess = "true"))
    TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MPO Bandit|AI Patroling", meta = (AllowPrivateAccess = "true"))
	EPatrolPointSelection PatrolPointSelection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MPO Bandit|AI Sensing", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MPO Bandit|AI Sensing", meta = (AllowPrivateAccess = "true"))
	AActor* LastSensedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MPO Bandit|AI Sensing", meta = (AllowPrivateAccess = "true"))
	bool bPlayerSensed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MPO Bandit|AI Sensing", meta = (AllowPrivateAccess = "true"))
	FVector LastSensedActionLocation;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="MyEvents")
	void OnPlayerSensedChanged(bool bNewPlayerSensed);

	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
