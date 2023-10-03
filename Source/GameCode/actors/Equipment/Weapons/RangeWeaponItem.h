// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "actors/Equipment/EquipableItem.h"
#include "RangeWeaponItem.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32);



/**
 * 
 */
class UAnimMontage;
UCLASS(BLueprintable)
class GAMECODE_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	ARangeWeaponItem();

	void Fire();
	void StopFire();

	int32 GetAmmo() const;
	void SetAmmo(int32 NewAmmo);
	bool CanShoot() const;

	FOnAmmoChanged OnAmmoChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWeaponBarrelComponent* WeaponBarell;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Weapon")
	UAnimMontage* WeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations | Character")
	UAnimMontage* CharacterFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Ammo")
	int32 MaxAmmo = 30;

private:
	float PlayAnimMontage(UAnimMontage* AnimMontage);

	int32 Ammo = 0;

	bool bIsFiring = false;
	FTimerHandle ShotTimer;
};