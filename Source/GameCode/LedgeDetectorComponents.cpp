// Fill out your copyright notice in the Description page of Project Settings.


#include "LedgeDetectorComponents.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DD_Types.h"
#include "DrawDebugHelpers.h"

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

	float DrawTime = 2.0f;

	DrawDebugCapsule(GetWorld(), ForwardStartLocation, ForwardCheckCapsuleHalfHeight, ForwardCheckCapsuleRadius, FQuat::Identity, FColor::Blue, false, DrawTime);
	DrawDebugCapsule(GetWorld(), ForwardEndLocation, ForwardCheckCapsuleHalfHeight, ForwardCheckCapsuleRadius, FQuat::Identity, FColor::Blue, false, DrawTime);
	if(!GetWorld()->SweepSingleByChannel(ForwardCheckHitResult, ForwardStartLocation, ForwardEndLocation, FQuat::Identity, ECC_Climbing, ForwardCapsuleShape, QueryParams))
	{
		return false;
	}
	DrawDebugCapsule(GetWorld(), ForwardCheckHitResult.Location, ForwardCheckCapsuleHalfHeight, ForwardCheckCapsuleRadius, FQuat::Identity, FColor::Blue, false, DrawTime);
	DrawDebugPoint(GetWorld(), ForwardCheckHitResult.ImpactPoint, 10.0f, FColor::Red, false, DrawTime);

	//Downward check
	

	FHitResult DownwardCheckHitResult;
	float DownwardSphereCheckRadius = CapsuleComponent->GetScaledCapsuleRadius();	//хранит размер капсулы
	FCollisionShape DownwardSphereShape = FCollisionShape::MakeSphere(DownwardSphereCheckRadius);	//хранит размер сферы

	float DownwardCheckDepthOffset = 10.0f;		//при перемещении игрока что бы стартовая позиция находилась глубже
	FVector DownwardStartLocation = ForwardCheckHitResult.ImpactPoint + ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereCheckRadius;			//Вычисляем максимальную позицую куда мы хотим забраться
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);

	if (!GetWorld()->SweepSingleByChannel(DownwardCheckHitResult, DownwardStartLocation, DownwardEndLocation, FQuat::Identity, ECC_Climbing, DownwardSphereShape, QueryParams))
	{
		return false;
	}

	DrawDebugSphere(GetWorld(), DownwardCheckHitResult.Location, DownwardSphereCheckRadius, 30, FColor::Cyan, false, DrawTime);
	DrawDebugPoint(GetWorld(), DownwardCheckHitResult.ImpactPoint, 15.0f, FColor::Red, false, DrawTime);

	//Overlap check

	float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();						//радиус капсулы
	float OverlapCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();				//половина высоты радиуса
	FCollisionShape OverlapCapsuleShape = FCollisionShape::MakeCapsule(OverlapCapsuleRadius, OverlapCapsuleHalfHeight);			//создаём капсулу
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + OverlapCapsuleHalfHeight * FVector::UpVector;				//храним местоположение которое будет проверяться на пересечение

	if (GetWorld()->OverlapAnyTestByProfile(OverlapLocation, FQuat::Identity, FName("Pawn"), OverlapCapsuleShape, QueryParams))
	{
		DrawDebugCapsule(GetWorld(), OverlapLocation, OverlapCapsuleHalfHeight, OverlapCapsuleRadius, FQuat::Identity, FColor::Black, false, DrawTime);
		return false;
	}

	LedgeDescription.Location = DownwardCheckHitResult.ImpactPoint;
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator();

	return true;
}