// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawnAnimInstance.h"
#include "SpiderPawnAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMECODE_API USpiderPawnAnimInstance : public UBasePawnAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadonly, Category = "Spider bot|IK settings")
	FVector RightFrontFootEffectorLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadonly, Category = "Spider bot|IK settings")
	FVector RightRearFootEffectorLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadonly, Category = "Spider bot|IK settings")
	FVector LeftFrontFootEffectorLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadonly, Category = "Spider bot|IK settings")
	FVector LeftRearFootEffectorLocation = FVector::ZeroVector;

private:
	TWeakObjectPtr<class ASpiderPawn> CachedSpiderPawnOwner;
};
