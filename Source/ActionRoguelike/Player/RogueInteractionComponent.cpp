// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "RogueGameTypes.h"
#include "Core/RogueInteractionInterface.h"
#include "Engine/OverlapResult.h"


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
	ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractionRadius);
	TArray<FOverlapResult> Overlaps;
	
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, CollisionShape);
	
	AActor* BestActor = nullptr;
	float HighestDotResult = -1.f;
	
	for (FOverlapResult& Overlap : Overlaps)
	{
		const FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		const FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();
		const FVector CameraDirection = PC->GetControlRotation().Vector();
		const float DotResult = FVector::DotProduct(OverlapDirection, CameraDirection);
		if (DotResult > HighestDotResult)
		{
			BestActor = Overlap.GetActor();
			HighestDotResult = DotResult;
		}
		
		DrawDebugBox(GetWorld(), OverlapLocation, FVector(50.0f),FColor::Red);
		DrawDebugString(GetWorld(), OverlapLocation, FString::Printf(TEXT("Dot: %f"), DotResult), nullptr, FColor::White, 0.0f, true, 1.0f);
	}
	
	// Found best Actor for Interaction
	if (BestActor)
	{
		SelectedActor = BestActor;
		DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.0f), FColor::Green);
	}
	
	DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);
}


void URogueInteractionComponent::Interact()
{
	/* if (IRogueInteractionInterface* InteractInterface = Cast<IRogueInteractionInterface>(SelectedActor))
	{
		InteractInterface->Interact();
	} */
	if (SelectedActor) IRogueInteractionInterface::Execute_Interact(SelectedActor);
}
