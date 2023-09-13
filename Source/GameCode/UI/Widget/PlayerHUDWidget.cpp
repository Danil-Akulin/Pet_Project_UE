// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerHUDWidget.h"
#include "Characters/DDBaseCharacter.h"
#include "Components/CharacterComponents/CharacterAttributeComponent.h"

float UPlayerHUDWidget::GetHealthPercent() const
{
	float Result = 1.0f;
	APawn* Pawn = GetOwningPlayerPawn();
	ADDBaseCharacter* Character = Cast<ADDBaseCharacter>(Pawn);
	if (IsValid(Character))
	{
		const UCharacterAttributeComponent* CharacterAttrebutes = Character->GetCharacterAttributesComponent();
		Result = CharacterAttrebutes->GetHealthPercent();
	}
	return Result;
}
