// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponBarrelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECODE_API UWeaponBarrelComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	
	void Shot(FVector ShotStart, FVector ShotDirection, AController* Controller);
		
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel attributes")
	float Range = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel attributes | Damage")
	float DamageAmount = 50.0f;

};
