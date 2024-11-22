// Copyright Epic Games, Inc. All Rights Reserved.

#include "GravityShooterGameMode.h"
#include "GravityShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGravityShooterGameMode::AGravityShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
