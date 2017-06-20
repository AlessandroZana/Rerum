// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PlatformMoviment.h"
#include "Components/ActorComponent.h"
#include "PlatformMoviment.h"
#include "DoorComponent.generated.h"

class camera;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenDoor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RERUM_API UDoorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void DoorMoviment(AActor * value);


	UPROPERTY(EditAnywhere)
	AActor* Telaio;//attore contenente il telaio della porta (attualmente non più usato)

	UPROPERTY(EditAnywhere)
	AActor* Botton1;//attore che contiene il primo pulsante da premere per apire la porta

	UPROPERTY(EditAnywhere)
	AActor* Botton2;//attore che contiene il secondo pulsante da premere per apire la porta

	UPROPERTY(EditAnywhere)
	AActor* Botton3;//attore che contiene il terzo pulsante da premere per apire la porta

	UPROPERTY(EditAnywhere)
	bool IsSpecialDoor;//booleano che gestisce se stiamo lavorando su una porta normale oppure ad apertura doppia

	UPROPERTY(EditAnywhere)
	AActor* LeftDoor;//nel caso di special door contiene l'anta di sinistra

	UPROPERTY(EditAnywhere)
	AActor* RightDoor;//nel caso di special door contiene l'anta di destra
	
	UPROPERTY(EditAnywhere)
	float destroyDoor = 4.f;//Tempo di delay per distruzione porta
	
	UPROPERTY(EditAnywhere)
	float DelayPlatform = 20;//tempo dopo il quale le piattaforme non sono più premute

	UPROPERTY(BluePrintAssignable)
	FOpenDoor Open;//Evento che apre la porta

	UFUNCTION(BluePrintCallable, Category = Default)
	void OpenWithLeva();

private:
	int Count = 0;//contiene il numero di pulsanti da premere che varia da 1 a 3
	int Pressed = 0;//contiene il numero di pulsanti premuti rispettando l'ordine stabilito

	float delay2 = 0.8f;//Tempo di delay per distruzione porta
	float time ;
	float time2;

	bool CanBeDestroy = false;//Variabile che stabilisce l'apertura della porta e di conseguenza la distruzione delle ante

	void DelayDestroy();//Funzione che distrugge le ante

	AActor* Number1 = nullptr;
	AActor* Number2 = nullptr;
	AActor* Number3 = nullptr;

};
