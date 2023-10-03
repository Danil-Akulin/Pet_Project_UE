// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "Characters/DDBaseCharacter.h"
#include "Components/CharacterComponents/CharacterAttributeComponent.h"
#include <GameFramework/Pawn.h>
#include "Blueprint/WidgetTree.h"
#include "AmmoWidget.h"

class UAmmoWidget* UPlayerHUDWidget::GetAmmoWidget()
{
	return WidgetTree->FindWidget<UAmmoWidget>(AmmoWidgetName);
}

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
