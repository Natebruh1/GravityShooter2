// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GravityShooterCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

struct gun
{
	UINT bulletCount = 1;
	FVector2D spread = FVector2D(0.f, 0.f);
	float reloadSpeed = 0.3f;
	float kickback = 0.1f;
	float minDamage = 15.f;
	float maxDamage = 15.f;
	float bulletRange = 12000.f;
	FColor gunColor = FColor(0, 180, 255);
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGravityShooterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	AGravityShooterCharacter();
	FVector getGravity();
	virtual void Tick(float deltaTime) override;
	void ClampPitch(FRotator& CRotation);
	void updateGravityFromProp();
protected:
	virtual void BeginPlay();
	float totalKickBack = 0.f;
	
public:
	//Guns
	gun guns[3] = { { 1,FVector2D(1.f,1.f),0.3f,0.7f,8.f,10.f,12000.f }, { 12,FVector2D(16.f,16.f),1.5f,5.f,6.f,8.f,450.f },1,FVector2D(0.1f,0.1f),5.f,12.f,100.f,100.f,12000.f,FColor(255,128,0)};
	int gunCount = 0;
	gun* currentGun;
	void switchGun();
	//Gameplay
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float currentHealth = 100.f;
public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ChangeGravityAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ShootAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SwitchGunAction;
	FTimerHandle bulletDelayHandle;
	FTimerHandle actorDestroyDelay;
	void killThis();
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void ShootGun();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void ChangeGravity();
	FRotator GetRelativeRotationFromGravity(FRotator Rotation, FVector GravityDirection);
	UPROPERTY(VisibleAnywhere)
	FVector gravityDirection=FVector::DownVector;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

