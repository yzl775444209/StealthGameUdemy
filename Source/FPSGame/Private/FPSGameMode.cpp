// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn) {
		if (SpectatingViewpointClass) {
			AActor* NewViewTarget = UGameplayStatics::GetActorOfClass(this, SpectatingViewpointClass);
			if (NewViewTarget) {
				for (auto It = GetWorld()->GetPlayerControllerIterator();It;It++)
				{
					APlayerController* PC = It->Get();
					if(PC)
						PC->SetViewTargetWithBlend(NewViewTarget, 0.5, EViewTargetBlendFunction::VTBlend_Linear);
				}
			}
		}
	}
}
