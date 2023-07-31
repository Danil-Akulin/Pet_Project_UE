// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GCBaseMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAMECODE_API UGCBaseMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

		virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void JumpStart();

	virtual bool IsFalling() const override;

protected:
	UPROPERTY(EditAnywhere)
		float MaxSpeed = 1200.0f;

	UPROPERTY(EditAnywhere)
		float InitialJumpVelocity = 500.0f;        //Высота прыжка

	UPROPERTY(EditAnywhere)
		bool bEnableGravity;					   //Включаем гравитацию у Pawn

	UPROPERTY(EditAnywhere, Category = "Delay")
		float JumpCooldown = 1.0f;

	bool bCanJump = true;

	FTimerHandle JumpCooldownTimerHandle;



private:
	FVector VerticalVelocity = FVector::ZeroVector;//Пустой вектор
	bool bIsFalling = false;                       //Падаем ли мы?
};