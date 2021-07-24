// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

class UStaticMeshComponent;
UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

	UPROPERTY(VisibleAnywhere,Category = "Cmp")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere,Category = "Cmp")
		class USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere, Category = "Effect")
		class UParticleSystem* PickUpFx;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//添加碰撞时候的光效
	void PlayEffects();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//同时Actor发生的overlap
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;
};
