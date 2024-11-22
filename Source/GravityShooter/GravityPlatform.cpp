// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityPlatform.h"

// Sets default values
AGravityPlatform::AGravityPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	gravObjMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gravity Object Mesh"));
	//gravObjMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AGravityPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravityPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGravityPlatform::updateGravity(AActor* target, bool updateLocal)
{
	FVector locVec = target->GetActorLocation() - GetActorLocation(); //From this object to the target
	locVec.Normalize();



	if (updateLocal)
	{
		gravityDirection = locVec.Dot(GetActorForwardVector()) > 0.f ? -GetActorForwardVector() : GetActorForwardVector();
	}

	//WIP Need to derive character from gravity character and set local gravity
}

FVector AGravityPlatform::getGravity()
{
	return gravityDirection;
}

