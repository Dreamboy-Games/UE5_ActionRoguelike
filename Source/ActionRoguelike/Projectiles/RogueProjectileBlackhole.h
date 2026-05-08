// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueProjectileBlackhole.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueProjectileBlackhole : public ARogueProjectile {
	GENERATED_BODY()

public:
	ARogueProjectileBlackhole();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<URadialForceComponent> RadialForceComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Blackhole")
	float BlackholePullStrength = -800000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackhole")
	float BlackholePullRadius = 1200.0f;
	
	UFUNCTION()
	void OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
};
