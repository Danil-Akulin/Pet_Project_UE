// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "../../Characters/Animations/DDBaseCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		Result = SprintSpeed;
	}
	return Result;
}

void UBaseCharacterMovementComponent::StartSprint()
{
	if (Stamina != 0)
	{
		GetWorld()->GetTimerManager().SetTimer(StaminaUpdateTimerHandle, this, &UBaseCharacterMovementComponent::UpdateStamina, StaminaUpdateInterval, true);
		UE_LOG(LogTemp, Warning, TEXT("Stamin is %f"), Stamina);
		bIsSprinting = true;
	}
}

void UBaseCharacterMovementComponent::StopSprint()
{
	bIsSprinting = false; 
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Delay, this, &UBaseCharacterMovementComponent::DelayedFunction, DelaySeconds, true);
}

void UBaseCharacterMovementComponent::UpdateStamina()
{
	if (Stamina <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stamin is over."));

		bIsSprinting = false;
		return;
	}
	if (bIsSprinting)
	{
		Stamina--;
	}
	else
	{
		return;
	}
}

void UBaseCharacterMovementComponent::DelayedFunction()
{
	if (!bIsSprinting)
	{
		if (Stamina >= 100)
		{
			return;
		}
		Stamina += 6;
		UE_LOG(LogTemp, Warning, TEXT("Stamin is %f"), Stamina);
	}
	return;
}

void UBaseCharacterMovementComponent::StartPullUp(const FLedgeDescription& LedgeDescription)
{
	TargetLedge = LedgeDescription;
	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_PullUp);
}

void UBaseCharacterMovementComponent::EndPullUp()
{
	SetMovementMode(MOVE_Walking);
}

bool UBaseCharacterMovementComponent::IsPullUp() const
{
	return UpdatedComponent && EMovementMode::MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_PullUp;
}

void UBaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case (uint8)ECustomMovementMode::CMOVE_PullUp:
	{
		float ProgressRatio = GetWorld()->GetTimerManager().GetTimerElapsed(PullUpTimer) / TargetPullUpTime;
		FVector NewLocation = FMath::Lerp(InitialPullUpPositsion, TargetLedge.Location, ProgressRatio);
		FRotator NewRotation = FMath::Lerp(InitialPullUpRotation, TargetLedge.Rotation, ProgressRatio);

		FVector Delta = NewLocation - GetActorLocation();


		FHitResult Hit;
		SafeMoveUpdatedComponent(Delta, NewRotation, false, Hit);

		break;
	}
	default:
		break;
	}


	Super::PhysCustom(DeltaTime, Iterations);
}

void UBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMove, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMove, PreviousCustomMode);

	if (MovementMode == MOVE_Swimming)
	{
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingCapsuleRadius, SwimmingCapsuleHalfHeight);
	}
	else if (PreviousMovementMove == MOVE_Swimming)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);
	}

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
			case (uint8)ECustomMovementMode::CMOVE_PullUp:
			{
				InitialPullUpPositsion = GetActorLocation();
				InitialPullUpRotation = GetOwner()->GetActorRotation();
				TargetPullUpTime = 0.25f;
				GetWorld()->GetTimerManager().SetTimer(PullUpTimer, this, &UBaseCharacterMovementComponent::EndPullUp, TargetPullUpTime, false);
			}

			default:
				break;
		}
	}
}
	