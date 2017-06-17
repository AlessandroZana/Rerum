// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Rerum.h"
#include "RerumCharacter.h"
#include "Orbis.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);
//////////////////////////////////////////////////////////////////////////
// ARerumCharacter

ARerumCharacter::ARerumCharacter()
{

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

// 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
// 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
// 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
// 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
// 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
}

void ARerumCharacter::CastToOrbis(UOrbis* orbisComponent)
{
	orbis = orbisComponent;
	if (orbis)
	{
		UE_LOG(LogTemp,Warning,TEXT("Orbis found"))
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Orbis not found"))
	}
}
//////////////////////////////////////////////////////////////////////////
// Animation

void ARerumCharacter::UpdateAnimation()
{
	if (CanChangeAnimation == false) { return; }
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
	float zVelocity = GetVelocity().Z;
	
	if (orbis->playerState == 2)
	{
		if (orbis->transform == true)
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(150.0);
			GetCapsuleComponent()->SetCapsuleRadius(40.0f);
			//UE_LOG(LogTemp, Warning, TEXT("Transform"));
			orbis->DisableInput();
			GetSprite()->SetFlipbook(transformToLight);
			if ((FPlatformTime::Seconds() - beginTransform) > transformDelay)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Not transform"));
				GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
				GetCapsuleComponent()->SetCapsuleRadius(40.0f);
				beginTransform = FPlatformTime::Seconds();
				orbis->EnableInput();
				orbis->transform = false;
			}
		}
		
		else if (GetCharacterMovement()->IsFalling())
		{
			if (zVelocity > 0.f)
			{
				if (orbis->state == EState::NotFlying)
				{
					GetSprite()->SetFlipbook(jumpLightUp);
				}
				else if (orbis->state == EState::Flying)
				{
					GetSprite()->SetFlipbook(jetpackLight);
				}

			}
			else if (zVelocity < 0.f)
			{
				GetSprite()->SetFlipbook(jumpLightDown);
				if (orbis->state == EState::FlyingDash && orbis->lightFuel > 0.f)
				{
					GetSprite()->SetFlipbook(airDashLight);
				}
			}
			

		}
		else if (PlayerSpeedSqr > 0.0f)
		{
			GetSprite()->SetFlipbook(walkLight);
		}
		else
		{
			GetSprite()->SetFlipbook(idleLight);
		}
	}
	else if(orbis->playerState == 1)
	{
	
		if (orbis->checkCapsuleCollision)
		{
			getCapsulePosition = GetCapsuleComponent()->GetComponentLocation();
			setcapsulePosition = FVector(getCapsulePosition.X, getCapsulePosition.Y, getCapsulePosition.Z + capsuleUp);
			GetCapsuleComponent()->SetRelativeLocation(setcapsulePosition);
			orbis->checkCapsuleCollision = false;
		}

		GetCapsuleComponent()->SetCapsuleHalfHeight(200.0);
		GetCapsuleComponent()->SetCapsuleRadius(40.0f);

		if (orbis->transform == true)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Transform"));
			orbis->DisableInput();
			GetSprite()->SetFlipbook(transformToHeavy);
			if ((FPlatformTime::Seconds() - beginTransform) > transformDelay)
			{
				GetCapsuleComponent()->SetCapsuleHalfHeight(200.0);
				GetCapsuleComponent()->SetCapsuleRadius(40.0f);
				//UE_LOG(LogTemp, Warning, TEXT("Not transform"));
				orbis->EnableInput();
				beginTransform = FPlatformTime::Seconds();
				orbis->transform = false;

			}
		}
		
		else if (orbis->OnDash == true && orbis->AlreadyJump == false && orbis->heavyFuel > 0.f)
		{
			GetSprite()->SetFlipbook(dashHeavy);
		}

		else if (GetCharacterMovement()->IsFalling())
		{
			if (zVelocity > 0.f)
			{
				if (orbis->state == EState::NotFlying)
				{
					GetSprite()->SetFlipbook(jumpHeavyUp);
				}
				else if (orbis->state == EState::Flying)
				{
					GetSprite()->SetFlipbook(jetpackHeavy);
				}
			}
			else if (zVelocity < 0.f)
			{
				if (orbis->state != EState::FlyingDash)
				{
					GetSprite()->SetFlipbook(jumpHeavyDown);
				}
				if (orbis->StopFalling() && orbis->state == EState::FlyingDash && orbis->heavyFuel > 0.f)
				{
					GetSprite()->SetFlipbook(smashHeavy);
				}
			}
		}

		else if (PlayerSpeedSqr > 0.0f)
		{
			GetSprite()->SetFlipbook(walkHeavy);
		}

		else
		{
			GetSprite()->SetFlipbook(idleHeavy);
		}

	}
}

void ARerumCharacter::BeginPlay()
{
	Super::BeginPlay();
	capsuleUp = 100.0;
	beginTransform = FPlatformTime::Seconds();
}
//Tick
void ARerumCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCharacter();
	if (orbis->state == EState::FlyingDash && CanChangeAnimation == true)
	{
		CanChangeAnimation = false;
		LastTimeCanChange= GetWorld()->GetTimeSeconds();
	}
	if (CanChangeAnimation == false)
	{
		if ((LastTimeCanChange + TimeBeforeChange) < GetWorld()->GetTimeSeconds())
		{
			CanChangeAnimation = true;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ARerumCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARerumCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARerumCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARerumCharacter::TouchStopped);
}

void ARerumCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ARerumCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	
	Jump();
}

void ARerumCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ARerumCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}