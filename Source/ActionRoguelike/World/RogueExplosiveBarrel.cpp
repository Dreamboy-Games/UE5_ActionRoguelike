// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueExplosiveBarrel.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


ARogueExplosiveBarrel::ARogueExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName("PhysicsActor");
	MeshComponent->Mobility = EComponentMobility::Movable;
	MeshComponent->bApplyImpulseOnDamage = true;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(FName("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->ImpulseStrength = ExplosionImpulseStrength;
	RadialForceComponent->Radius = ExplosionRadius;
	RadialForceComponent->DestructibleDamage = ExplosionDamage;
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;
}


void ARogueExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}


float ARogueExplosiveBarrel::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// Allow exploding once, also skip when timer is already running
	if (bExploded || GetWorldTimerManager().IsTimerActive(ExplodeTimerHandle))
	{
		return ActualDamage;
	}
	
	// Play Burning Sound & Effect
	ActiveBurningEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BurningEffect, MeshComponent, NAME_None, 
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	ActiveBurningSoundComp = UGameplayStatics::SpawnSoundAttached(BurningSound, MeshComponent);

	// Start Explode Timer
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &ARogueExplosiveBarrel::Explode, ExplodeDelay);
	
	return ActualDamage;
}

void ARogueExplosiveBarrel::Explode()
{
	bExploded = true;
	
	if (ActiveBurningEffectComp)	ActiveBurningEffectComp->Deactivate();
	if (ActiveBurningSoundComp) ActiveBurningSoundComp->Stop();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), GetActorRotation());
	
	// Splash Damage
	TArray<TObjectPtr<AActor>> IgnoreActors;
	IgnoreActors.Add(this);
	UGameplayStatics::ApplyRadialDamageWithFalloff(this, ExplosionDamage, 10.0f, MeshComponent->Bounds.Origin, 
		ExplosionRadius * 0.5f, ExplosionRadius, 100.f, DmgTypeClass, IgnoreActors, this);
	// Radial Impulse
	RadialForceComponent->FireImpulse();
	
	Destroy();
}