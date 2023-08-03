// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Characters/Animations/DDBaseCharacterAnimInstance.h"
#include "BaseCharacterMovementComponent.generated.h"

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

protected:

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

};
