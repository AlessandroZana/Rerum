// Fill out your copyright notice in the Description page of Project Settings.

#include "Rerum.h"
#include "PlatformLight.h"
#include "PlatformMoviment.h"


// Sets default values for this component's properties
UPlatformMoviment::UPlatformMoviment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlatformMoviment::BeginPlay()
{
	Super::BeginPlay();
	// ...
}


// Called every frame
void UPlatformMoviment::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	/*if (CanMove)
	{
		if (Moviment)
		{
			
			if (time+delay>GetWorld()->GetTimeSeconds())
			{
				UE_LOG(LogTemp, Warning, TEXT("FUNZIONA "));
				FVector position = FVector(0, 0, -5);
				GetOwner()->AddActorLocalOffset(position);
				
			}else
			{
				FVector position = FVector(0, 0, 0);
				GetOwner()->AddActorLocalOffset(position);
				OpenDoor();
				//UE_LOG(LogTemp, Warning, TEXT("Distruggi %s"),*Door->GetName());
			}
			
		


		}
		
	}
	else
	{
		if (Moviment)
		{
			OpenDoor();
		}
	}
	// ...*/
}

void UPlatformMoviment::SetTrue()
{
	/*delay = 0.15f;
	time = GetWorld()->GetTimeSeconds();
	Moviment = true;*/
	
	if (Door)//controllo se si è stato associato ad una porta / se la porta non è ancora stata distrutta
	{
		AActor* Botton = Cast<AActor>(GetOwner());//si casta l'attore che si è premuto
		Door->FindComponentByClass<UDoorComponent>()->DoorMoviment(Botton);//chiamata della funzione doormoviment con passaggio del actor premuto per il controllo se sia stato premuto il tasto giusto
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Porta già aperta"));
	}
}

void UPlatformMoviment::PlatformLightInit(UPlatformLight* platformInit)
{
	platformLight = platformInit;
}

void UPlatformMoviment::LightOn()
{
	if (!ensure(platformLight)) { return; }
	platformLight->SetVisibility(true);
}

void UPlatformMoviment::LightOff()
{
	if (!ensure(platformLight)) { return; }
	platformLight->SetVisibility(false);
}

/*void UPlatformMoviment::OpenDoor()
{
	//Door->FindComponentByClass<UDoorComponent>()->DoorMoviment();
	//FVector position = FVector(0, 0, -5);
	//Door->AddActorLocalOffset(position);
}*/


