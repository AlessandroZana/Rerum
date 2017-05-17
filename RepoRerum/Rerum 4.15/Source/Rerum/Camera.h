// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Orbis.h"
#include "Components/ActorComponent.h"
#include "Camera.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RERUM_API UCamera : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCamera();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	UPROPERTY(EditAnywhere)
	ACharacter* player;//riferiemnto al player

	void MoveCamera();//serve per fare il moviemtno della camera
	void CameraZoomIn();//funzione che gestisce lo zoom della camera
	void CameraZoomOut();//funzione che gestisce lo zoom della camera

	UPROPERTY(EditAnywhere)
	AActor * NuovaPosizione;//riferimento del punto di zoom una volta sul trigger

	UPROPERTY(EditAnywhere)

	float MaxCameraDistance;//distanza massima della camera quando si effettua lo spostamento per trigger

	UPROPERTY(EditAnywhere)
		float MaxCameraZoomOut=500;//distanza massima dello zoom quando si usa il jetpack

	UPROPERTY(EditAnywhere)
		float LowZPosition=430;//posizione asse z minimo della telecamera

	UPROPERTY(EditAnywhere)
		float VelocitaDizoomOutTelecamera = 15;//Velocità dello spostamento/zoom out telecamera

private:

	bool CantBlockit = false;//booleano che gestisce il controllo dello zoom 
	bool IsUsingJetpack = false;//booleano che informa se il player usa il jetpack o no
	bool IsMoving = false;//booleano che verifica se la camera si è mossa dal player
	float Time;//float per il momento in cui si muove
	float ComeBack = 2;//tempo prima che la camera ritorni sul player
	float CameraY;//posizion asse y della camera all'inizio
	float positionY;//posizione asse y della camera durante il jetpack
};
