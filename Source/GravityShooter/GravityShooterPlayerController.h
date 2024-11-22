// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GravityShooterPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class GRAVITYSHOOTER_API AGravityShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:

	virtual void BeginPlay() override;
	AGravityShooterPlayerController();
	// End Actor interface
};
