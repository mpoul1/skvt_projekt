#include "Characters/MPOAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMPOAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MPOCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (MPOCharacter)
		MPOCharacterMovementComponent = MPOCharacter->GetCharacterMovement();
}

void UMPOAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!MPOCharacter || !MPOCharacterMovementComponent)
		return;

	GroundSpeed = UKismetMathLibrary::VSizeXY(MPOCharacterMovementComponent->Velocity);
	VerticalSpeed = MPOCharacterMovementComponent->Velocity.Z;
	bIsCrouching = MPOCharacterMovementComponent->IsCrouching();
	bIsInAir = MPOCharacterMovementComponent->IsFalling();
}

