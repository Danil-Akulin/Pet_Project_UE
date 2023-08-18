// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameCode/DD_Types.h"

ALadder::ALadder()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LadderRoot"));

	LeftRailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftRail"));
	LeftRailMesh->SetupAttachment(RootComponent);

	RightRailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightRail"));
	RightRailMesh->SetupAttachment(RootComponent);

	StepsMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Steps"));
	StepsMesh->SetupAttachment(RootComponent);

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName("CollisionProfilePawnInteractionVolume");
}

void ALadder::OnConstruction(const FTransform& Transform)
{
	LeftRailMesh->SetRelativeLocation(FVector(0.0f, -LadderWidth * 0.5f, LadderHeight * 0.5f));
	RightRailMesh->SetRelativeLocation(FVector(0.0f, LadderWidth * 0.5f, LadderHeight * 0.5f));

	UStaticMesh* LeftRailMeshComponent = LeftRailMesh->GetStaticMesh();
	if (IsValid(LeftRailMeshComponent))
	{
		float MeshHeight = LeftRailMeshComponent->GetBoundingBox().GetSize().Z;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			LeftRailMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, LadderHeight / MeshHeight));
		}
	}

	UStaticMesh* RightRailMeshComponent = RightRailMesh->GetStaticMesh();
	if (IsValid(RightRailMeshComponent))
	{
		float MeshHeight = RightRailMeshComponent->GetBoundingBox().GetSize().Z;
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			RightRailMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, LadderHeight / MeshHeight));
		}
	}
	
	UStaticMesh* StepsMeshComponent = StepsMesh->GetStaticMesh();
	if (IsValid(StepsMeshComponent))
	{
		float MeshWidth = StepsMeshComponent->GetBoundingBox().GetSize().Y;
		if (!FMath::IsNearlyZero(MeshWidth))
		{
			StepsMesh->SetRelativeScale3D(FVector(0.1f, LadderWidth / MeshWidth, 1.0f));
		}	
	}
	
	StepsMesh->ClearInstances();

	uint32 StepsCount = FMath::FloorToInt((LadderHeight - BottomStepOffset) / StepsInterval);

	for (uint32 i = 0; i < StepsCount; ++i)
	{
		FTransform InstanceTransform(FVector(0.0f, 0.0f, BottomStepOffset + i * StepsInterval));
		StepsMesh->AddInstance(InstanceTransform);
	}

	float BoxDepthExtend = GetLadderInteractionVolume()->GetUnscaledBoxExtent().X;
	GetLadderInteractionVolume()->SetBoxExtent(FVector(BoxDepthExtend, LadderWidth * 0.5f, LadderHeight * 0.5f));
	GetLadderInteractionVolume()->SetRelativeLocation(FVector(BoxDepthExtend, 0.0f, LadderHeight * 0.5f));
}

UBoxComponent* ALadder::GetLadderInteractionVolume() const
{
	return StaticCast<UBoxComponent*>(InteractionVolume);
}

