// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DD_Types.h"
#include "CharacterEquipmentComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponAmmoChanged, int32);


class ARangeWeaponItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECODE_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	EEquippedItemType GetCurrentEquippedItemType() const;

	void Fire();

	FOnCurrentWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;

virtual void BeginPlay() override;



protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TSubclassOf<ARangeWeaponItem> SideArmClass;


private:
	void CreateLoadout();

	UFUNCTION()
	void OnCurrentAmmoChanged(int32 NewAmmo);

	ARangeWeaponItem* CurrentEquippedItem;
	TWeakObjectPtr<class ADDBaseCharacter> CachedBaseCharacter;
};
