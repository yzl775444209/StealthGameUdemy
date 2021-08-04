// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Public/FPSGameMode.h"
#include "Net/UnrealNetwork.h"
// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	SetReplicates(true);
	//GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeend);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
	if(bPatrol)
		MoveToNextPatrolPoint();
	if(HasAuthority())
		SetGuardState(EAIState::Idle);
	UE_LOG(LogTemp, Warning,TEXT("ttt %d"),GuardState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bPatrol) {
		// Patrol Goal Checks
		if (CurrentPoint)
		{
			FVector Delta = GetActorLocation() - CurrentPoint->GetActorLocation();
			Delta.Z = 0;
			float DistanceToGoal = Delta.Size();

			// Check if we are within 75 units of our goal, if so - pick a new patrol point
			// Keep in mind this includes vertical height difference! If your patrol point is in the floor, the distance to the pivot of guard is higher
			// In that case you may need to increase this value in your project or better align control points (ideally you remove Z axis all together by using
			// Alternative: float DistanceToGoal = FMath::Distance2D(GetActorLocation(), CurrentPatrolPoint->GetActorLocation());

			if (DistanceToGoal < 75)
			{
				MoveToNextPatrolPoint();
			}
		}
	}
}

// Called to bind functionality to input
void AFPSAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AFPSAIGuard::OnPawnSeend(APawn* Pawn)
{
	if (Pawn==nullptr) {
		return;
	}
	DrawDebugSphere(GetWorld(), Pawn->K2_GetActorLocation(), 10.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode* GM =Cast<AFPSGameMode>( GetWorld()->GetAuthGameMode());
	if (GM) {
		//游戏结束
		GM->CompleteMission(Pawn,false);
	}
	//设置AI状态
	SetGuardState(EAIState::Alerted);
	GetController()->StopMovement();
	
}

void AFPSAIGuard::OnNoiseHeard(APawn* Pawn, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), Location, 10.0f, 12, FColor::Green, false, 10.0f);

	FVector fDirection = GetActorLocation() - Pawn->K2_GetActorLocation();
	fDirection.Normalize();
	OrignRotator = GetActorRotation();
	fDirection.Z = 0;
	
	FRotator fRotator = FRotationMatrix::MakeFromX(fDirection).Rotator();
	SetActorRotation(fRotator);
	GetWorldTimerManager().ClearTimer(ResetHandle);
	//GetWorldTimerManager().SetTimer(ResetHandle, this, &AFPSAIGuard::ResetRotation, 3.0f, false, -1.0f);
	GetWorldTimerManager().SetTimer(ResetHandle, FTimerDelegate::CreateUObject(this, &AFPSAIGuard::ResetRotation), 3.0f, false, -1.0f);
	SetGuardState(EAIState::Suspicious);
	GetController()->StopMovement();
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if (CurrentPoint == nullptr || CurrentPoint == SecondPatrolPoint) {
		CurrentPoint = FirstPatrolPoint;
	}
	else 
	{
		CurrentPoint = SecondPatrolPoint;
	}
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPoint);
}

void AFPSAIGuard::ResetRotation()
{
	if(GuardState==EAIState::Alerted)
		return;
	SetGuardState(EAIState::Idle);
	SetActorRotation(OrignRotator);
	if (bPatrol) {
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
		return;
	GuardState = NewState;
}

void AFPSAIGuard::OnRep_GuideState()
{
	OnGuardChanged(GuardState);
}
void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSAIGuard,GuardState);
}