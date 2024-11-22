// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityShooterNPCRun.h"
#include "Camera/CameraComponent.h"
void AGravityShooterNPCRun::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	if (Controller != nullptr)
	{
		AddMovementInput(GetFirstPersonCameraComponent()->GetForwardVector(), moveSpeed*deltaTime);
		GetFirstPersonCameraComponent()->AddRelativeRotation(FRotator(0.f,moveSpeed*deltaTime,0.f));
		GetMesh()->AddRelativeRotation(FRotator(0.f, moveSpeed * deltaTime, 0.f));
	}
}
