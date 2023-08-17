// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"
#include "../../Characters/DDBaseCharacter.h"
#include "Components/CapsuleComponent.h "

// Sets default values
/*
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}*/

// Called when the game starts or when spawned
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
	ADDBaseCharacter* BaseCharacter = Cast<ADDBaseCharacter>(OtherActor);
	if (IsValid(BaseCharacter))
	{
		return;
	}

	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return;
	}

	BaseCharacter->RegisterInteractiveActor(this);

}
void AInteractiveActor::OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADDBaseCharacter* BaseCharacter = Cast<ADDBaseCharacter>(OtherActor);
	if (IsValid(BaseCharacter))
	{
		return;
	}

	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return;
	}
	BaseCharacter->UnRegisterInteractiveActor(this);
}
/*
// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
