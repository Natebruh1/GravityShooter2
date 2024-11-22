// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityShooterNPCFlip.h"

void AGravityShooterNPCFlip::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(flipTimerHandle, this, &AGravityShooterNPCFlip::flipGravity, timerLength, true);
}

void AGravityShooterNPCFlip::flipGravity()
{
	gravityDirection = -gravityDirection;
	updateGravityFromProp();
}
