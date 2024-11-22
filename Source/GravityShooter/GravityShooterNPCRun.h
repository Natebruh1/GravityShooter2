// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityShooterNPC.h"



#include "GravityShooterNPCRun.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYSHOOTER_API AGravityShooterNPCRun : public AGravityShooterNPC
{
	GENERATED_BODY()
public:
	virtual void Tick(float deltaTime) override;
	UPROPERTY(EditAnywhere)
	float moveSpeed = 10.f;
};
