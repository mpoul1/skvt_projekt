// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MPOBandit.h"

#include "AIController.h"
#include "AITypes.h"
#include "DetourCrowdAIController.h"
#include "Characters/MPOCharacterPerceptionModifier.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"


AMPOBandit::AMPOBandit()
{
	PrimaryActorTick.bCanEverTick = true;
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void AMPOBandit::BeginPlay()
{
	Super::BeginPlay();

	BanditAIController = Cast<AAIController>(GetController());
	BanditStateAIController = Cast<ADetourCrowdAIController>(GetController());
	BanditAIMovementComponent = GetComponentByClass<UCharacterMovementComponent>();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMPOBandit::OnTargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AMPOBandit::OnTargetPerceptionForgotten);
}

// Function to check line of sight
bool AMPOBandit::IsPlayerVisible(AActor* PlayerActor)
{
    if (!PlayerActor) return false;

	auto perceptionModifier = PlayerActor->GetComponentByClass<UMPOCharacterPerceptionModifier>();
	if (!perceptionModifier) return true;

	return perceptionModifier->bIsPerceptionActive;
}


void AMPOBandit::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (LastSensedActor!=nullptr && Actor == LastSensedActor && !Stimulus.WasSuccessfullySensed())
	{
		Stimulus.SetExpirationAge(5.f); // Set a reasonable expiration age for the stimulus;
		OnTargetPerceptionForgotten(Actor);
		return;
	}
	
	if (!Actor || Actor == this || !Stimulus.WasSuccessfullySensed())
		return;

	// If the sensed actor is Player we should handle it
	// Check if the sensed actor is a player character
	if (!Actor->IsA(APawn::StaticClass()))
		return;;
	auto Pawn = Cast<APawn>(Actor);
	if (!Pawn || !Pawn->IsPlayerControlled())
		return;
		
		
	if (!IsPlayerVisible(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player not visible: %s, propably hidden by some Glass"), *Actor->GetName());
		return;
	}

	LastSensedActor = Actor;
	LastSensedActionLocation = Actor->GetActorLocation();
	
	bPlayerSensed = true;
	UE_LOG(LogTemp, Warning, TEXT("Perceived Player: %s"), *Actor->GetName());
	OnPlayerSensedChanged(bPlayerSensed);
}

void AMPOBandit::OnTargetPerceptionForgotten(AActor* Actor)
{
	if (LastSensedActor != Actor)
		return;

	LastSensedActionLocation = Actor->GetActorLocation();
	LastSensedActor = nullptr;
	bPlayerSensed = false;
	UE_LOG(LogTemp, Warning, TEXT("Lost Player"));
	OnPlayerSensedChanged(bPlayerSensed);
}


/**
 * Moves the bandit to the current PatrolTarget using the AI controller's MoveTo function.
 * If there is no PatrolTarget or the AI controller is not set, it does nothing.
 */
void AMPOBandit::MoveToPatrolTarget()
{
	if (!PatrolTarget || !BanditAIController)
		return;

	if (GetDistanceToPatrolTarget()<AcceptanceRadius)
		return;

	FAIMoveRequest moveReq;
	moveReq.SetGoalActor(PatrolTarget);
	moveReq.SetAcceptanceRadius(AcceptanceRadius); // Set a reasonable acceptance radius
	moveReq.SetUsePathfinding(true);

	FNavPathSharedPtr NavPath;
	BanditAIController->MoveTo(moveReq, &NavPath);
	DrawPathPoints(NavPath);
}

/**
 * Returns the index of the next patrol point based on the current PatrolTarget and the PatrolPointSelection mode.
 * If there are no patrol points, it returns INDEX_NONE.
 * 
 * @return The index of the next patrol point.
 */
int32 AMPOBandit::GetNextPatrolPointIndex() const
{
	if (PatrolPoints.Num() == 0)
		return INDEX_NONE;
	
	auto currentIndex = PatrolPoints.IndexOfByKey(PatrolTarget);
	
	switch (PatrolPointSelection)
	{
		case EPatrolPointSelection::Random:
			return FMath::RandRange(0, PatrolPoints.Num() - 1);
		
		case EPatrolPointSelection::Forward:
			if (currentIndex == INDEX_NONE)
				return 0;
		
			return (currentIndex + 1) % PatrolPoints.Num(); // Wrap around to the start
		
		case EPatrolPointSelection::Backward:
			if (currentIndex == INDEX_NONE)
				return PatrolPoints.Num()-1;
			
			return (currentIndex - 1 + PatrolPoints.Num()) % PatrolPoints.Num(); // Wrap around to the end
		
	}

	return INDEX_NONE; // Default case, should not happen
}

float AMPOBandit::GetDistanceToPatrolTarget() const
{
	if (!PatrolTarget)
		return 0.0f;

	auto ClampAcceptanceRadius = FMath::Max(FVector::Dist(GetActorLocation(), PatrolTarget->GetActorLocation()) - AcceptanceRadius, 0.0f); // Ensure AcceptanceRadius is not zero or negative
	return ClampAcceptanceRadius;
}

/**
 * Returns the next patrol point based on the current PatrolTarget and the PatrolPointSelection mode.
 * If there are no patrol points, it returns nullptr.
 * 
 * @return The next patrol point actor or nullptr if no points are available.
 */
AActor* AMPOBandit::GetNextPatrolPoint()
{
	auto NextPatrolPointIndex = GetNextPatrolPointIndex();
	
	if (NextPatrolPointIndex == INDEX_NONE)
		return nullptr;

	return PatrolPoints[NextPatrolPointIndex];
}

/**
 * Selects the next patrol point in the PatrolPoints array and sets it as the PatrolTarget.
 * If the current PatrolTarget is not found in the array, it defaults to the first point.
 * If there are no patrol points, it does nothing.
 */
void AMPOBandit::SelectNextPatrolPoint()
{
	auto NextPatrolPoint = GetNextPatrolPoint();
	if (!NextPatrolPoint)
		return;
	
	PatrolTarget = GetNextPatrolPoint();
}

/**
 * Draws debug spheres at each point in the navigation path.
 * This is useful for visualizing the path the bandit will take.
 * 
 * @param navPath The navigation path to draw points for.
 */
void AMPOBandit::DrawPathPoints(FNavPathSharedPtr navPath)
{
	for (auto& Point :navPath->GetPathPoints())
	{
		DrawDebugSphere(GetWorld(), Point.Location, 10.0f, 12, FColor::Green, false, 10.f);
	}
}

void AMPOBandit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMPOBandit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

