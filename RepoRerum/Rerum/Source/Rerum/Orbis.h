// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Camera.h"
#include "PlatformMoviment.h"
#include "RerumCharacter.h"
#include "Components/ActorComponent.h"
#include "Orbis.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEvents);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RERUM_API UOrbis : public UActorComponent
{
	GENERATED_BODY()
	
	void SetUpInputComponent();

	void Fly();//Funzione richiamata dall evento pressed
	void NotFly();//Funzione richiamata dall evento released
	void CharacterOnAir();//Funzione per far volare il character
	void IsOnAir();//Funzione che serve a definire se il player ha saltato
	float RemoveFuel(float fuel);//Funzione che rimuove il fuel
	float MoreFuel(float fuel, float fuelMax);//Funzione che aggiunge il fuel
	
	void Dash();//Funzione richiamata dall evento pressed shift
	void NotDash();//Funzione richiamata dall evento released shift
	void CharacterOnDash();//Funzione per far dashare/correre il character
	bool StopFalling();//funzione che distrugge oggetti e blocca il char durante caduta
	FVector EndLineStopRun();//Funzione per fermare il player

	void ChangeHeavyLight();//Funzione che cambia lo stato del player da heavy a light
	
	void CharacterHitTrigger();//funzione che gestisce il contatto con il trigger
	
	FVector StartLine();//Funzione che prende la location dell' actor
	FVector EndLine();//Prende la fine del vettore costruito verso il basso per controllare la ricarica fuel
	FHitResult GetRechargePlatform();//Funzione che controlla se si sta su una piattaforma di ricarica
	bool StopRunCharacter();//funziona che distrugge oggetti e blocca il char durante la corsa
	bool HeavyLightPlatform();//funzione che analizza le piattaforme sulle quali il player si trova

	enum playerChangeState { HEAVY = 1, LIGHT };//Cambia lo stato da leggero a pesante
	playerChangeState playerState;//Variabile che tiene conto dello state di orbis
	
	bool OnAir;//Booleano che controlla se si sta tenendo premuto o rilasciando il tasto
	bool OnDash;//Booleano che controlla se il player ha iniziato il dash/corsa
	bool AlreadyJump;//booleano che controlla se il player ha già saltato
	
	FVector actorLocation;//Prende la location di orbis
	FVector lineDirection;//Vettore sull' asse Z , serve a prendere il vector per la piattaform di ricarica
	FVector lineEndDirection;//Vettore che controlla sotto ai piedi di Orbis(piattaforma di ricarica)
	FRotator PlayerRotation;//contiene la rotazione del player in rotator
	FVector PlayerDirection;//contiene la rotazione del player in vector
	FVector lineDirectionStopRun;//serve per fermare il player mentre corre
	FVector lineEndDirectionStopRun;//serve per fermare il player mentre corre
	float range;//Range per allungare il vettore del line trace
	
	float RealTimeDash;//prende il tempo in cui viene usatto il dash	
	float RealTimeRun;//Prende il tempo in cui viene usata la corsa
	float RealTimeJump;//Prende il tempo in cui viene usato il jump

	bool DelayOnRun = true;//booelano che determina il delay della corsa del player
	bool CanDoIt = false;//booleano di supporto per il delay della corsa del playr
	bool AlreadyTriggered = false;//Non permette la ripetizione del trigger 
	
	ACharacter* player;
	UInputComponent* InputComponent = nullptr;
	UPlatformMoviment* PlatformPressed;

public:	
	// Sets default values for this component's properties
	UOrbis();

	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	void EnableInput();//funzione che abilita gli input
	void DisableInput();//funzione che disabilita gli input
	
	//Variabili per i game designer
	//Forza del jetpack
	UPROPERTY(EditAnywhere)
	FVector JetpackForceL;//jetpack leggero
	UPROPERTY(EditAnywhere)
	FVector JetpackForceH;//jetpack pesante

	UPROPERTY(EditAnywhere)
	FVector DashForce;//Forza del dash

	UPROPERTY(EditAnywhere)
	float MaxTimeRun = 2;//timing di delay corsa
	UPROPERTY(EditAnywhere)
	float DelayTimeJump = 1;//Timing di delay salto
	UPROPERTY(EditAnywhere)
	float DelayTimeDash = 1;//Timing di delay dash

	//Ho messo queste variabili in modo che quando si ricaricano i fuel non possano superare questa soglia
	//Fuel della forma pesante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float heavyFuelMax;//Quantità massima di fuel in forma pesante
    //Fuel della forma leggera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float lightFuelMax;//Quantità massima di fuel in forma leggera

	UPROPERTY(EditAnywhere)
	float FallingForce;//Gestisce la forza in caduta
	
	UPROPERTY(BluePrintAssignable)
	FEvents Red;//Evento cambio in rosso
	UPROPERTY(BluePrintAssignable)
	FEvents Blue;//Evento cambio in blu
	UPROPERTY(BluePrintAssignable)
	FEvents Disable;//evento disabilita input
	UPROPERTY(BluePrintAssignable)
	FEvents Enable;//evento abilita input
	
	UPROPERTY(EditAnywhere)
	AActor* Camera;//Variabile che fa riferimento alla nostra nuova camera

	bool CanJumpIsUp = false;//booleano di supporto per il salto del player
							 //Stanno in Public perchè li uso da blueprint per stamparli a schermo
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	float heavyFuel;//Controlla la quantità di fuel in forma pesante
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	float lightFuel;//Controlla la quantità di fuel in forma leggera

	UPROPERTY(BlueprintReadWrite, Category = Default)
	bool DelayOnDash = false;//boleano che determina il delay del dash del player
	UPROPERTY(BlueprintReadWrite, Category = Default)
	bool DelayOnJump = false;//boleano che determina il delay del jump del player


	bool IsOnDash = false;
	float MaxTimeDash;

};
