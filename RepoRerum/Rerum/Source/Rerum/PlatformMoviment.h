// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DoorComponent.h"
#include "Components/ActorComponent.h"
#include "PlatformMoviment.generated.h"

class UPlatformLight;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RERUM_API UPlatformMoviment : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlatformMoviment();

	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	void HandlePressure();//funzione chiamata dal player che serve a passare alla porta il tasto premuto
	
	UFUNCTION(BlueprintCallable, Category = "Light")
	void PlatformLightInit(UPlatformLight* platformInit);
	
	UPROPERTY(EditAnywhere)
	AActor* Door;//porta alla quale la piattaforma è associata

	void LightOn();
	void LightOff();

	
	//void OpenDoor();
	//float time;
	//float delay;
private:
	bool CanMove = false;//booleano per lo spostamento delle piattaforme(al momento non più utilizzato)
	bool Moviment = false;//secondo booleano per lo spostamento delle piattaforme (al momento non più utlizzato)
	UPlatformLight* platformLight = nullptr;
};
