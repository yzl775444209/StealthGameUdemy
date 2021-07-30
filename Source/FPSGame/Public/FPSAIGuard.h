// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

UENUM(BlueprintType)
enum class EAIState:uint8
{
	Idle,

	Suspicious,

	Alerted
};
UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UPawnSensingComponent* PawnSensingComp;
	//是否支持来回巡逻
	UPROPERTY(EditAnywhere, Category = "AI")
		bool bPatrol;
	//巡逻点一
	UPROPERTY(EditAnywhere, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor* FirstPatrolPoint;
	//巡逻点二
	UPROPERTY(EditAnywhere, Category = "AI", meta=(EditCondition= "bPatrol"))
		AActor* SecondPatrolPoint;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//当前的巡逻点
	AActor* CurrentPoint;
	//当前状态
	UPROPERTY(ReplicatedUsing=OnRep_GuideState)
	EAIState GuardState;
	//时间句柄
	FTimerHandle ResetHandle;
	//初始面向
	FRotator OrignRotator;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void OnRep_GuideState();
	UFUNCTION()
	void OnPawnSeend(APawn* Pawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* Pawn, const FVector& Location, float Volume);

	//来回走
	void MoveToNextPatrolPoint();

	//重置转向
	UFUNCTION()
	void ResetRotation();

	void SetGuardState(EAIState NewState);
	UFUNCTION(BlueprintImplementableEvent)
		void OnGuardChanged(EAIState NewState);
};
