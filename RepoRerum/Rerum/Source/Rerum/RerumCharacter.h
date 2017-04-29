// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "PaperCharacter.h"
#include "RerumCharacter.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FColorEvent);

// This class is the default character for Rerum, and it is responsible for all
// physical interaction between the player and the world.
//
//   The capsule component (inherited from ACharacter) handles collision with the world
//   The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
//   The Sprite component (inherited from APaperCharacter) handles the visuals
class UTextRenderComponent;
class UOrbis;

UCLASS(config=Game)
class ARerumCharacter : public APaperCharacter
{
	GENERATED_BODY()
	/////////////////////////////////////////////////////////////////////////////////////	

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	
	FVector getCapsulePosition;
	FVector setcapsulePosition;
	float capsuleUp;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
protected:

	//Animazioni orbis leggero
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunningAnimationLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimationLight;
	
	//Animazioni orbis pesante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunningAnimationHeavy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimationHeavy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* trasformationToSenior;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* trasformationToJunior;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
	
	//Puntatore alla classe Orbis
	UOrbis* orbis = nullptr;
public:
	ARerumCharacter();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void CastToOrbis(UOrbis* orbisComponent);
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

						
