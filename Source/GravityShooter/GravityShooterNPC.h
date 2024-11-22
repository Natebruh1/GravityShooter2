// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityShooterCharacter.h"
#include "GravityShooterNPC.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYSHOOTER_API AGravityShooterNPC : public AGravityShooterCharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FVector DefaultGravity;
protected:
	virtual void BeginPlay();
};
