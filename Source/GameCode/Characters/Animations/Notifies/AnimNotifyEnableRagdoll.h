// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotifyEnableRagdoll.generated.h"

/**
 * 
 */
UCLASS()
class GAMECODE_API UAnimNotifyEnableRagdoll : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation);
};
