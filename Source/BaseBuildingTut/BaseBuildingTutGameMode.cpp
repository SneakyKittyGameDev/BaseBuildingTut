// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseBuildingTutGameMode.h"
#include "BaseBuildingTutCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABaseBuildingTutGameMode::ABaseBuildingTutGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
