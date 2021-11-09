// Copyright Epic Games, Inc. All Rights Reserved.

#include "RogadardGameMode.h"
#include "RogadardPlayerController.h"
#include "RogadardCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARogadardGameMode::ARogadardGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARogadardPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}