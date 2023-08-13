// Fill out your copyright notice in the Description page of Project Settings.


#include "DDBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/MovementComponents/BaseCharacterMovementComponent.h"
#include "../LedgeDetectorComponents.h"

ADDBaseCharacter::ADDBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	BaseCharacterMovementComponent = StaticCast<UBaseCharacterMovementComponent*>(GetCharacterMovement());

	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponents>(TEXT("LedgeDetector"));
}

void ADDBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ADDBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void ADDBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void ADDBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryChangeSprintState();
}

bool ADDBaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() && !GetBaseCharacterMovementComponent()->IsPullUp();
}

void ADDBaseCharacter::PullUp()
{
	FLedgeDescription LedgeDiscriptin;
	if (LedgeDetectorComponent->DetectLedge(LedgeDiscriptin))
	{
		FPullUpMovementParameters PullUpParameters;
		PullUpParameters.PullUpCurve = HighPullUpSettings.PullUpCurve;
		PullUpParameters.InitialLocation = GetActorLocation();
		PullUpParameters.InitialRotation = GetActorRotation();
		PullUpParameters.TargetLocation = LedgeDiscriptin.Location;
		PullUpParameters.TargetRotation = LedgeDiscriptin.Rotation;

		float MinRange;
		float MaxRange;	

		HighPullUpSettings.PullUpCurve->GetTimeRange(MinRange, MaxRange);

		PullUpParameters.Duration = MaxRange - MinRange;

		float PullUpHeight = (PullUpParameters.TargetLocation - PullUpParameters.InitialLocation).Z;

		FVector2D SourceRange(HighPullUpSettings.MinHeight, HighPullUpSettings.MaxHeight);
		FVector2D TargetRange(HighPullUpSettings.MinHeightStartTime, HighPullUpSettings.MaxHeightStartTime);
		PullUpParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, PullUpHeight);

		GetBaseCharacterMovementComponent()->StartPullUp(PullUpParameters);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(HighPullUpSettings.PullUpMontage, 1.0f, EMontagePlayReturnType::Duration, PullUpParameters.StartTime);
	}
}


bool ADDBaseCharacter::CanSprint()
{
	return true;
}

void ADDBaseCharacter::TryChangeSprintState()
{																			// если есть запрос на спринт но мы не бежим но мы можем бежать
	if (bIsSprintRequested && !BaseCharacterMovementComponent->IsSprinting() && CanSprint())
	{
		BaseCharacterMovementComponent->StartSprint();
	}

	if (!bIsSprintRequested && BaseCharacterMovementComponent->IsSprinting())			//если кнопка спринта не нажата но мы бежим
	{
		BaseCharacterMovementComponent->StopSprint();
	}
}

