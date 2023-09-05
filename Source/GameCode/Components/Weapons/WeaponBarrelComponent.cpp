// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBarrelComponent.h"
#include "DD_Types.h"
#include <DrawDebugHelpers.h>
#include "Subsystems/DebugSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UWeaponBarrelComponent::Shot()
{
	FVector ShotStart = GetComponentLocation();
	FVector ShotDirection = GetComponentRotation().RotateVector(FVector::ForwardVector);
	FVector ShotEnd = ShotStart + Range * ShotDirection;

#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubSystem->IsCategoryEnabled(DebugCategoryRangeWeapon);
#else
	bool bIsDebugEnabled = false;
#endif

	FHitResult ShotResult;
	if (GetWorld()->LineTraceSingleByChannel(ShotResult, ShotStart, ShotEnd, ECC_Bullet))
	{
		ShotEnd = ShotResult.ImpactPoint;
		if (bIsDebugEnabled)
		{
			DrawDebugSphere(GetWorld(), ShotEnd, 15.0f, 24, FColor::Cyan, false, 1.0f);
		}
	}
	if (bIsDebugEnabled)
	{
		DrawDebugLine(GetWorld(), ShotStart, ShotEnd, FColor::Red, false, 1.0f, 0, 3.0f);
	}
}