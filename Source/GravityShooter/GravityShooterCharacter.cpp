// Copyright Epic Games, Inc. All Rights Reserved.

#include "GravityShooterCharacter.h"
#include "GravityShooterProjectile.h"
#include "Animation/AnimInstance.h"
#include "GravityPlatform.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGravityShooterCharacter

AGravityShooterCharacter::AGravityShooterCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

FVector AGravityShooterCharacter::getGravity()
{
	return gravityDirection;
}

void AGravityShooterCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	if (totalKickBack > 0.f)
	{
		FirstPersonCameraComponent->AddRelativeRotation(FRotator(totalKickBack, 0.f, 0.f));
		totalKickBack = totalKickBack / (1.5+deltaTime);
		totalKickBack -= deltaTime;

	}
	else
	{
		totalKickBack = 0.f;
	}
	if (currentHealth <= 0.f)
	{
		SetActorLocation(GetActorLocation() + FVector(0.f, 10.f, 0.f));
		GetCapsuleComponent()->SetSimulatePhysics(true);
		GetWorld()->GetTimerManager().SetTimer(actorDestroyDelay, this, &AGravityShooterCharacter::killThis, 7.f, false);
		
	}
	
}

void AGravityShooterCharacter::ClampPitch(FRotator& CRotation)
{
	CRotation.Pitch = FMath::Clamp(CRotation.Pitch, -89.f, 89.f);
}

void AGravityShooterCharacter::updateGravityFromProp()
{
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
	
	

	GetCharacterMovement()->SetGravityDirection(gravityDirection);

	//GetCharacterMovement()->SetGravityDirection(FVector(1,0,0));
	
	FVector regRot = -FVector::UpVector;

	FRotator GravityAlignedRot = FRotationMatrix::MakeFromZ(-gravityDirection).Rotator();


	SetActorRotation(GravityAlignedRot);
	Controller->SetControlRotation(GravityAlignedRot);
	FirstPersonCameraComponent->SetWorldRotation(GravityAlignedRot);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AGravityShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	currentGun = &(guns[0]);
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void AGravityShooterCharacter::switchGun()
{
	gunCount++;
	currentGun = &(guns[gunCount % 3]);
}

//////////////////////////////////////////////////////////////////////////// Input

void AGravityShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGravityShooterCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGravityShooterCharacter::Look);
		
		//Changing Gravity
		EnhancedInputComponent->BindAction(ChangeGravityAction, ETriggerEvent::Started, this, &AGravityShooterCharacter::ChangeGravity);

		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AGravityShooterCharacter::ShootGun);

		//Switching Gun
		EnhancedInputComponent->BindAction(SwitchGunAction, ETriggerEvent::Triggered, this, &AGravityShooterCharacter::switchGun);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGravityShooterCharacter::killThis()
{
	Destroy();
}

void AGravityShooterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		
		// add movement 
		AddMovementInput(FirstPersonCameraComponent->GetForwardVector(), MovementVector.Y);
		AddMovementInput(FirstPersonCameraComponent->GetRightVector(), MovementVector.X);
	}
}

void AGravityShooterCharacter::ShootGun()
{
	FTimerManagerTimerParameters a = { false,true,-1.f };
	//UE_LOG(LogTemp, Warning, TEXT("Attempting To Shoot"));
	if (GetWorld()->GetTimerManager().IsTimerActive(bulletDelayHandle))
	{

	}
	else
	{
		//Shoot Bullet
		for (UINT i = 0; i < currentGun->bulletCount; i++)
		{
			FVector3d start = GetActorLocation();
			auto rot = FirstPersonCameraComponent->GetComponentRotation() +FRotator(FMath::RandRange(-currentGun->spread.Y, currentGun->spread.Y),FMath::RandRange(-currentGun->spread.X, currentGun->spread.X),0.f);
			start += 160.f * rot.Vector();//
			FVector3d end = start + currentGun->bulletRange * rot.Vector();
			FHitResult hit;
			GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_GameTraceChannel1);
			if (hit.GetActor())

			{
				UE_LOG(LogTemp, Warning, TEXT("Shot an Actor"));


				if (Cast<AGravityShooterCharacter>(hit.GetActor()))
				{
					Cast<AGravityShooterCharacter>(hit.GetActor())->currentHealth -= FMath::RandRange(currentGun->minDamage, currentGun->maxDamage);
				}
				DrawDebugLine(GetWorld(), start, hit.Location, currentGun->gunColor, false, 0.05f, 0, 10.f);
			}
			else
			{
				GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Camera);


				DrawDebugLine(GetWorld(), start, start + currentGun->bulletRange * rot.Vector(), currentGun->gunColor, false, 0.05f, 0, 10.f);
			}
		}
		


		
		totalKickBack += currentGun->kickback;
		//Set Timer
		GetWorld()->GetTimerManager().SetTimer(bulletDelayHandle, currentGun->reloadSpeed, a);
	}
}

void AGravityShooterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		
		///*FRotationMatrix GravityRotationMatrix = FRotationMatrix(gravityUp.Rotation());
		//FVector RightVector = GravityRotationMatrix.GetScaledAxis(EAxis::Y);
		//FVector ForwardVector = GravityRotationMatrix.GetScaledAxis(EAxis::X);*/
		FVector gravityUp = -gravityDirection;
		FVector worldForward = FVector::ForwardVector; //Doesn't change
		FVector RightVector = FVector::CrossProduct(gravityUp, worldForward).GetSafeNormal(); //Get Model Right
		FVector ForwardVector = FVector::CrossProduct(RightVector, gravityUp).GetSafeNormal(); //Get model forward


		//FRotator CameraRotation = GetViewRotation();
		//CameraRotation = FirstPersonCameraComponent->GetComponentRotation();

		//float yawChange = LookAxisVector.X * (1.f - FMath::Abs(gravityDirection.X));
		//float pitchChange = LookAxisVector.Y;
		//float rollChange = LookAxisVector.X * (1.f - FMath::Abs(gravityDirection.Z));

		//FQuat yawQuat = FQuat(gravityUp, FMath::DegreesToRadians(yawChange)); //FQuat constructor takes radians whereas camera rotation is stored in degrees
		//FQuat pitchQuat = FQuat(RightVector, FMath::DegreesToRadians(pitchChange));
		//FQuat rollQuat = FQuat(ForwardVector, FMath::DegreesToRadians(rollChange));


		//FQuat combinedRotation = rollQuat * pitchQuat * yawQuat * CameraRotation.Quaternion();

		//FirstPersonCameraComponent->SetWorldRotation(combinedRotation);


		//UE_LOG(LogTemp, Warning, TEXT("XAngle=%f"), combinedRotation.Euler().X);


		//FQuat currentRotQuat = CameraRotation.Quaternion();

		//FQuat RollAdjust=FQuat(gravityUp,FMath::DegreesToRadians(Controller->RollIn))

		
		//UE_LOG(LogTemp, Warning, TEXT("XRatio=%f"), 1.f - gravityDirection.X);
		//UE_LOG(LogTemp, Warning, TEXT("ZRatio=%f"), 1.f - FMath::Abs(gravityDirection.Z));
		 
		float pitchMantissa = FMath::Sign(FirstPersonCameraComponent->GetComponentRotation().Pitch);
		//pitchMantissa = FMath::Sin(pitchMantissa);


		FRotator controlRotation = Controller->GetControlRotation();
		controlRotation.Pitch = FMath::Clamp(controlRotation.Pitch, -89.f, 89.f);
		FQuat CurrentQuat = FQuat(controlRotation);
		FVector currentUp = GetActorUpVector();
		FVector currentRight = GetActorRightVector();


		FTransform CameraTransform(controlRotation);

		//add yaw and pitch input to controller
		float yawDelta = LookAxisVector.X * (1.f - FMath::Abs(gravityDirection.X));
		float pitchDelta = LookAxisVector.Y;
		float rollDelta = LookAxisVector.X * (1.f - FMath::Abs(gravityDirection.Z));

		FRotator rawRot = FRotator(-LookAxisVector.Y, LookAxisVector.X, 0.f);
		

		FQuat pitchQuat = FQuat(currentRight, FMath::DegreesToRadians(LookAxisVector.Y));
		FQuat yawQuat = FQuat(currentUp, FMath::DegreesToRadians(LookAxisVector.X));
		UE_LOG(LogTemp, Warning, TEXT("Yaw Quat %s"), *(yawQuat.ToString()))

		if (FirstPersonCameraComponent->GetRelativeRotation().Pitch + rawRot.Pitch > 89.9f || FirstPersonCameraComponent->GetRelativeRotation().Pitch + rawRot.Pitch < -89.9f)
		{
			rawRot.Pitch = 0.f;
		}

		FQuat NewQuat = yawQuat * CurrentQuat * pitchQuat;
		
		ClampPitch(rawRot);
		float prevRoll=FirstPersonCameraComponent->GetComponentRotation().Roll;

		FirstPersonCameraComponent->AddRelativeRotation(rawRot);
		

		//Controller->SetControlRotation(newRot);
		//FirstPersonCameraComponent->g
		/*FRotator DeltaRot(pitchDelta, yawDelta, rollDelta);
		FQuat DeltaQuat = FQuat(DeltaRot);
		FQuat NewQuat = CurrentQuat * DeltaQuat;*/

		/*FQuat deltaQuat = FQuat::MakeFromEuler(FVector(pitchDelta, yawDelta, rollDelta));
		FQuat newQuat = CameraTransform.GetRotation() * deltaQuat;

		FRotator newRot = newQuat.Rotator();*/
		//Controller->SetControlRotation(NewQuat.Rotator());


		//AddControllerYawInput(yawDelta);
		////AddControllerYawInput(LookAxisVector.X);

		//AddControllerPitchInput(pitchDelta);
		//AddControllerRollInput(rollDelta);
		
		
		

		UE_LOG(LogTemp, Warning, TEXT("ZRot=%f"), LookAxisVector.X * (1.f - FMath::Abs(gravityDirection.Z)));
		UE_LOG(LogTemp, Warning, TEXT("XRot=%f"), LookAxisVector.X * (1.f - FMath::Abs(gravityDirection.X)));
		UE_LOG(LogTemp, Warning, TEXT("Pitch Mantissa=%f"), pitchMantissa);
	}
}

void AGravityShooterCharacter::ChangeGravity()
{
	UE_LOG(LogTemp, Warning, TEXT("Changing Gravity"));
	
	FVector3d start = GetActorLocation();
	/*auto  rotProp=FirstPersonCameraComponent->GetAbsoluteRotationPropertyName();
	FProperty* prp = FirstPersonCameraComponent->GetClass()->FindPropertyByName(rotProp);
	prp->ContainerPtrToValuePtr<FRotator3d>(prp);*/



	
	FRotator3d rot = GetRelativeRotationFromGravity(FirstPersonCameraComponent->GetComponentRotation(),gravityDirection);
	rot = FirstPersonCameraComponent->GetComponentRotation();
	FVector3d end = start + 12000.f * rot.Vector();
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility);
	if (hit.GetActor())

	{
		UE_LOG(LogTemp, Warning, TEXT("Traced an Actor"));
		UE_LOG(LogTemp, Warning, TEXT("Actor Location is : %s"), *(hit.GetActor()->GetActorLocation().ToString()));
		DrawDebugLine(GetWorld(), start, hit.Location, FColor(255, 0, 0), false, 3.f, 0, 10.f);


		auto gPlat = Cast<AGravityPlatform>(hit.GetActor()); //Gravity Platform
		if (gPlat!=nullptr)
		{
			bUseControllerRotationPitch = true;
			bUseControllerRotationYaw = true;
			bUseControllerRotationRoll = true;
			UE_LOG(LogTemp, Warning, TEXT("Actor Forward is : %s before"), *(hit.GetActor()->GetActorForwardVector().ToString()));
			gPlat->updateGravity(this);

			GetCharacterMovement()->SetGravityDirection(gPlat->getGravity());
			
			//GetCharacterMovement()->SetGravityDirection(FVector(1,0,0));
			gravityDirection = gPlat->getGravity();
			FVector regRot = -FVector::UpVector;
			
			FRotator GravityAlignedRot = FRotationMatrix::MakeFromZ(-gravityDirection).Rotator();


			SetActorRotation(GravityAlignedRot);
			Controller->SetControlRotation(GravityAlignedRot);
			FirstPersonCameraComponent->SetWorldRotation(GravityAlignedRot);
			bUseControllerRotationPitch = false;
			bUseControllerRotationYaw = false;
			bUseControllerRotationRoll = false;
			//SetActorRotation(GetRelativeRotationFromGravity(GetActorRotation(), gravityDirection));
			//rstPersonCameraComponent->SetWorldRotation(gravityDirection.Rotation());
			//FirstPersonCameraComponent->SetWorldRotation(0.f,0.f,0.f);
			

			UE_LOG(LogTemp, Warning, TEXT("Actor Forward is : %s After"), *(hit.GetActor()->GetActorForwardVector().ToString()));

			//Controller->ClientSetRotation(FVector(-gravityDirection).Rotation());

			FVector worldForward = FVector::ForwardVector; //Doesn't change
			FVector RightVector = FVector::CrossProduct(-gravityDirection, worldForward).GetSafeNormal(); //Get Model Right
			FVector ForwardVector = FVector::CrossProduct(RightVector, -gravityDirection).GetSafeNormal(); //Get model forward
			//Controller->ClientSetRotation(ForwardVector.Rotation());
		}
		
		
	}
}

FRotator AGravityShooterCharacter::GetRelativeRotationFromGravity(FRotator Rotation, FVector GravityDirection)
{
	if (!GravityDirection.Equals(FVector::DownVector))
	{
		FQuat GravityRotation = FQuat::FindBetweenNormals(GravityDirection, FVector::DownVector);
		return (GravityRotation * Rotation.Quaternion()).Rotator();
	}

	return Rotation;
}

void AGravityShooterCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AGravityShooterCharacter::GetHasRifle()
{
	return bHasRifle;
}