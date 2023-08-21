// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"
#include "../../Characters/DDBaseCharacter.h"
#include "Components/CapsuleComponent.h "

void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(InteractionVolume))
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeOverlapBegin);
		InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeOverlapEnd);
	}
}
void AInteractiveActor::OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsOverlapCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}
	ADDBaseCharacter* BaseCharacter = StaticCast<ADDBaseCharacter*>(OtherActor);	
	BaseCharacter->RegisterInteractiveActor(this);

}
void AInteractiveActor::OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsOverlapCharacterCapsule(OtherActor, OtherComp))
	{ 
		return;
	}
	ADDBaseCharacter* BaseCharacter = StaticCast<ADDBaseCharacter*>(OtherActor);
	BaseCharacter->UnRegisterInteractiveActor(this);
}

bool AInteractiveActor::IsOverlapCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	ADDBaseCharacter* BaseCharacter = Cast<ADDBaseCharacter>(OtherActor);
	if (!IsValid(BaseCharacter))
	{
		return false;
	}

	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return false;
	}
	return true;
}

