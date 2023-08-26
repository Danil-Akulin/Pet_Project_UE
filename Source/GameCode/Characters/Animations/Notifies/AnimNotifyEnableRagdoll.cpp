// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyEnableRagdoll.h"
#include "../../../DD_Types.h"

void UAnimNotifyEnableRagdoll::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation)
{
	MeshComponent->SetCollisionProfileName(CollisionProfileRegdoll);
	MeshComponent->SetSimulatePhysics(true);
}
