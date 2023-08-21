// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterMovementComponent.h"
#include "../../Characters/Animations/DDBaseCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "../../actors/Interactive/Environment/Ladder.h"
#include "../../Characters/DDBaseCharacter.h"

void UBaseCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (IsOnLadder())
	{
		return;
	}
	Super::PhysicsRotation(DeltaTime);
}

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		Result = SprintSpeed;
	}
	else if (IsOnLadder())
	{
		Result = PullUpOnLadderMaxSpeed;
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

void UBaseCharacterMovementComponent::StartPullUp(const FPullUpMovementParameters& PullUpParameters)
{
	CurrentPullUpParameters = PullUpParameters;
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

float UBaseCharacterMovementComponent::GetLadderSpeedRatio() const
{
	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	return FVector::DotProduct(LadderUpVector, Velocity) / PullUpOnLadderMaxSpeed;

}

void UBaseCharacterMovementComponent::AttachToLadder(const ALadder* Ladder)
{
	CurrentLadder = Ladder;

	FRotator TargetOrientationRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
	TargetOrientationRotation.Yaw += 180.0f;

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderForwardVector = CurrentLadder->GetActorForwardVector();
	float Projection = GetCharacterToCurrentLadderProjection(GetActorLocation());

	FVector NewCharacterLocation = CurrentLadder->GetActorLocation() + Projection * LadderUpVector + LadderToCharacterOffset * LadderForwardVector;
	if (CurrentLadder->GetIsOnTop())
	{
		NewCharacterLocation = CurrentLadder->GetAttachFromTopAnimMontageStartingLocation();
	}

	GetOwner()->SetActorLocation(NewCharacterLocation);
	GetOwner()->SetActorRotation(TargetOrientationRotation);

	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Ladder);
}

void UBaseCharacterMovementComponent::DetachFromLadder()
{
	SetMovementMode(MOVE_Falling);
}

bool UBaseCharacterMovementComponent::IsOnLadder() const
{
	return UpdatedComponent && EMovementMode::MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Ladder;
}

const ALadder* UBaseCharacterMovementComponent::GetCurrentLadder()
{
	return CurrentLadder;
}

void UBaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case (uint8)ECustomMovementMode::CMOVE_PullUp:
	{
		PhysPullUp(DeltaTime, Iterations);
		break;
	}
	case (uint8)ECustomMovementMode::CMOVE_Ladder:
	{
		PhysLadder(DeltaTime, Iterations);
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

	if (PreviousMovementMove == MOVE_Custom && PreviousMovementMove == (uint8)ECustomMovementMode::CMOVE_Ladder)
	{
		CurrentLadder = nullptr;
	}

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
			case (uint8)ECustomMovementMode::CMOVE_PullUp:
			{
				GetWorld()->GetTimerManager().SetTimer(PullUpTimer, this, &UBaseCharacterMovementComponent::EndPullUp, CurrentPullUpParameters.Duration, false);
			}

			default:
				break;
		}
	}
}

ADDBaseCharacter* UBaseCharacterMovementComponent::GetBaseCharacterOwner() const
{
	return StaticCast<ADDBaseCharacter*>(CharacterOwner);
}
	
void UBaseCharacterMovementComponent::PhysPullUp(float DeltaTime, int32 Iterations)
{
	float ElapseTime = GetWorld()->GetTimerManager().GetTimerElapsed(PullUpTimer) + CurrentPullUpParameters.StartTime; //возвращает кол-во секунд с момента запуска

	FVector PullUpCurveValue = CurrentPullUpParameters.PullUpCurve->GetVectorValue(ElapseTime);

	float PositsionAlpha = PullUpCurveValue.X;
	float XYCorrectionAlpha = PullUpCurveValue.Y;
	float ZCorrectionAlpha = PullUpCurveValue.Z;

	FVector CorrectedInitialLocation = FMath::Lerp(CurrentPullUpParameters.InitialLocation, CurrentPullUpParameters.InitialAnimationLocation, XYCorrectionAlpha);
	CorrectedInitialLocation.Z = FMath::Lerp(CurrentPullUpParameters.InitialLocation.Z, CurrentPullUpParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);

	FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentPullUpParameters.TargetLocation, PositsionAlpha);
	FRotator NewRotation = FMath::Lerp(CurrentPullUpParameters.InitialRotation, CurrentPullUpParameters.TargetRotation, PositsionAlpha);


	FVector Delta = NewLocation - GetActorLocation();


	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, NewRotation, false, Hit);
}

void UBaseCharacterMovementComponent::PhysLadder(float DeltaTime, int32 Iterations)
{
	CalcVelocity(DeltaTime, 1.0f, false, PullUpOnLadderBreakingDecelaretion);

	FVector Delta = Velocity * DeltaTime;

	if (HasAnimRootMotion())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), false, Hit);
		return;
	}


	FVector NewPositsion = GetActorLocation() + Delta;
	float NewPositsionProjection = GetCharacterToCurrentLadderProjection(NewPositsion);

	if (NewPositsionProjection < MinLadderBottomOffset)
	{
		DetachFromLadder();
		return;
	}
	else if (NewPositsionProjection > (CurrentLadder->GetLadderHeight() - MaxLadderHightTopOffset))
	{
		GetBaseCharacterOwner()->PullUp(true);
		return;
	}

	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, Hit);
}

float UBaseCharacterMovementComponent::GetCharacterToCurrentLadderProjection(const FVector& Location) const
{
	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderToCharacterDistance = Location - CurrentLadder->GetActorLocation();
	return FVector::DotProduct(LadderUpVector, LadderToCharacterDistance);
}
