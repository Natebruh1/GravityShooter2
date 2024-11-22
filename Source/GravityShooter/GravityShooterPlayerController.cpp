// Copyright Epic Games, Inc. All Rights Reserved.


#include "GravityShooterPlayerController.h"
#include "GravityCameraManager.h"
#include "EnhancedInputSubsystems.h"

void AGravityShooterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AGravityCameraManager* camM = Cast<AGravityCameraManager>((PlayerCameraManager.Get()));

	
}

void AGravityShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}

	
	
	
}

AGravityShooterPlayerController::AGravityShooterPlayerController()
{
	
}
