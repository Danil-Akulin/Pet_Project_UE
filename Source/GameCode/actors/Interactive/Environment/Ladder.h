// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InteractiveActor.h"
#include "Ladder.generated.h"

 /**
 * 
 */
class UStaticMeshComponent;
UCLASS(Blueprintable)
class GAMECODE_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()
public:
	ALadder();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder parameters")
	float LadderHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters", meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float LadderWidth = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder parameters")
	float StepsInterval = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder parameters")
	float BottomStepOffset = 25.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightRailMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeftRailMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInstancedStaticMeshComponent* StepsMesh;

	class UBoxComponent* GetLadderInteractionVolume() const;
};