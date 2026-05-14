// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTTask_RangedAttack.h"

#include "AIController.h"
#include "RogueGameTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogueProjectile.h"

URogueBTTask_RangedAttack::URogueBTTask_RangedAttack()
{
	bNotifyTick = true;
	TargetActorKey.SelectedKeyName = NAME_TargetActor;
}

EBTNodeResult::Type URogueBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Validate Target - Has TargetActor been killed/destroyed?
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(TargetActor))
	{
		return EBTNodeResult::Failed;
	}
	
	// Start Rotation to Face TargetActor
	OwnerComp.GetAIOwner()->SetFocus(TargetActor, EAIFocusPriority::Gameplay);
	
	return EBTNodeResult::InProgress;
}

void URogueBTTask_RangedAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Validate Target - Has TargetActor been killed/destroyed?
	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(TargetActor))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	ACharacter* Pawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(Pawn);
	
	// Rotate to Face TargetActor
	FVector DirectionToTarget = TargetActor->GetActorLocation() - Pawn->GetActorLocation();
	DirectionToTarget.Z = 0.0f;
	const FRotator TargetRotation = DirectionToTarget.Rotation();
	if (!DirectionToTarget.IsNearlyZero())
	{
		const FRotator NewRotation = FMath::RInterpTo(Pawn->GetActorRotation(), TargetRotation, DeltaSeconds, 8.0f);
		OwnerComp.GetAIOwner()->SetControlRotation(NewRotation);
		Pawn->SetActorRotation(NewRotation);
	}

	// If Not Facing Target, return
	const float YawDelta = FMath::Abs(FMath::FindDeltaAngleDegrees(Pawn->GetActorRotation().Yaw, TargetRotation.Yaw));
	if (YawDelta > 5.0f)
	{
		return;
	}
	
	// Shoot Projectile
	const FVector SpawnLocation = Pawn->GetMesh()->GetSocketLocation(MuzzleSocketName);

	const FVector Direction = TargetActor->GetActorLocation() - SpawnLocation;
	FRotator SpawnRotation = Direction.Rotation(); 
	SpawnRotation.Pitch += FMath::FRandRange(0.0f, MaxBulletSpread);
	SpawnRotation.Yaw += FMath::FRandRange(-MaxBulletSpread, MaxBulletSpread);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Pawn;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	FinishLatentTask(OwnerComp, NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}
