// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileBlackhole.h"

#include "RogueProjectileMagic.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


ARogueProjectileBlackhole::ARogueProjectileBlackhole()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileSpeed = 50.0f;
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(FName("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->ImpulseStrength = BlackholePullStrength;
	RadialForceComponent->Radius = BlackholePullRadius;
	RadialForceComponent->AddObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	RadialForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	RadialForceComponent->bIgnoreOwningActor = true;
	RadialForceComponent->SetAutoActivate(true);
		
	SphereComponent->SetSphereRadius(20.0f);
	SphereComponent->SetCollisionProfileName("BlackholeCore");
	
	ProjectileMovementComponent->InitialSpeed = 500.0f;
	InitialLifeSpan = 5.0f;
}

void ARogueProjectileBlackhole::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Note: Make sure GenerateOverlapEvents is enabled on the cubes in the world
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueProjectileBlackhole::OnSphereOverlappedActor);
}

void ARogueProjectileBlackhole::BeginPlay()
{
	Super::BeginPlay();
	RadialForceComponent->FireImpulse();
}


void ARogueProjectileBlackhole::OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}
