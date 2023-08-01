// Fill out your copyright notice in the Description page of Project Settings.


#include "DDBaseCharacterAnimInstance.h"
#include "../DDBaseCharacter.h"
#include "../../Components/MovementComponents/BaseCharacterMovementComponent.h"
#include <GameFramework/CharacterMovementComponent.h>

void UDDBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<ADDBaseCharacter>(), TEXT("ERROR - ALL BAD"));
	CachedBaseCharacter = StaticCast<ADDBaseCharacter*>(TryGetPawnOwner());
}

void UDDBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UBaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();
	Speed = CharacterMovement->Velocity.Size();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsSprinting = CharacterMovement->IsSprinting();
}
 