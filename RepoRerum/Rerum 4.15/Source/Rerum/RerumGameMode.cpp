// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Rerum.h"
#include "RerumGameMode.h"
#include "RerumCharacter.h"

ARerumGameMode::ARerumGameMode()
{
	// set default pawn class to our character
	DefaultPawnClass = ARerumCharacter::StaticClass();	
}
