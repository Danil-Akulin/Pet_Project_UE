// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBarrelComponent.h"
#include "DD_Types.h"
#include <DrawDebugHelpers.h>
#include "Subsystems/DebugSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

void UWeaponBarrelComponent::Shot(FVector ShotStart, FVector ShotDirection, AController* Controller)
{
	FVector Muzzle = GetComponentLocation();
	FVector ShotEnd = ShotStart + Range * ShotDirection;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashFX, Muzzle, GetComponentRotation());

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
		AActor* HitActor = ShotResult.GetActor();

		if (bIsDebugEnabled)
		{
			DrawDebugSphere(GetWorld(), ShotEnd, 15.0f, 24, FColor::Cyan, false, 1.0f);
		}
		if (IsValid(HitActor))
		{
			HitActor->TakeDamage(DamageAmount, FDamageEvent{}, Controller, GetOwner());
		}
	}

	UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, Muzzle, GetComponentRotation());
	TraceFXComponent->SetVectorParameter(FXParamTraceEnd, ShotEnd);


	if (bIsDebugEnabled)
	{
		DrawDebugLine(GetWorld(), Muzzle, ShotEnd, FColor::Red, false, 1.0f, 0, 3.0f);
	}
}