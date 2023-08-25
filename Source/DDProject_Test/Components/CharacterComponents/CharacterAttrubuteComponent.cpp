// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttrubuteComponent.h"
#include "Math/UnrealMathUtility.h"
#include <DDProject_Test/Subsystems/DebugSubsystem.h>
#include "Kismet/GameplayStatics.h"
#include "../../Characters/DDBaseCharacter.h"
#include "../../DD_Types.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UCharacterAttrubuteComponent::UCharacterAttrubuteComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCharacterAttrubuteComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedBaseCharacterOwner = StaticCast<ADDBaseCharacter*>(GetOwner());
	Health = MaxHealth;
	CachedBaseCharacterOwner->OnTakeAnyDamage.AddDynamic(this, &UCharacterAttrubuteComponent::OnTakeAnyDamage);
}

//#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
void UCharacterAttrubuteComponent::DebugDrawAttributes()
{
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	if (!DebugSubsystem->IsCategoryEnabled(DebugCategoryCharacterAttributes))
	{
		return;
	}


	FVector TextLocation = CachedBaseCharacterOwner->GetActorLocation() + (CachedBaseCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.0f) * FVector::UpVector;
	DrawDebugString(GetWorld(), TextLocation, FString::Printf(TEXT("Health: %.2f"), Health), nullptr, FColor::Green, 0.0f, true);
} 
//#endif

void UCharacterAttrubuteComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogDamage, Warning, TEXT("UCharacterAttributesComponent::OnTakeAnyDamage %s recieved %.2f amount of damage from %s"), *CachedBaseCharacterOwner->GetName(), Damage, *DamageCauser->GetName());
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

// Called every frame
void UCharacterAttrubuteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

//#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	DebugDrawAttributes();
//#endif
}

