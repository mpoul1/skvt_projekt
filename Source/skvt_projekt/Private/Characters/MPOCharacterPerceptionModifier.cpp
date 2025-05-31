// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MPOCharacterPerceptionModifier.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

class UAIPerceptionStimuliSourceComponent;
// Sets default values for this component's properties
UMPOCharacterPerceptionModifier::UMPOCharacterPerceptionModifier()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 3.0f;
}

// Called when the game starts
void UMPOCharacterPerceptionModifier::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = Cast<ACharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UMPOCharacterPerceptionModifier::BeginPlay - Owner must be AActor!"));
		return;
	}
	CharacterMovementComponent = Owner->GetComponentByClass<UCharacterMovementComponent>();
	PerceptionStimuliComponent = Owner->GetComponentByClass<UAIPerceptionStimuliSourceComponent>();}

void UMPOCharacterPerceptionModifier::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	EvaluatePerception();
}

void UMPOCharacterPerceptionModifier::DisablePerception()
{
	UE_LOG(LogTemp, Warning, TEXT("[UMPOCharacterPerceptionModifier] Perception Disabled"));
	bEvaluatingPerception = true;
}
void UMPOCharacterPerceptionModifier::EnablePerception()
{
	UE_LOG(LogTemp, Warning, TEXT("[UMPOCharacterPerceptionModifier] Perception Enabled"));
	bEvaluatingPerception = false;
	ChangePerceptionTo(true);
	UE_LOG(LogTemp, Warning, TEXT("[UMPOCharacterPerceptionModifier] Perception Changed to: %s"),
		   (bIsPerceptionActive ? TEXT("Enabled") : TEXT("Disabled")));
}

void UMPOCharacterPerceptionModifier::ChangePerceptionTo(bool newIsPerceptionActive)
{
	bIsPerceptionActive = newIsPerceptionActive;
	if (PerceptionStimuliComponent)
	{
		if (bIsPerceptionActive)
			PerceptionStimuliComponent->RegisterWithPerceptionSystem();
		else
			PerceptionStimuliComponent->UnregisterFromPerceptionSystem();
	}
	UE_LOG(LogTemp, Warning, TEXT("Perception Changed to: %s"),
	       (bIsPerceptionActive ? TEXT("Enabled") : TEXT("Disabled")));
	
	OnPerceptionChanged.Broadcast(bIsPerceptionActive);
}

void UMPOCharacterPerceptionModifier::EvaluatePerception()
{
	if (!bEvaluatingPerception) return;
	if (!Owner || !CharacterMovementComponent)
		return;
	
	auto newIsPerceptionActive = !CharacterMovementComponent->IsCrouching();
	if (newIsPerceptionActive == bIsPerceptionActive)
		return;

	ChangePerceptionTo(newIsPerceptionActive);
}


