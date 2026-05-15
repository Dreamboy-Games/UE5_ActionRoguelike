// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction_ProjectileAttack.h"

#include "NiagaraFunctionLibrary.h"
#include "RogueGameTypes.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogueProjectile.h"

TAutoConsoleVariable<float> CVarProjectileAimDebugDrawing(TEXT("game.projectile.DebugDraw"), 0.0f, TEXT("Enable projectile aim adjustment debug rendering. (0 = off, > 0 = duration)"), ECVF_Cheat);

URogueAction_ProjectileAttack::URogueAction_ProjectileAttack()
{
	CooldownTime = 0.5f;
}


void URogueAction_ProjectileAttack::StartAction_Implementation()
{
	Super::StartAction_Implementation();
	
	URogueActionSystemComponent* ActionComp = GetOwningComponent();
	ACharacter* Character = CastChecked<ACharacter>(ActionComp->GetOwner());
	
	// Play Attack Montage
	Character->PlayAnimMontage(AttackMontage);
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, Character->GetMesh(), MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	UGameplayStatics::PlaySound2D(this, CastingSound);
	
	// Start Attack Delay Timer
	FTimerHandle AttackTimerHandle;
	constexpr float AttackDelayTimer = 0.2f;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::AttackTimerElapsed, AttackDelayTimer, false);
}

void URogueAction_ProjectileAttack::StopAction_Implementation()
{
	Super::StopAction_Implementation();
}


void URogueAction_ProjectileAttack::AttackTimerElapsed()
{
	URogueActionSystemComponent* ActionComp = GetOwningComponent();
	ACharacter* Character = CastChecked<ACharacter>(ActionComp->GetOwner());
	
	FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(MuzzleSocketName);
	FVector AdjustedTargetLocation;
	
	FVector EyeLocation;
	FRotator EyeRotation;
	Character->GetController()->GetPlayerViewPoint(EyeLocation, EyeRotation);
	
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 5000.0f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	UWorld* World = GetWorld();
	
	FHitResult Hit;
	if (World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_PROJECTILE, QueryParams))
	{
		AdjustedTargetLocation = Hit.Location;
	}
	else
	{
		AdjustedTargetLocation = TraceEnd;
	}	
	FRotator SpawnRotation = (AdjustedTargetLocation - SpawnLocation).Rotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	Character->MoveIgnoreActorAdd(NewProjectile);
	
	StopAction();
	
#if !UE_BUILD_SHIPPING
	if (float DebugDrawDuration = CVarProjectileAimDebugDrawing.GetValueOnGameThread(); DebugDrawDuration > 0.0f)
	{
		// hit location or trace end
		DrawDebugBox(World, AdjustedTargetLocation, FVector(20.0f), FColor::Green, false, DebugDrawDuration);
		// adjusted line trace
		DrawDebugLine(World, EyeLocation, TraceEnd, FColor::Green, false, DebugDrawDuration);
		// new projectile path
		DrawDebugLine(World, SpawnLocation, AdjustedTargetLocation, FColor::Yellow, false, DebugDrawDuration);
		// original projectile path
		DrawDebugLine(World, SpawnLocation, SpawnLocation + (EyeRotation.Vector() * 5000.0f), FColor::Purple, false, DebugDrawDuration);
	}
#endif
}
