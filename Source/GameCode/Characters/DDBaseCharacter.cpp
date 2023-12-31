﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DDBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/MovementComponents/BaseCharacterMovementComponent.h"
#include "LedgeDetectorComponents.h"
#include "actors/Interactive/Environment/Ladder.h"
#include "Components/CharacterComponents/CharacterAttributeComponent.h"
#include "DD_Types.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"

ADDBaseCharacter::ADDBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	BaseCharacterMovementComponent = StaticCast<UBaseCharacterMovementComponent*>(GetCharacterMovement());

	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponents>(TEXT("LedgeDetector"));
	CharacterAttributesComponent = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("CharacterAttributes"));

	CharacterEquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("CharacterEquipment"));
}

void ADDBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAttributesComponent->FOnDeathEvent.AddUObject(this, &ADDBaseCharacter::OnDeath);
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

void ADDBaseCharacter::PullUp(bool bForce /*= false*/)
{
	if (!bIsAlive)
	{
		return;
	}

	if (!(CanPullUp() || bForce))
	{
		return;
	}

	FLedgeDescription LedgeDiscriptin;
	if (LedgeDetectorComponent->DetectLedge(LedgeDiscriptin))
	{

		FPullUpMovementParameters PullUpParameters;
		PullUpParameters.PullUpCurve = HighPullUpSettings.PullUpCurve;
		PullUpParameters.InitialLocation = GetActorLocation();
		PullUpParameters.InitialRotation = GetActorRotation();
		PullUpParameters.TargetLocation = LedgeDiscriptin.Location;
		PullUpParameters.TargetRotation = LedgeDiscriptin.Rotation;

		float PullUpHeight = (PullUpParameters.TargetLocation - PullUpParameters.InitialLocation).Z;

		const FPullUpSettings& PullUpSettings = GetPullUpSettings(PullUpHeight);

		float MinRange;
		float MaxRange;	
		PullUpSettings.PullUpCurve->GetTimeRange(MinRange, MaxRange);

		PullUpParameters.Duration = MaxRange - MinRange;

		PullUpParameters.PullUpCurve = PullUpSettings.PullUpCurve;
		FVector2D SourceRange(PullUpSettings.MinHeight, PullUpSettings.MaxHeight);
		FVector2D TargetRange(PullUpSettings.MinHeightStartTime, PullUpSettings.MaxHeightStartTime);
		PullUpParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, PullUpHeight);

		PullUpParameters.InitialAnimationLocation = PullUpParameters.TargetLocation - PullUpSettings.AnimationCorrectionZ * FVector::UpVector + PullUpSettings.AnimationCorrectionXY * LedgeDiscriptin.LedgeNormal;

		if (bIsPullingUp)
		{
			return;
		}
		bIsPullingUp = true;
		GetBaseCharacterMovementComponent()->StartPullUp(PullUpParameters);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(PullUpSettings.PullUpMontage, 1.0f, EMontagePlayReturnType::Duration, PullUpParameters.StartTime);

		float AnimationDuration = PullUpSettings.PullUpMontage->GetPlayLength();
		GetWorldTimerManager().SetTimer(PullUpResetTimer, this, &ADDBaseCharacter::ResetPullUpFlag, AnimationDuration, false);

	}
}

void ADDBaseCharacter::RegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.AddUnique(InteractiveActor);
}

void ADDBaseCharacter::UnRegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.RemoveSingleSwap(InteractiveActor);
}

void ADDBaseCharacter::ResetPullUpFlag()
{
	bIsPullingUp = false;
}


bool ADDBaseCharacter::CanSprint()
{
	return true;
}

bool ADDBaseCharacter::CanPullUp() const
{
	return !GetBaseCharacterMovementComponent()->IsOnLadder();
}

void ADDBaseCharacter::PullUpLadder(float Value)
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder() && !FMath::IsNearlyZero(Value))
	{
		FVector LadderUpVector = GetBaseCharacterMovementComponent()->GetCurrentLadder()->GetActorUpVector();
		AddMovementInput(LadderUpVector, Value);
	}
}

void ADDBaseCharacter::InteractionWithLadder()
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder())
	{
		GetBaseCharacterMovementComponent()->DetachFromLadder(EDetachFromLadderMethod::JumpOff);
	}
	else
	{
		const ALadder* AvailableLadder = GetAvailableLadder();
		if (IsValid(AvailableLadder))
		{
			if (AvailableLadder->GetIsOnTop())
			{
				PlayAnimMontage(AvailableLadder->GetAttachFromTopAnimMontage());
			}
			GetBaseCharacterMovementComponent()->AttachToLadder(AvailableLadder);
		}

	}
}

const ALadder* ADDBaseCharacter::GetAvailableLadder() const
{
	const ALadder* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<ALadder>())
		{
			Result = StaticCast<const ALadder*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

void ADDBaseCharacter::Falling()
{
	GetBaseCharacterMovementComponent()->bNotifyApex = true;
}

void ADDBaseCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	CurrentFallLenght = GetActorLocation();
}

void ADDBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	float FallHeight = (CurrentFallLenght - GetActorLocation()).Z * 0.01f;
	if (IsValid(FallDamageCurve))
	{
		float DamageAmount = FallDamageCurve->GetFloatValue(FallHeight);
		TakeDamage(DamageAmount, FDamageEvent(), GetController(), Hit.Actor.Get());
	}
}

const UCharacterEquipmentComponent* ADDBaseCharacter::GetCharacterEquipmentComponent() const
{
	return CharacterEquipmentComponent;
}

UCharacterEquipmentComponent* ADDBaseCharacter::GetCharacterEquipmentComponent_Mutable() const
{
	return CharacterEquipmentComponent;
}

void ADDBaseCharacter::Fire()
{
	CharacterEquipmentComponent->Fire();
}

UCharacterAttributeComponent* ADDBaseCharacter::GetCharacterAttributesComponent() const
{
	return CharacterAttributesComponent;
}

void ADDBaseCharacter::OnDeath()
{
	bIsAlive = false;
	GetCharacterMovement()->DisableMovement();
	float AnimationOnDeathDuration = PlayAnimMontage(OnDeathAnimMontage);
	if (AnimationOnDeathDuration == 0.0f)
	{
		EnableRegdoll();
	}
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

const FPullUpSettings& ADDBaseCharacter::GetPullUpSettings(float LedgeHeight) const
{
	return LedgeHeight > LowPullUpMaxHeight ? HighPullUpSettings : LowPullUpSettings;
}


void ADDBaseCharacter::EnableRegdoll()
{
	GetMesh()->SetCollisionProfileName(CollisionProfileRegdoll);
	GetMesh()->SetSimulatePhysics(true);
}