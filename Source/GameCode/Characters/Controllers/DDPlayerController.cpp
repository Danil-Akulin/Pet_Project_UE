// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlayerController.h"
#include "../DDBaseCharacter.h"

void ADDPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedBaseCharacter = Cast<ADDBaseCharacter>(InPawn);
}

void ADDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ADDPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ADDPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ADDPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ADDPlayerController::LookUp);
	InputComponent->BindAxis("SwimForward", this, &ADDPlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &ADDPlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &ADDPlayerController::SwimUp);
	InputComponent->BindAxis("PullUpLadder", this, &ADDPlayerController::PullUpLadderUp);
	InputComponent->BindAction("InteractWithLadder", EInputEvent::IE_Pressed, this, &ADDPlayerController::InteractionWithLadder);
	InputComponent->BindAction("PullUp", EInputEvent::IE_Pressed, this, &ADDPlayerController::PullUp);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ADDPlayerController::Jump);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ADDPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ADDPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ADDPlayerController::StopSprint);
}

void ADDPlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}

void ADDPlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

void ADDPlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value);
	}
}

void ADDPlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value);
	}
}

void ADDPlayerController::PullUp()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->PullUp();
	}
}

void ADDPlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}

void ADDPlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void ADDPlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}

void ADDPlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}

void ADDPlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}

void ADDPlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void ADDPlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}

void ADDPlayerController::PullUpLadderUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->PullUpLadder(Value);
	}
}

void ADDPlayerController::InteractionWithLadder()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->InteractionWithLadder();
	}
}
