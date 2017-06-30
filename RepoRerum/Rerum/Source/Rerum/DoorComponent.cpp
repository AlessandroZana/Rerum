// Fill out your copyright notice in the Description page of Project Settings.

#include "Rerum.h"
#include "Camera.h"
#include "DoorComponent.h"
#include "PlatformMoviment.h"
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
	/*if (firstButton)
	{
		Count++;
	}
	if (secondButton)
	{
		Count++;
	}
	if (thirdButton)
	{
		Count++;
	}*/
}


// Called every frame
void UDoorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	if (platformSequence.Num() != 0)//nel caso che questo contatore sia nulla non si procederà con i passi successivi
	{
			if (IsSpecialDoor)//controllo se stiamo parlando di una door normale o di una special ( apertura 2 ante)
			{
				if (nextButtonIndex == platformSequence.Num())//se il numero di tasti premuto (pressed) è uguale al numero di tasti da premere (count) allora si apre la porta
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
					if (firstButtonPressed != nullptr)
					{
						firstButtonPressed->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					if (secondButtonPressed != nullptr)
					{
						secondButtonPressed->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					if (thirdButtonPressed != nullptr)
					{
						thirdButtonPressed->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					CanBeDestroy = true;//booleano che permette la distruzione delle ante quando finiscono il loro movimento di apertura
					
				}
			}
			else
			{
				if (nextButtonIndex == platformSequence.Num())//se il numero di tasti premuto (pressed) è uguale al numero di tasti da premere (count) allora si apre la porta
				{

					Open.Broadcast();
					//Telaio->SetActorEnableCollision(ECollisionEnabled::NoCollision);
				
					if (firstButtonPressed != nullptr)
					{
						firstButtonPressed->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					if (secondButtonPressed != nullptr)
					{
						secondButtonPressed->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					if (thirdButtonPressed != nullptr)
					{
						thirdButtonPressed->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
					}
					CanBeDestroy = true;//booleano che permette la distruzione delle ante quando finiscono il loro moviemnto di apertura
				}

			}
			if (CanBeDestroy == false)
			{
				//stampa della porta e del numero di pulsanti relativi a quella porta
				/*UE_LOG(LogTemp,Warning,TEXT("Porta : %s"),*GetOwner()->GetName())
				UE_LOG(LogTemp, Warning, TEXT(" Numero di pulsanti %d"), Count);
				UE_LOG(LogTemp, Warning, TEXT(" Numero di pulsanti premuti %d"), nextButtonIndex);*/
			}
			if (nextButtonIndex > 0)
			{
				if (sequenceStartTime + DelayPlatform < GetWorld()->GetTimeSeconds())
				{
					nextButtonIndex = 0;
					firstButtonPressed = nullptr;
					secondButtonPressed = nullptr;
					thirdButtonPressed = nullptr;
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

void UDoorComponent::HandlePlatformPressure(AActor * platform)//controllo tasti premuti
{
	if (platformSequence.Num() > nextButtonIndex)
	{
		if (platform == platformSequence[nextButtonIndex])
		{
			if (nextButtonIndex == 0)
			{
				sequenceStartTime = GetWorld()->GetTimeSeconds();
			}
			ActivatePlatform(platform);
			nextButtonIndex++;
		}
	}
}

void UDoorComponent::DelayDestroy()
{
	if (!alreadydestroy)
	{
		if (camera)
		{
			auto tmp = camera->FindComponentByClass<UCamera>();

			if (tmp)
			{
				tmp->NuovaPosizione = GetOwner();
				tmp->MoveCamera();
			}
		}
	}
	alreadydestroy = true;
	if (IsSpecialDoor)//prima di distruggere si verifica di quale tipologia di porta stiamo parlando tramite questo if
	{
		if (time + delay2 < GetWorld()->GetTimeSeconds())//nel caso di special door si distruggono 2 ante con un delay diverso
		{
			LeftDoor->Destroy();//distruzione 1 anta
			RightDoor->Destroy();//distruzione 1 anta
			//LeftDoor = nullptr;
			//RightDoor = nullptr;
		}
	}
	
	else
	{
		if (time + destroyDoor < GetWorld()->GetTimeSeconds())//nel caso di porta normale si distrugge una sola anta con un delay diverso
		{
			GetOwner()->Destroy();//distruzione 1 anta
		}
			
	}
}
void UDoorComponent::OpenWithLeva()
{
	time = GetWorld()->GetTimeSeconds();//salvataggio del tempo nella variabile time in modo da permettere la distruzione dopo un delay prestabilito

	Open.Broadcast();
	

		firstButton = nullptr;
	
		secondButton = nullptr;
	
		thirdButton = nullptr;
	
		DelayDestroy();//chaiamta della funzione che distrugge le porte
	
}

void UDoorComponent::ActivatePlatform(AActor* platform)
{
	auto platformMovement = platform->FindComponentByClass<UPlatformMoviment>();
	if (platformMovement)
	{
		platformMovement->LightOn();
		//UE_LOG(LogTemp, Warning, TEXT("Light"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Light"));
	}
	//platform->FindComponentByClass<UPlatformMoviment>()->Door = nullptr;
}
