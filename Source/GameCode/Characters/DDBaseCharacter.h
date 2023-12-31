// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Curves/CurveVector.h"
#include "DDBaseCharacter.generated.h"

USTRUCT(BlueprintType)
struct FPullUpSettings
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* PullUpMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* PullUpCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeightStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeightStartTime = 0.5f;

};

class UCharacterEquipmentComponent;
class AInteractiveActor;
class UBaseCharacterMovementComponent;
class UCharacterAttributeComponent;
UCLASS(Abstract, NotBlueprintable)
class GAMECODE_API ADDBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ADDBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};

	virtual void ChangeCrouchState();

	virtual void StartSprint();
	virtual void StopSprint();

	virtual void Tick(float DeltaTime) override;

	virtual bool CanJumpInternal_Implementation() const override;

	virtual void SwimForward(float Value) {};
	virtual void SwimRight(float Value) {};
	virtual void SwimUp(float Value) {};

	virtual void PullUp(bool bForce);

	FORCEINLINE UBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const { return BaseCharacterMovementComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class ULedgeDetectorComponents* LedgeDetectorComponent;

	void RegisterInteractiveActor(AInteractiveActor* InteractiveActor);
	void UnRegisterInteractiveActor(AInteractiveActor* InteractiveActor);

	void PullUpLadder(float Value);
	void InteractionWithLadder();
	const class ALadder* GetAvailableLadder() const;

	virtual void Falling() override;
	virtual void NotifyJumpApex() override;
	virtual void Landed(const FHitResult& Hit) override;

	const UCharacterEquipmentComponent* GetCharacterEquipmentComponent() const;

	UCharacterEquipmentComponent* GetCharacterEquipmentComponent_Mutable() const;

	void Fire();

	UCharacterAttributeComponent* GetCharacterAttributesComponent() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	float SprintSpeed = 790.0f;

	virtual bool CanSprint();

	bool CanPullUp() const;

	UBaseCharacterMovementComponent* BaseCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | PullUp")
	FPullUpSettings HighPullUpSettings;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | PullUp")
	FPullUpSettings LowPullUpSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | PullUp", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float LowPullUpMaxHeight = 125.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	class UCharacterAttributeComponent* CharacterAttributesComponent;

	virtual void OnDeath();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Anim")
	class UAnimMontage* OnDeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Attributes")
	class UCurveFloat* FallDamageCurve;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	class UCharacterEquipmentComponent* CharacterEquipmentComponent;
	
private:

	void TryChangeSprintState();
	bool bIsSprintRequested = false;

	const FPullUpSettings& GetPullUpSettings(float LedgeHeight) const;

	void ResetPullUpFlag();

	bool bIsPullingUp = false;
	FTimerHandle PullUpResetTimer;

	bool bIsAlive = true;

	TArray<AInteractiveActor*> AvailableInteractiveActors;

	void EnableRegdoll();

	FVector CurrentFallLenght;
};
