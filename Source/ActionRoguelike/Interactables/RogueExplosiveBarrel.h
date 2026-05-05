// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueExplosiveBarrel.generated.h"

class UNiagaraComponent;
class URadialForceComponent;
class USoundBase;
class UAudioComponent;
class UNiagaraSystem;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueExplosiveBarrel : public AActor {
	GENERATED_BODY()

public:
	ARogueExplosiveBarrel();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplodeDelay = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionImpulseStrength = 150000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionRadius = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionDamage = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	TSubclassOf<UDamageType> DmgTypeClass;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	TObjectPtr<UNiagaraSystem> BurningEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	TObjectPtr<USoundBase> BurningSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	TObjectPtr<USoundBase> ExplosionSound;
	
	void Explode();

private:
	bool bExploded = false;
	
	FTimerHandle ExplodeTimerHandle;
	
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ActiveBurningEffectComp = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> ActiveBurningSoundComp = nullptr;
};
