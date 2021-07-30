// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "Public\FPSPlayerController.h"
#include "Engine\World.h"
#include "GameFramework\Pawn.h"
void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	//遍历所有的玩家
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator();it;it++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(it->Get());
		if (PC && PC->IsLocalController()) {
			PC->OnMissionCompleted(InstigatorPawn,bMissionSuccess);
			APawn* pawn = PC->GetPawn();
			//停止玩家操作
			if (pawn) {
				pawn->DisableInput(PC);
			}
		}
	}
}
