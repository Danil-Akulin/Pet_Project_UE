// Fill out your copyright notice in the Description page of Project Settings.


#include "LedgeDetectorComponents.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DD_Types.h"

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
	FCollisionShape ForwardCapsuleShape = FCollisionShape::MakeCapsule(ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight);
	FVector ForwardStartLocation = CharacterBottom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardEndLocation = ForwardStartLocation + CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;

	if(!GetWorld()->SweepSingleByChannel(ForwardCheckHitResult, ForwardStartLocation, ForwardEndLocation, FQuat::Identity, ECC_Climbing, ForwardCapsuleShape, QueryParams))
	{
		return false;
	}

	
	//Downward check

	FHitResult DownwardCheckHitResult;
	float DownwardSphereCheckRadius = CapsuleComponent->GetScaledCapsuleRadius();
	FCollisionShape DownwardSphereShape = FCollisionShape::MakeSphere(DownwardSphereCheckRadius);

	float DownwardCheckDepthOffset = 10.0f;
	FVector DownwardStartLocation = ForwardCheckHitResult.ImpactPoint + ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereCheckRadius;
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);

	if (!GetWorld()->SweepSingleByChannel(DownwardCheckHitResult, DownwardStartLocation, DownwardEndLocation, FQuat::Identity, ECC_Climbing, DownwardSphereShape, QueryParams))
	{
		return false;
	}

	//Overlap check

	float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float OverlapCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	FCollisionShape OverlapCapsuleShape = FCollisionShape::MakeCapsule(OverlapCapsuleRadius, OverlapCapsuleHalfHeight);
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + OverlapCapsuleHalfHeight * FVector::UpVector;

	if (GetWorld()->OverlapAnyTestByProfile(OverlapLocation, FQuat::Identity, FName("Pawn"), OverlapCapsuleShape, QueryParams))
	{
		return false;
	}

	LedgeDescription.Location = DownwardCheckHitResult.ImpactPoint;
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator();

	return true;
}