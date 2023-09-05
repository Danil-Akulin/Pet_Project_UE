// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../PlayerCharacter.h"
#include "DD_Types.h"
#include "DDBaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMECODE_API UDDBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	float Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	bool bIsSwimming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	bool bIsOnLadder = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	float LadderSpeedRatio = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	bool bIsStrafe = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation", meta = (UIMin = -180.0f, UIMax = 180.0f))
	float Direction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	EEquippedItemType CurrentEquippedType = EEquippedItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Animation")
	FRotator AimRotation = FRotator::ZeroRotator;

private:
	TWeakObjectPtr<class ADDBaseCharacter> CachedBaseCharacter;
};
