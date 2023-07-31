// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"		
#include "BasePawnAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMECODE_API UBasePawnAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base pawn anim instance")
	float InputForward;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base pawn anim instance")
	float InputRight;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base pawn anim instance")
	bool bIsInAir;

private:
	TWeakObjectPtr<class AGameCodeBasePawn> CachedBasePawn;
};
