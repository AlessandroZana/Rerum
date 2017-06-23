// Fill out your copyright notice in the Description page of Project Settings.

#include "Rerum.h"
#include "Camera.h"


// Sets default values for this component's properties
UCamera::UCamera()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCamera::BeginPlay()
{
	Super::BeginPlay();
	CameraY = GetOwner()->GetActorLocation().Y;//valore fisso di cameraY 
	positionY = GetOwner()->GetActorLocation().Y;//valore utilizzato per le variazioni di zoom
	// ...
	
}


// Called every frame
void UCamera::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	if (CantBlockit)//if che gestisce lo zoom in e zoom out della telecamere
	{
		//UE_LOG(LogTemp, Warning, TEXT("CantBlockIt = true "));
		positionY += VelocitaDizoomOutTelecamera;//aumento costante di distanza della telecamere in modo da non dare lo scatto
		if (positionY > CameraY + MaxCameraZoomOut)
		{
			positionY = CameraY + MaxCameraZoomOut;
		}

		//UE_LOG(LogTemp, Warning, TEXT("positionY : %f "),positionY);

	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("CantBlockIt = false "));
		positionY -= 50;//diminuzione costante di distanza della telecamere in modo da non dare lo scatto
		if (positionY < CameraY )
		{
			positionY = CameraY ;
		}
		//UE_LOG(LogTemp, Warning, TEXT("positionY : %f "), positionY);
	}
	if (player)
	{
		if (IsUsingJetpack == false)//se non sto usando il jetpack gestisce in modo normale la telecamera altrimenti sta fermo
		{
			if (IsMoving)
			{
				player->FindComponentByClass<UOrbis>()->DisableInput();
				if (Time + ComeBack < GetWorld()->GetTimeSeconds())
				{
					FVector Position = FVector(player->GetActorLocation().X, CameraY, player->GetActorLocation().Z + 60);//si setta l'fvector che poi diventrà la nuova posizione della camera

					if (Position.Z < LowZPosition)//controllo che la z della camera non vada sotto un tot in modo da non vedere troppo sotto il terrono
					{
						Position.Z = LowZPosition;
					}
					GetOwner()->SetActorLocation(Position, false, nullptr);//cambio della posizione della camera in modo costante in modo da simulare un follow del player 
					IsMoving = false;
					player->FindComponentByClass<UOrbis>()->EnableInput();
				}
			}
			else
			{
				
				//UE_LOG(LogTemp, Warning, TEXT("Posizione del player %s"), *player->GetActorLocation().ToString());

			//	UE_LOG(LogTemp, Warning, TEXT("Posizione della telecamera %s"), *GetOwner()->GetActorLocation().ToString());

				FVector Position = FVector(player->GetActorLocation().X, positionY, player->GetActorLocation().Z + 60);//si setta l'fvector che poi diventrà la nuova posizione della camera

				if (Position.Z < LowZPosition)//controllo che la z della camera non vada sotto un tot in modo da non vedere troppo sotto il terrono
				{
					Position.Z = LowZPosition;
				}

				//	UE_LOG(LogTemp, Warning, TEXT("Posizione del Fvector %s"), *Position.ToString());

				GetOwner()->SetActorLocation(Position, false, nullptr);//cambio della posizione della camera in modo costante in modo da simulare un follow del player 

				
			}
		}
		else
		{
			CameraZoomOut();
			
			//UE_LOG(LogTemp, Warning, TEXT("Posizione della telecamera %s"), *GetOwner()->GetActorLocation().ToString());
		}
		
	}
	
	// ...
}

void UCamera::MoveCamera()
{
	if (IsMoving != true)
	{
		if (NuovaPosizione)
		{
			IsMoving = true;
			FVector Position = FVector(NuovaPosizione->GetActorLocation().X, GetOwner()->GetActorLocation().Y + MaxCameraDistance, NuovaPosizione->GetActorLocation().Z + 60);
			if (Position.Z < 500)//controllo che la z della camera non vada sotto un tot in modo da non vedere troppo sotto il terrono
			{
				Position.Z = 500;
			}
			GetOwner()->SetActorLocation(Position, false, nullptr);
			Time = GetWorld()->GetTimeSeconds();

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATTENZIONE NUOVA POSIZIONE DELLA CAMERA ASSENTE"));
		}
	}
}

void UCamera::CameraZoomIn()
{
	if (IsUsingJetpack == true)//questa funzione viene richiamata quando il player non è più in aria ma viene fatto prima un controllo per verificare che sia stato usato il jetpack 
	{
		FVector Position = FVector(player->GetActorLocation().X, CameraY, player->GetActorLocation().Z + 60);//si setta l'fvector che poi diventrà la nuova posizione della camera

		if (Position.Z < LowZPosition)//controllo che la z della camera non vada sotto un tot in modo da non vedere troppo sotto il terrono
		{
			Position.Z = LowZPosition;
		}

		GetOwner()->SetActorLocation(Position, false, nullptr);//cambio della posizione della camera in modo costante in modo da simulare un follow del player 
		IsUsingJetpack = false;//permette la modifica da altre funzioni alla posizione della telecamera
		CantBlockit = false;//booleano che settato a false fa tornare la telecamera vicina al player
	}
}

void UCamera::CameraZoomOut()
{
	
	if (IsUsingJetpack == false || CantBlockit == true)//doppio booleano uno per determinare l'inizio del uso jetpack e l'altro per impedire che altre funzioni lavorino sulla telecamera
	{
		
		FVector Position = FVector(player->GetActorLocation().X,positionY, player->GetActorLocation().Z + 60);//si setta l'fvector che poi diventrà la nuova posizione della camera

		if (Position.Z < LowZPosition)//controllo che la z della camera non vada sotto un tot in modo da non vedere troppo sotto il terrono
		{
			Position.Z = LowZPosition;
		}

		GetOwner()->SetActorLocation(Position, false, nullptr);//cambio della posizione della camera in modo costante in modo da simulare un follow del player 
		IsUsingJetpack = true;//booleano che settato a true non fa modificare la telecamera da altre funzioni
		CantBlockit = true;//booleano che settato a true fa allontanare la telecamere dal player
	}
}
