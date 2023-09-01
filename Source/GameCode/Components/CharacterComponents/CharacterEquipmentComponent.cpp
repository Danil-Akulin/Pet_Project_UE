// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEquipmentComponent.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "Characters/DDBaseCharacter.h"

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
	CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}