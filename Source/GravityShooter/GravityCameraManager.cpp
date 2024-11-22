// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityCameraManager.h"

AGravityCameraManager::AGravityCameraManager()
{
	ViewPitchMax = 179.9f;
	ViewPitchMin = -180.f;

	ViewRollMax = 179.9f;
	ViewRollMin = -180.f;

	ViewYawMax = 179.9f;
	ViewYawMin = -180.f;
	
}

void AGravityCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Yaw Max is %f"), ViewYawMax);
	if (charRef) //If we know the player character
	{
		FVector gravDir = charRef->getGravity();

		
	}
}

void AGravityCameraManager::setCharRef(AGravityShooterCharacter* newCharRef)
{
	charRef = newCharRef;
}
