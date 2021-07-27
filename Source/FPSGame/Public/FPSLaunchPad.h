// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();
	UPROPERTY(VisibleAnywhere,Category = "Component")
		class UBoxComponent* boxComp;
	UPROPERTY(VisibleAnywhere,Category = "Component")
		class UStaticMeshComponent* MeshComp;
	UFUNCTION()
		void OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystem* ActivateLaunchPadEffect;
	UPROPERTY(VisibleAnywhere, Category = "Component")
		float ForceAngle;
	UPROPERTY(VisibleAnywhere, Category = "Component")
		float ForceStrength;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
