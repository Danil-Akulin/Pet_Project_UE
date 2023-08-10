﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "LedgeDetectorComponents.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DD_Types.h"
#include "DrawDebugHelpers.h"
#include "Utils/GCTraceUtils.h"

// Called when the game starts
void ULedgeDetectorComponents::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());

}

bool ULedgeDetectorComponents::DetectLedge(OUT FLedgeDescription& LedgeDescription)
{
	UCapsuleComponent* CapsuleComponent = CachedCharacterOwner->GetCapsuleComponent();
	//
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());
	//
	float BottomZOffset = 2.0f;
	FVector CharacterBottom = CachedCharacterOwner->GetActorLocation() - (CapsuleComponent->GetScaledCapsuleHalfHeight() - BottomZOffset) * FVector::UpVector;
	

	//Forward check
	float ForwardCheckCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float ForwardCheckCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * 0.5f;

	FHitResult ForwardCheckHitResult;
	FVector ForwardStartLocation = CharacterBottom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardEndLocation = ForwardStartLocation + CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;

	float DrawTime = 2.0f;

	if (!GCTraceUtils::SweepCapsuleSingleByProfile(GetWorld(), ForwardCheckHitResult, ForwardStartLocation, ForwardEndLocation, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight, FQuat::Identity, ECC_Climbing, QueryParams, FCollisionResponseParams::DefaultResponseParam, true, DrawTime))
	{
		return false;
	}

	//Downward check
	

	FHitResult DownwardCheckHitResult;
	float DownwardSphereCheckRadius = CapsuleComponent->GetScaledCapsuleRadius();	//хранит размер капсулы

	float DownwardCheckDepthOffset = 10.0f;		//при перемещении игрока что бы стартовая позиция находилась глубже
	FVector DownwardStartLocation = ForwardCheckHitResult.ImpactPoint + ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereCheckRadius;			//Вычисляем максимальную позицую куда мы хотим забраться
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);



	if (!GCTraceUtils::SweepSphereSingleByProfile(GetWorld(), DownwardCheckHitResult, DownwardStartLocation, DownwardEndLocation, DownwardSphereCheckRadius, ECC_Climbing, QueryParams, FCollisionResponseParams::DefaultResponseParam, true, DrawTime))
	{
		return false;
	}

	//Overlap check

	float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();						//радиус капсулы
	float OverlapCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();				//половина высоты радиуса
	float OverlapCapsuleFloatOffset = 1.0f;
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + (OverlapCapsuleHalfHeight + OverlapCapsuleFloatOffset) * FVector::UpVector;				//храним местоположение которое будет проверяться на пересечение



	if (GCTraceUtils::OverlapCapsuleByProfile(GetWorld(), OverlapLocation, OverlapCapsuleRadius, OverlapCapsuleHalfHeight, FQuat::Identity, FName("Pawn"), QueryParams, true, DrawTime))
	{
		false;
	}


	LedgeDescription.Location = DownwardCheckHitResult.ImpactPoint;
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator();

	return true;
}