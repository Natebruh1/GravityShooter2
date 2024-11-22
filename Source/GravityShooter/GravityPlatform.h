// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravityPlatform.generated.h"

UCLASS()
class GRAVITYSHOOTER_API AGravityPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravityPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void updateGravity(AActor* target, bool updateLocal=true);
	FVector getGravity();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* gravObjMesh;
private:
	FVector gravityDirection;
};
