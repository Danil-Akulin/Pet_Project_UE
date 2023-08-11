// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"

bool UDDGameInstance::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool bResult = Super::ProcessConsoleExec(Cmd, Ar, Executor);

	if (!bResult)
	{
		TArray<UGameInstanceSubsystem*> Subsystem = GetSubsystemArray<UGameInstanceSubsystem>();
		for (UGameInstanceSubsystem* CurrentSubsystem : Subsystem)
		{
			bResult = bResult || CurrentSubsystem->ProcessConsoleExec(Cmd, Ar, Executor);
		}
	}

	return bResult;
}