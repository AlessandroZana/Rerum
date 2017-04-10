// Fill out your copyright notice in the Description page of Project Settings.

#include "Rerum.h"
#include "Orbis.h"
#include "DrawDebugHelpers.h"



// Sets default values for this component's properties
UOrbis::UOrbis()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
void UOrbis::SetJetpack()
{
	jetpack = GetOwner()->FindComponentByClass<UPhysicsThrusterComponent>();
	if (jetpack) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Jetpack Found"));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Jetpack not found"));
	}
}

// Called when the game starts
void UOrbis::BeginPlay()
{
	Super::BeginPlay();
	SetJetpack();
	//inizializzazione delle variabili per i delay dell salto e azione
	RealTimeJump = GetWorld()->GetTimeSeconds();
	RealTimeDash = GetWorld()->GetTimeSeconds();
	RealTimeRun = GetWorld()->GetTimeSeconds();

	//Inizializzo le var dello scatto e del volo a false
	OnDash = false;
	OnAir = false;

	//Inizializzo le var  dei fuel al loro massimo
	lightFuel = lightFuelMax;
	heavyFuel = heavyFuelMax;
	//Orbis inizia in forma "leggera"
	playerState = LIGHT;
	//Vettore sull asse z utilizzato nel linetrace per tenere conto della piattaforma di ricarica 
	lineDirection = FVector(0.f, 0.f, 1.f);
	//Lunghezza del linetrace
	range = 250;
	//Cast al character
	player = Cast<ARerumCharacter>(GetOwner());
	//Binding dei tasti
	SetUpInputComponent();

	bool checkCapsuleLocation = false;
	JetpackForceLxTime = JetpackForceLFirstHit;
	MinHeavy = heavyFuelMax / 10;
	MinLight = lightFuelMax / 10;
}  


// Called every frame
void UOrbis::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlayerRotation = player->GetActorRotation();//presa la rotazione dell player per poter avere il lane girato sempre nel verso del player
	PlayerDirection = PlayerRotation.Vector();//Settata un Fvector che contiene la rotazione
	lineDirectionStopRun = FVector(-PlayerDirection.X, 0.f, 0.f);//Impostato il linedirection con la rotazione corrente del player
	//funzione che gestisce il delay sulla carica incontrollata
	if (CanDoIt)
	{
		if (RealTimeRun + MaxTimeRun < GetWorld()->GetTimeSeconds())
		{
			DelayOnRun = false;//se il booleano è true ed il tempo trascorso è maggiore del delay si setta a false questa variabile che blocca la corsa
		}
	}
	//funzione che gestisce il delay del salto del player grande
	if (RealTimeJump + DelayTimeJump < GetWorld()->GetTimeSeconds())
	{
		DelayOnJump = false;//se il tempo trascorso è maggiore del delay si setta a false in modo da permettere di saltare nuovamente
	}
	//fuznione che gestisce il delay del dash del player forma leggera
	if (RealTimeDash + DelayTimeDash < GetWorld()->GetTimeSeconds())
	{
		DelayOnDash = false;//se il tempo trascorso è maggiore del delay si setta a false in modo da permettere di dashare nuovamente
	}
	CharacterHitTrigger();
	switch (playerState)//Switch che controlla se entrambe le forme possono usare il jetpack
	{
	//Orbis Leggero
	case LIGHT:
		//Temporaneo,lo fa diventare blu
		Blue.Broadcast();
		if (lightFuel >0)//Controllo quantita fuel leggero
		{
			CharacterOnAir();//Se sopra lo 0 posso volare e fare il dash
			CharacterOnDash();
		}
		break;
	//Orbis pesante
	case HEAVY:
		//Temporaneo,lo fa diventare rosso
		Red.Broadcast();
		if (heavyFuel > 0)//Controllo quantita fuel pesante
		{
			CharacterOnAir();//Se sopra lo 0 posso volare e fare il dash
			CharacterOnDash();
		}
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("State not found"));
		break;
	}
	//Ottiene il risultato dell funzione per controllare se Orbis sta sopra una piattaforma
	auto HitResult = GetRechargePlatform();
	//Rende l' oggetto colpito dalla var sopra in un actor 
	auto ActorHit = HitResult.GetActor();
	//controllo per verificare che si impatti su una piattaforma , stampa solo se si impatta ma rispettando i criteri piattaforma/player
	if (HeavyLightPlatform())
	{
		
		//UE_LOG(LogTemp, Warning, TEXT("Pulsante Premuto"));
	}

	if (ActorHit)//Se l' oggetto è la piattaforma  ricarica il fuel, se minore della quantità max
	{
		if (ActorHit->ActorHasTag("Triangolo"))
		{
			heavyFuel = MoreFuel(heavyFuel, heavyFuelMax);//Ricarico i fuel attraverso la funzione
		}
		else 
			if (ActorHit->ActorHasTag("Cerchio"))
			{
				lightFuel = MoreFuel(lightFuel, lightFuelMax);//Ricarico i fuel attraverso la funzione
			}else
			{
				//nel caso si lascia il vecchio recupero 
				heavyFuel = MoreFuel(heavyFuel, heavyFuelMax);//Ricarico i fuel attraverso la funzione
				lightFuel = MoreFuel(lightFuel, lightFuelMax);//Ricarico i fuel attraverso la funzione
			}
		
		
		
	}

	/*//stampa a video della linea di impatto verso il basso
	DrawDebugLine(GetWorld(),
		StartLine(),//punto di partenza della linea
		EndLine(),//punto di fine della linea
		FColor(255, 0, 0),//rosso
		false,
		0.f,
		0,
		10.f);
	//stampa a video della linea di impatto avanti costantemente aggiornata
	DrawDebugLine(GetWorld(),
		StartLine(),//punto di partenza della linea
		EndLineStopRun(),//punto di fine della linea
		FColor(255, 0, 0),//rosso
		false,
		0.f,
		0,
		10.f);
		*/
	if (CanJumpIsUp)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Può usare jetpack"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Non Può usare jetpack"));
	}
	if (!player->GetCharacterMovement()->IsFalling())//se il player non sta cadendo settiamo il can jump a false
	{
		CanJumpIsUp = false;//booleano che gestisce se il player è in volo o no 
		Camera->FindComponentByClass<UCamera>()->CameraZoomIn();//chiamata della funzione zoomIn per far tornare la telecamere in modo normale
		AlreadyJump = false;
	}
	if (player->GetCharacterMovement()->IsFalling())//se il player sta cadendo settiamo il can jump a true
	{
		CanJumpIsUp = true;//booleano che gestisce se il player è in volo o no 
		if (CanDoIt == false)//Candoit è il booleano che si occupa di gestire la corsa
		{
			AlreadyJump = true;
		}
	}
	if (HaUsatoIlDash == true)
	{
		if (AlreadyJump == false && player->GetCharacterMovement()->GravityScale != 2.0f && OnDash == true)
		{
			player->GetCharacterMovement()->GravityScale = 2.0f;
			OnDash = false;
			HaUsatoIlDash = false;
		}
	}

	if (heavyFuel < MinHeavy)
	{
		if (LastTimeJump + 5 < GetWorld()->GetTimeSeconds())
		{
			heavyFuel= heavyFuel + 0.05;
			//UE_LOG(LogTemp, Warning, TEXT("Heavy fuel si riempie"));
			if (heavyFuel > MinHeavy)
			{
				heavyFuel = MinHeavy;
			}
		}
	}
	if (lightFuel < MinLight)
	{
		if (LastTimeJump + 5 < GetWorld()->GetTimeSeconds())
		{
			lightFuel= lightFuel + 0.1;
			//UE_LOG(LogTemp, Warning, TEXT("Light fuel si riempie"));
			if (lightFuel > MinLight)
			{
				lightFuel = MinLight;
			}
		}
	}
	if (lightFuel < 0)
	{
		lightFuel = 0;
	}
	if (CanJumpIsUp)
	{
		if (LastTimeJump + 0.5 < GetWorld()->GetTimeSeconds())
		{
			JetpackForceLxTime = JetpackForceL;
		}
	}
	//bozza per il max dash
	/*if (IsOnDash == true)
	{
		if (MaxTimeDash < GetWorld()->GetTimeSeconds())
		{
			FVector stop = FVector(0, 0, 0);
			player->LaunchCharacter(stop, false, true);
			UE_LOG(LogTemp, Warning, TEXT("stop dash"));
			IsOnDash = false;
		}
	}*/
	
	// ...
}

void UOrbis::SetUpInputComponent()
{
	//Ottiene gli input del project setting
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Action", IE_Pressed, this, &UOrbis::Action);//Action On
		InputComponent->BindAction("Action", IE_Released, this, &UOrbis::NoAction);//Action Off
		InputComponent->BindAction("Jetpack", IE_Pressed, this, &UOrbis::Fly);//Jetpack On
		InputComponent->BindAction("Jetpack", IE_Released, this, &UOrbis::NotFly);//Jetpack Off
		InputComponent->BindAction("ChangeState", IE_Pressed, this, &UOrbis::ChangeHeavyLight);//Cambio stato orbis
		InputComponent->BindAction("Dash", IE_Pressed, this, &UOrbis::Dash);//Dash
		InputComponent->BindAction("Dash", IE_Released, this, &UOrbis::NotDash);//Dash
	}
	else
	{
		//Verifica per essere sicuri che il componente sia stato trovato
		UE_LOG(LogTemp, Error, TEXT("%s Input Component not Found"), *GetOwner()->GetName());
	}
}



void UOrbis::EnableInput()
{
	Enable.Broadcast();
}

void UOrbis::DisableInput()
{
	Disable.Broadcast();
}

void UOrbis::IsOnAir()//serve ad evitare che con il salto semplice si attivi il jetpack
{
	CanJumpIsUp = true;
	AlreadyJump = true;
	
}

void UOrbis::Fly()
{
	if (CanJumpIsUp)
	{
		
		
		if (heavyFuel > 0 && playerState == HEAVY)
		{
			//Boleano che permette di volare tenendo premuto un tasto
			OnAir = true;
			//Boleano che controlla che Orbis sia in aria
			AlreadyJump = true;
			Camera->FindComponentByClass<UCamera>()->CameraZoomOut();//chiamata alla funzione zoomOut che serve per allontare la telecamera e simulure uno zoom verso fuori
			LastTimeJump = GetWorld()->GetTimeSeconds();
		}
		else
		{
			if (lightFuel > 0 && playerState == LIGHT)
			{
				//Boleano che permette di volare tenendo premuto un tasto
				OnAir = true;
				//Boleano che controlla che Orbis sia in aria
				AlreadyJump = true;
				Camera->FindComponentByClass<UCamera>()->CameraZoomOut();//chiamata alla funzione zoomOut che serve per allontare la telecamera e simulure uno zoom verso fuori
				LastTimeJump = GetWorld()->GetTimeSeconds();
			}	
		}
	}
	else
	{
		JetpackForceLxTime = JetpackForceLFirstHit;
		IsOnAir();
	}
}

void UOrbis::NotFly()
{
	//Boleano che fa smettere di volare se rilasciato il tasto
	OnAir = false;
	JetpackForceLxTime = JetpackForceLFirstHit;

}

void UOrbis::Dash()
{
	//Var per attivare il dash di orbis
	OnDash = true;
}

void UOrbis::NotDash()
{
	//Var per disattivare il dash di orbis
	if (playerState == LIGHT)
	{
		OnDash = false;//attenzione questa funzione vale solo per la forma light in modo da risolvere il bug del dash
	}
}

void UOrbis::CharacterOnDash()
{
	//funzione che gestisce il dash in caso di player forma light
	if (OnDash == true && playerState == LIGHT)
	{
		if (player)
		{
			if (DelayOnDash == false)//if che ne gestisce il delay mediante booleano
			{
				//se il player si trova in aria
				if (OnAir == true || CanJumpIsUp == true)
				{
					FVector DashMoviment = PlayerDirection * DashForce;
					//float clamp = FMath::Clamp<float>(DashMoviment, 0, 1000);
					player->LaunchCharacter(DashMoviment, false, true);
					IsOnDash = true;
					OnDash = false;
					MaxTimeDash = GetWorld()->GetTimeSeconds();
				}
				else//se il player si trova a terra
				{
					FVector DashMoviment = PlayerDirection * DashForce * 2;
					player->LaunchCharacter(DashMoviment, false, true);
					OnDash = false;
				}
			DelayOnDash = true;//si setta il booleano del delay a true
			RealTimeDash = GetWorld()->GetTimeSeconds();//si prende il tempo nel quale si è usato il dash in modo da poter calcolare il delay corretto
			}
		}
	}
	//funzine che gestisce nel caso il player sia in forma heavy
	if (OnDash == true && playerState == HEAVY)
	{
		if (player)
		{
			//se il player si trova in aria
			if (AlreadyJump == true)
			{
				if (StopFalling())
				{
					player->GetCharacterMovement()->GravityScale = FallingForce;
					HaUsatoIlDash = true;
				}
				else
				{
					player->GetCharacterMovement()->GravityScale = 2.0f;
					OnDash = false;
					HaUsatoIlDash = false;
				}
			}
			else//se il player si trova a terra
			{
				if (HaUsatoIlDash == false)
				{
					if (StopRunCharacter())
					{

						DisableInput();//blueprint che disabilita input
						player->GetCharacterMovement()->MaxWalkSpeed = 2000;
						player->AddMovementInput(PlayerDirection, 100);
						AlreadyJump = false;//per evitare che esegua sia l'azione in aria che l'azione a terra
						if (CanDoIt == false)//Candoit è il booleano che si occupa di gestire la corsa
						{

							CanDoIt = true;
							RealTimeRun = GetWorld()->GetTimeSeconds();
						}

					}
					else//la corsa si blocca se si tocca un ostacolo che non si può rompere
					{
						EnableInput();//blueprint che abilita input
						player->AddMovementInput(FVector(0.f, 0.f, 0.f), 100);
						player->GetCharacterMovement()->MaxWalkSpeed = 400;
						//settaggio dei booleani per poter rieseguire dinuovo la corsa
						OnDash = false;
						CanDoIt = false;
						DelayOnRun = true;

					}
					if (DelayOnRun == false)//la corsa si blocca superand il time massimo
					{
						EnableInput();//blueprint che abilita input
						player->AddMovementInput(FVector(0.f, 0.f, 0.f), 100);
						player->GetCharacterMovement()->MaxWalkSpeed = 400;
						//settaggio dei booleani per poter rieseguire dinuovo la corsa
						OnDash = false;
						CanDoIt = false;
						DelayOnRun = true;
					}
				}
			}

		}
	}

}
//Funzione del jetpack
void UOrbis::CharacterOnAir()
{
	if (OnAir == true && playerState == LIGHT)//Controlla se il player è in stato leggero e se il tasto è sempre premuto
	{
		//Controlla se il character esiste
		if (player)
		{

			//Se il riferimento a character non è null uso la funzione launch per farlo volare
			//Uso la force di Orbis leggero
			player->LaunchCharacter(JetpackForceLxTime, false, true);
			//Tolgo il fuel dal jetpack con la funzione
			lightFuel = RemoveFuel(lightFuel);

		}
	}

	if (OnAir == true && playerState == HEAVY)//Controlla se il player è in stato pesante e se il tasto è sempre premuto
	{
		//Controlla se il character esiste
		if (player)
		{

			if (DelayOnJump == false)
			{
				
				
				DelayOnJump = true;//booleano che serve per evitare la ripetizione del jump 
				RealTimeJump = GetWorld()->GetTimeSeconds();//presa del tempo in modo da far funzionare il delay sul jump
			
				//Se il riferimento a character non è null uso la funzione launch per farlo volare
				//Uso la forza di Orbis pesante
				player->LaunchCharacter(JetpackForceH, false, true);
				//Tolgo il fuel dal jetpack con la funzione
				heavyFuel = RemoveFuel(heavyFuel);
				OnAir = false;//Settp a false on air in modo che il player debba premere piu volte per salire di altezza	
			}
		}
	}
}

void UOrbis::ChangeHeavyLight()//Funzione che cambia forma di orbis
{
	switch (playerState)//Switch che cambia lo stato di orbis col click destro del mouse
	{
	//Se è pesante diventa leggero
	case HEAVY:
		
			playerState = LIGHT;
			//modifiche delle statistiche del player
			player->GetCharacterMovement()->MaxWalkSpeed = 600;
			player->GetCharacterMovement()->GravityScale = 2;
		
		break;
	//Se è leggero diventa pesante
	case LIGHT:
		
		
			checkCapsuleCollision = true;
			playerState = HEAVY;
			//modifiche delle statistiche del player
			player->GetCharacterMovement()->MaxWalkSpeed = 400;
			player->GetCharacterMovement()->GravityScale = 2.5;
		
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("State not found"));
		break;
	}
}

//Funzione per rimuovere fuel
//Gli posso passare il fuel grazie alla var locale fuel
float UOrbis::RemoveFuel(float fuel)
{
	//Tolgo fuel e lo uso come valore di ritorno
	fuel--;
	return fuel;
}

//Funzione per aggiungere Fuel
//Gli passo 2 var una è il fuel corrente e l' altra è il fuel max
float UOrbis::MoreFuel(float fuel, float fuelMax)
{
	//Finche il fuel corrente passato è minore di quello massimo lo ricarico
	while (fuel < fuelMax)
	{
		fuel++;
	}
	return fuel;
}
//Funzione che prende la location corrente dell' actor
//Usata come vettore d' inzio del linetrace
FVector UOrbis::StartLine()
{
	//Prende e immagazzina nel var la location di Orbis
	actorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//Ritorna la location
	return actorLocation;
}

//Funzione che prende la fine del vettore per determinare se Orbis è appoggiato su una piattaforma di ricarica
FVector UOrbis::EndLine()
{
	//Prendo la locazione
	actorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//Aggiungo il vettore che sta sull asse z e allungo con il range
	lineEndDirection = actorLocation + lineDirection * -range;
	//Ritorno dove finisce il vector del linetrace
	return lineEndDirection;
}
//Funzione che controlla se si sta su una piattaforma di ricarica
FHitResult UOrbis::GetRechargePlatform()
{
	//var che detiene l' oggetto colpito
	FHitResult Hit;

	//Da usare in caso di tag
	FCollisionQueryParams TraceParametres(FName(TEXT("")),
		                                  false,
		                                  GetOwner());
	//Line trace che controlla che l' oggetto colpito sia una piattaforma di ricarica
	GetWorld()->LineTraceSingleByObjectType(Hit,
		                                    StartLine(),
		                                    EndLine(),
		                                    FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		                                    TraceParametres);
	//Prende la reference dell attore che si sta colpendo
	AActor* ActorHit = Hit.GetActor();
	//Ritorna il risultato del  oggetto colpito sotto a Orbis
	return Hit;
}

FVector UOrbis::EndLineStopRun()//funzione per l'aggiornamento della linea frontale del player
{
	actorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	lineEndDirectionStopRun = actorLocation + lineDirectionStopRun * -range;
	return lineEndDirectionStopRun;
}
bool UOrbis::StopRunCharacter()//funzione che gestisce gli impatti frontali del player
{
	bool result = false;
	FHitResult Hit;
	//UE_LOG(LogTemp, Warning, TEXT("FUNZIONE CORSA"));

	FCollisionQueryParams TraceParametres(FName(TEXT("")),
		                                  false,
		                                  GetOwner());

	if (GetWorld()->LineTraceSingleByObjectType(Hit,
		                                        StartLine(),
		                                        EndLineStopRun(),
		                                        FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic),
		                                        TraceParametres))
	{
		AActor* ActorHit = Hit.GetActor();
		if (ActorHit->ActorHasTag("CoinsCerchio"))
		{
			result = true;

		}
		else
			if (ActorHit->ActorHasTag("CoinsTriangolo"))
			{
				result = true;

			}
			else
			if (ActorHit->ActorHasTag("Leva"))
			{
				result = true;

			}
				else
			{
				//UE_LOG(LogTemp, Warning, TEXT("FUNZIONE dynamic"));
				result = false;//nel caso del retunr false il player continua a correre siccome non impatta con nulla di indistruttibile
			}
		
	}
	else if (GetWorld()->LineTraceSingleByObjectType(Hit,
		                                             StartLine(),
		                                             EndLineStopRun(),
		                                             FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		                                             TraceParametres))
	{
		//UE_LOG(LogTemp, Warning, TEXT("FUNZIONE physic"));
		result = false;//nel caso del retunr false il player continua a correre siccome non impatta con nulla di indistruttibile
	}
	else
	{
		result = true;//nel caso di retunr del true vuol dire che abbiamo impattato con qualcosa di non distruttibile pertanto la corsa termina
	}
	
	if (GetWorld()->LineTraceSingleByObjectType(Hit,
		                                        StartLine(),
		                                        EndLineStopRun(),
		                                        FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		                                        TraceParametres))
	{
		//UE_LOG(LogTemp, Warning, TEXT("FUNZIONE Static"));
		if (!Hit.GetActor()->ActorHasTag("Leva"))
		{
			Hit.GetActor()->Destroy();

		}
		
	}

	

	return result;
}

bool UOrbis::HeavyLightPlatform()//funzione che si occupa di riconoscere le piattaforme speciali e le gestisce
{
	bool result = false;
	FHitResult Hit;

	FCollisionQueryParams TraceParametres(FName(TEXT("")),
		                                  false,
		                                  GetOwner());

	GetWorld()->LineTraceSingleByObjectType(Hit,
		                                    StartLine(),
		                                    EndLine(),
		                                    FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic),
		                                    TraceParametres);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)//nel caso questo actor sia verificato gestisce i casi in cui player e piattaforma coincidono
	{
		
		auto platform = Cast<AActor>(Hit.GetActor());
		if (ActorHit->ActorHasTag("Blue"))
		{
			//nel caso di forma light controlla che piattaforma e player abbiano lo stesso colore/forma durante l'impatto
			if (playerState == LIGHT)
			{

				if (platform)
				{
					platform->FindComponentByClass<UPlatformMoviment>()->SetTrue();//chiamata della funzione settrue della piattaforma per l'apertura della porta
					UE_LOG(LogTemp, Warning, TEXT("Platform : %s "), *platform->GetName());

				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No Cast"));
				}
				result = true;
			}
		}
		if (ActorHit->ActorHasTag("Red"))
		{
			//nel caso di forma heavy controlla che piattaforma e player abbiano lo stesso colore/forma durante l'impatto
			if (playerState == HEAVY)
			{


				if (platform)
				{
					platform->FindComponentByClass<UPlatformMoviment>()->SetTrue();//chiamata della funzione settrue della piattaforma per l'apertura della porta
					UE_LOG(LogTemp, Warning, TEXT("Platform : %s "), *platform->GetName());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No Cast"));
				}
				result = true;
			}
		}

	}
	return result;
}

bool UOrbis::StopFalling()//funzione che gestisce la caduta incontrollata del player in forma heavy
{
	bool result = false;
	FHitResult Hit;

	FCollisionQueryParams TraceParametres(FName(TEXT("")),
		                                  false,
		                                  GetOwner());
	//gestisce la caduta e la blocca all'impatto con un oggetto non distruttibile 
	if (GetWorld()->LineTraceSingleByObjectType(Hit,
		                                        StartLine(),
		                                        EndLine(),
		                                        FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic),
		                                        TraceParametres))
	{
		
		/*auto Object = Cast<UPrimitiveComponent>(Hit.GetActor()->GetRootComponent());
		if (Object)
		{
			UE_LOG(LogTemp, Warning, TEXT("non nullo"));
		}*/
		AActor* ActorHit = Hit.GetActor();
		if (ActorHit->ActorHasTag("CoinsCerchio"))
		{
			result = true;

		}
		else
			if (ActorHit->ActorHasTag("CoinsTriangolo"))
			{
				result = true;

			}
			else
				if (ActorHit->ActorHasTag("Leva"))
				{
					result = true;

				}
				else
			{
				AlreadyJump = false;//booleano di fine caduta
				result = false;
			}
	
	}
	else if (GetWorld()->LineTraceSingleByObjectType(Hit,
		                                             StartLine(),
		                                             EndLine(),
		                                             FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		                                             TraceParametres))
	{
		AlreadyJump = false;//booleano di fine caduta
		result = false;
	}
	else
	{
		
		result = true;
	}
	//funzione che distrugge gli oggetti distruttibili
	if (GetWorld()->LineTraceSingleByObjectType(Hit,
		                                        StartLine(),
		                                        EndLine(),
		                                        FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		                                        TraceParametres))
	{
		if (!Hit.GetActor()->ActorHasTag("Leva"))
		{
			Hit.GetActor()->Destroy();
		}
	}

	return result;
}

void UOrbis::CharacterHitTrigger()
{
	/*FHitResult Hit;

	FCollisionQueryParams TraceParametres(FName(TEXT("")),
		false,
		GetOwner());

	if (GetWorld()->LineTraceSingleByObjectType(Hit,
		StartLine(),
		EndLineStopRun(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Visibility),
		TraceParametres))
	{
		UE_LOG(LogTemp, Warning, TEXT(" %s"), *Hit.GetActor()->GetName());
	}
	{
		UE_LOG(LogTemp, Warning, TEXT("Hittato %s"),*Hit.GetActor()->GetName());
		if (Hit.GetActor()->ActorHasTag("Trigger"))
		{
			if (AlreadyTriggered)
			{
				UE_LOG(LogTemp, Warning, TEXT("Colpito"));
				if (Camera)
				{
					Camera->FindComponentByClass<UCamera>()->MoveCamera();
					AlreadyTriggered = false;
				}
			}

		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hittato nulla"));
		AlreadyTriggered = true;
	}*/

}


void UOrbis::CoinsCerchio()
{
	lightFuel = lightFuel + MinLight * 2;
	if (lightFuel > lightFuelMax)
	{
		lightFuel = lightFuelMax;
	}
}

void UOrbis::CoinsTriangolo()
{
	heavyFuel = heavyFuel + MinHeavy * 2;
	if (heavyFuel > heavyFuelMax)
	{
		heavyFuel = heavyFuelMax;
	}
}

void UOrbis::Action()
{
	if (CanUseAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Funzione action"));
		if (Leva != nullptr)
		{
			
			UE_LOG(LogTemp, Warning, TEXT("Utilizzo leva %s"), *Leva->GetName());
			CanChangeSpriteLeva = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("non puoi usare Funzione action"));
	}
}

void UOrbis::NoAction()
{
	

	CanChangeSpriteLeva = false;
}