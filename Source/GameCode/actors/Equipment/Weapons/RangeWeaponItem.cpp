// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponItem.h"
#include "Components/Weapons/WeaponBarrelComponent.h"
#include "DD_Types.h"
#include "Characters/DDBaseCharacter.h"

void ARangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	SetAmmo(MaxAmmo);
}

ARangeWeaponItem::ARangeWeaponItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarrelComponent>(TEXT("WeaponBarell"));
	WeaponBarell->SetupAttachment(WeaponMesh, SocketWeaponMuzzle);
}

void ARangeWeaponItem::Fire()
{
	ADDBaseCharacter* CharacterOwner = StaticCast<ADDBaseCharacter*>(GetOwner());

	if (!CanShoot())
	{
		StopFire();
		return;
	}

	CharacterOwner->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);


	APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
	if (!IsValid(Controller))
	{
		return;
	}
	bIsFiring = true;
	FVector PlayerViewPoint;
	FRotator PlayerViewRotation;

	Controller->GetPlayerViewPoint(PlayerViewPoint, PlayerViewRotation);

	FVector ViewDirection = PlayerViewRotation.RotateVector(FVector::ForwardVector);

	SetAmmo(Ammo - 1);

	WeaponBarell->Shot(PlayerViewPoint, ViewDirection, Controller);
}

void ARangeWeaponItem::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
	bIsFiring = false;
}

int32 ARangeWeaponItem::GetAmmo() const
{
	return Ammo;
}

void ARangeWeaponItem::SetAmmo(int32 NewAmmo)
{
	Ammo = NewAmmo;
	if (OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(Ammo);
	}
}

bool ARangeWeaponItem::CanShoot() const
{
	return Ammo > 0;
}

float ARangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	return WeaponAnimInstance->Montage_Play(AnimMontage);
}
