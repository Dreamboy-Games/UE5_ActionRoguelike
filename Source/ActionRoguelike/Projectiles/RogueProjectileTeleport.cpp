// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileTeleport.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RogueProjectileMagic.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARogueProjectileTeleport::ARogueProjectileTeleport()
{
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMovementComponent->InitialSpeed = 6000.0f;
}


void ARogueProjectileTeleport::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ThisClass::StartDelayedTeleport, 0.2f);
}

void ARogueProjectileTeleport::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// Clear timer to prevent a second teleportation
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
	StartDelayedTeleport();
}

void ARogueProjectileTeleport::StartDelayedTeleport()
{
	//PlayExplodeEffects();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExpandEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExpandSound, GetActorLocation(), FRotator::ZeroRotator);
	
	ProjectileMovementComponent->StopMovementImmediately();
	LoopedNiagaraComponent->Deactivate();
	LoopedAudioComponent->Stop();
	SetActorEnableCollision(false);
	
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ThisClass::HandleTeleportation, 0.2f);
}

void ARogueProjectileTeleport::HandleTeleportation()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, TeleportEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, TeleportSound, GetActorLocation(), FRotator::ZeroRotator);
	
	APawn* ActorToTeleport = GetInstigator();
	check(ActorToTeleport);
	
	ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
	
	Destroy();
}
