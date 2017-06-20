// Fill out your copyright notice in the Description page of Project Settings.

#include "Rerum.h"
#include "Camera.h"
#include "DoorComponent.h"
#include "PaperSprite.h"


// Sets default values for this component's properties
UDoorComponent::UDoorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorComponent::BeginPlay()
{
	Super::BeginPlay();
	//controllo se è stato inserito almeno un pulsante e settaggio del contatore
	if (Botton1)
	{
		Count++;
	}
	if (Botton2)
	{
		Count++;
	}
	if (Botton3)
	{
		Count++;
	}
	
}


// Called every frame
void UDoorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	if (Count != 0)//nel caso che questo contatore sia nulla non si procederà con i passi successivi
	{
			if (IsSpecialDoor)//controllo se stiamo parlando di una door normale o di una special ( apertura 2 ante)
			{
				if (Pressed == Count)//se il numero di tasti premuto (pressed) è uguale al numero di tasti da premere (count) allora si apre la porta

				{
					if (LeftDoor)//se left door non è nulla la si muove 
					{
						FVector newposition = FVector(-3, 0, 0);
						LeftDoor->AddActorLocalOffset(newposition);
					}
					if (RightDoor)//se right door non è nulla la si muove 
					{
						FVector newposition2 = FVector(-3, 0, 0);
						RightDoor->AddActorLocalOffset(newposition2);
					}
					//Telaio->SetActorEnableCollision(ECollisionEnabled::NoCollision);
					if (Number1 != nullptr)
					{
						Number1->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					if (Number2 != nullptr)
					{
						Number2->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					if (Number3 != nullptr)
					{
						Number3->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					CanBeDestroy = true;//booleano che permette la distruzione delle ante quando finiscono il loro moviemnto di apertura
					
				}
			}
			else
			{
				if (Pressed == Count)//se il numero di tasti premuto (pressed) è uguale al numero di tasti da premere (count) allora si apre la porta
				{

					Open.Broadcast();
					//Telaio->SetActorEnableCollision(ECollisionEnabled::NoCollision);
				
					if (Number1 != nullptr)
					{
						Number1->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					if (Number2 != nullptr)
					{
						Number2->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					if (Number3 != nullptr)
					{
						Number3->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					CanBeDestroy = true;//booleano che permette la distruzione delle ante quando finiscono il loro moviemnto di apertura
				}

			}
			if (CanBeDestroy == false)
			{
				//stampa della porta e del numero di pulsanti relativi a quella porta
				/*UE_LOG(LogTemp,Warning,TEXT("Porta : %s"),*GetOwner()->GetName())
				UE_LOG(LogTemp, Warning, TEXT(" Numero di pulsanti %d"), Count);
				UE_LOG(LogTemp, Warning, TEXT(" Numero di pulsanti premuti %d"), Pressed);*/
			}
			if (Pressed > 0)
			{
				if (time2 + DelayPlatform < GetWorld()->GetTimeSeconds())
				{
					Pressed = 0;
					Number1 = nullptr;
					Number2 = nullptr;
					Number3 = nullptr;
				}
			}

	}

	if (CanBeDestroy)
	{
		DelayDestroy();//chaiamta della funzione che distrugge le porte
	}
	else
	{
		time = GetWorld()->GetTimeSeconds();//salvattaggio del tempo nella variabile time in modo da permettere la distruzione dopo un delay prestabilito
	}

}

void UDoorComponent::DoorMoviment(AActor * value)//controllo tasti premuti
{
	if (Pressed == 0)//contatore che stabilisce il tasto da premere
	{
		if (value == Botton1)//se il tasto premuto è uguale al tasto da premere si aumenta il contatore dei tasti premuti
		{
			time2 = GetWorld()->GetTimeSeconds();
			Number1 = value;
			//value->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
			Pressed++;
		}
	}
	if (Pressed == 1)//contatore che stabilisce il tasto da premere
	{
		if (value == Botton2)//se il tasto premuto è uguale al tasto da premere si aumenta il contatore dei tasti premuti
		{
			Number2 = value;
			//value->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
			Pressed++;
		}
	}
	if (Pressed == 2)//contatore che stabilisce il tasto da premere
	{
		if (value == Botton3)//se il tasto premuto è uguale al tasto da premere si aumenta il contatore dei tasti premuti
		{
			Number3 = value;
			//value->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
			Pressed++;
		}
	}

}

void UDoorComponent::DelayDestroy()
{
	
	if (IsSpecialDoor)//prima di distruggere si verifica di quale tipologia di porta stiamo parlando tramite questo if
	{
		if (time + delay2 < GetWorld()->GetTimeSeconds())//nel caso di special door si distruggono 2 ante con un delay diverso
		{
			LeftDoor->Destroy();//distruzzione 1 anta
			RightDoor->Destroy();//distruzzione 1 anta
			//LeftDoor = nullptr;
			//RightDoor = nullptr;
		}
	}
	
	else
	{
		if (time + destroyDoor < GetWorld()->GetTimeSeconds())//nel caso di porta normale si distrugge una sola anta con un delay diverso
		{
			GetOwner()->Destroy();//distruzzione 1 anta
		}
			
	}
}
void UDoorComponent::OpenWithLeva()
{
	time = GetWorld()->GetTimeSeconds();//salvattaggio del tempo nella variabile time in modo da permettere la distruzione dopo un delay prestabilito

	Open.Broadcast();
	

		Botton1 = nullptr;
	
		Botton2 = nullptr;
	
		Botton3 = nullptr;
	
		DelayDestroy();//chaiamta della funzione che distrugge le porte
	
}
