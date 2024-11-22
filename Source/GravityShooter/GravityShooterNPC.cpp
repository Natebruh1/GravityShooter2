// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityShooterNPC.h"

void AGravityShooterNPC::BeginPlay()
{
	Super::BeginPlay();
	if (DefaultGravity!=FVector(0.f,0.f,0.f))
	{
		gravityDirection = DefaultGravity;
		updateGravityFromProp();
	}
	
}
