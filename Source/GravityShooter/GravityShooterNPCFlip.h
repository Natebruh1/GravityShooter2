// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityShooterNPC.h"
#include "GravityShooterNPCFlip.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYSHOOTER_API AGravityShooterNPCFlip : public AGravityShooterNPC
{
	GENERATED_BODY()
public:
	FTimerHandle flipTimerHandle;
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	float timerLength;
	void flipGravity();
};
