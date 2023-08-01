// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawnAnimInstance.h"
#include "../GameCodeBasePawn.h"
#include "../../Components/MovementComponents/GCBaseMovementComponent.h"

void UBasePawnAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	CachedBasePawn = StaticCast<AGameCodeBasePawn*>(TryGetPawnOwner());
}

void UBasePawnAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBasePawn.IsValid())
	{
		return;
	}

	InputForward = CachedBasePawn->GetInputForward();
	InputRight = CachedBasePawn->GetInputRight();
	bIsInAir = CachedBasePawn->GetMovementComponent()->IsFalling();
}
