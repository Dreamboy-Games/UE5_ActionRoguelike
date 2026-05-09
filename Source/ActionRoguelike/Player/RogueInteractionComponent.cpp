// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "RogueGameTypes.h"
#include "Core/RogueInteractionInterface.h"
#include "Engine/OverlapResult.h"

TAutoConsoleVariable<bool> CVarInteractionDebugDrawing(TEXT("game.interaction.DebugDraw"), false, TEXT("Enable interaction component debug drawing. (0 = 0ff, 1 = enabled)"), ECVF_Cheat);


URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check for Interactables in range
	const APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	const FVector Center = PC->GetPawn()->GetActorLocation();
	const FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractionRadius);
	TArray<FOverlapResult> Overlaps;
	
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, CollisionShape);
	
	AActor* BestActor = nullptr;
	float HighestWeight = 0.0f;
	
	bool bEnabledDebugDraw = CVarInteractionDebugDrawing.GetValueOnGameThread();

	const float InteractionRadiusSqrd = InteractionRadius * InteractionRadius;
	
	for (FOverlapResult& Overlap : Overlaps)
	{
		FVector Origin;
		FVector BoxExtends;
		Overlap.GetActor()->GetActorBounds(true, Origin, BoxExtends);
		//const FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		
		const FVector OverlapDirection = (Origin - CameraLocation).GetSafeNormal();
		const FVector CameraDirection = PC->GetControlRotation().Vector();
		
		const float DistanceToSqrd = (Origin - Center).SizeSquared();
		// normalize and invert, smaller dist is higher weight
		const float NormalizedDistanceTo = 1.0f - (DistanceToSqrd / InteractionRadiusSqrd);
		
		const float DotResult = FVector::DotProduct(OverlapDirection, CameraDirection);
		const float NormalizedDotResult = DotResult * 0.5f + 0.5f; // -1to1 -> 0to1

		const float Weight = (NormalizedDotResult * DirectionWeightScale) + (NormalizedDistanceTo * DistanceToWeightScale);
		if (Weight > HighestWeight)
		{
			BestActor = Overlap.GetActor();
			HighestWeight = Weight;
		}
		
		if (bEnabledDebugDraw)
		{
			DrawDebugBox(GetWorld(), Origin, FVector(50.0f),FColor::Red);
			DrawDebugString(GetWorld(), Origin, FString::Printf(TEXT("Weight: %f, Dot: %f, Dist: %f"), Weight, NormalizedDotResult, NormalizedDistanceTo),
				nullptr, FColor::White, 0.0f, true, 1.0f);
		}
	}
	
	// Found best Actor for Interaction
	if (BestActor)
	{
		SelectedActor = BestActor;
		if (bEnabledDebugDraw) DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.0f), FColor::Green);
	}
	
	if (bEnabledDebugDraw) DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);
}


void URogueInteractionComponent::Interact()
{
	/* if (IRogueInteractionInterface* InteractInterface = Cast<IRogueInteractionInterface>(SelectedActor))
	{
		InteractInterface->Interact();
	} */
	const FVector Center = CastChecked<APlayerController>(GetOwner())->GetPawn()->GetActorLocation();
	bool SelectedActorInRange = (SelectedActor->GetActorLocation() - Center).Size() < InteractionRadius;
	if (SelectedActor && SelectedActorInRange)
	{
		IRogueInteractionInterface::Execute_Interact(SelectedActor);
	}
}
