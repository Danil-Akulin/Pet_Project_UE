// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEquipmentComponent.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Characters/DDBaseCharacter.h"
#include "DD_Types.h"

EEquippedItemType UCharacterEquipmentComponent::GetCurrentEquippedItemType() const
{
	EEquippedItemType Result = EEquippedItemType::None;
	if (IsValid(CurrentEquippedItem))
	{
		Result = CurrentEquippedItem->GetItemType();
	}
	return Result;
}

void UCharacterEquipmentComponent::Fire()
{
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->Fire();
	}
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedBaseCharacter = StaticCast<ADDBaseCharacter*>(GetOwner());
	CreateLoadout();
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	if (!IsValid(SideArmClass))
	{
		return;
	}
	CurrentEquippedItem = GetWorld()->SpawnActor<ARangeWeaponItem>(SideArmClass);
	CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketCharacterWeapon);
	CurrentEquippedItem->SetOwner(CachedBaseCharacter.Get());
	CurrentEquippedItem->OnAmmoChanged.AddUFunction(this, FName("OnCurrentAmmoChanged"));
	OnCurrentAmmoChanged(CurrentEquippedItem->GetAmmo());
}

void UCharacterEquipmentComponent::OnCurrentAmmoChanged(int32 Ammo)
{
	if (OnCurrentWeaponAmmoChangedEvent.IsBound())
	{
		OnCurrentWeaponAmmoChangedEvent.Broadcast(Ammo);
	}
}
