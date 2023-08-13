// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Characters/Animations/DDBaseCharacterAnimInstance.h"
#include <GameCode/LedgeDetectorComponents.h>
#include "BaseCharacterMovementComponent.generated.h"


struct FPullUpMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotation = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;


	float Duration = 1.0f;
	float StartTime = 0.0f;

	UCurveVector* PullUpCurve;
};


UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_PullUp UMETA(DisplayName = "PullUp"),
	CMOVE_Max UMETA(Hidden)
};


/**
 * 
 */
UCLASS()
class GAMECODE_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	

public:
	FORCEINLINE bool IsSprinting() { return bIsSprinting; };

	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();

	void UpdateStamina();
	void DelayedFunction();

	void StartPullUp(const FPullUpMovementParameters& PullUpParameters);

	void EndPullUp();
	bool IsPullUp() const;

protected:

	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMove, uint8 PreviousCustomMode) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Swimming", meta=(ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleRadius = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Swimming", meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleHalfHeight = 60.0f;

private:
	bool bIsSprinting;

	float Stamina = 100.0f;
	const float StaminaUpdateInterval = 0.1f;
	FTimerHandle StaminaUpdateTimerHandle;
	FTimerHandle TimerHandle_Delay;

	float DelaySeconds = 3.0f;


	FPullUpMovementParameters CurrentPullUpParameters;

	FTimerHandle PullUpTimer;

};
