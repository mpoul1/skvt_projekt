// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "MPOPerceptionModifier.generated.h"

UINTERFACE(MinimalAPI)
class UMPOPerceptionModifier : public UInterface
{
	GENERATED_BODY()
};

class IMPOPerceptionModifier
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MPO Bandit|AI Sensing")
	void EnablePerception();
	virtual void EnablePerception_Implementation() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MPO Bandit|AI Sensing")
	void DisablePerception();
	virtual void DisablePerception_Implementation() = 0;
};
