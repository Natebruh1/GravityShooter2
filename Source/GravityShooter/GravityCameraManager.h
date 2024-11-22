// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GravityShooterCharacter.h"
#include "GravityCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYSHOOTER_API AGravityCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	AGravityCameraManager();
	virtual void Tick(float DeltaTime) override;
	void setCharRef(AGravityShooterCharacter* newCharRef);
protected:
	AGravityShooterCharacter* charRef;
	
};
