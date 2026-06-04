// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RogueCoinPickupSubsystem.generated.h"

class URogueDeveloperSettings;
class UAudioComponent;
class UInstancedStaticMeshComponent;
struct FPrimitiveInstanceId;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URogueCoinPickupSubsystem : public UTickableWorldSubsystem {
	GENERATED_BODY()
	// UTickableWorldSubsystem lifetime tied to level
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	void AddCoinPickups(TArray<FVector> NewLocations, TArray<int32> CoinAmounts);
	
	void RemoveCoinPickup(int32 IndexToRemove);
	
	virtual void Tick(float DeltaTime) override;
	
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(RogueCoinPickupSubsystem, STATGROUP_Tickables);
	}
	
protected:	
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> WorldISM;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> WorldAudioComp;
	
	TArray<FVector> CoinLocations;
	TArray<int32> CoinAmounts;
	TArray<FPrimitiveInstanceId> MeshIDs;
	
	void OnPickupMeshLoadComplete(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject) const;
	void OnPickupSoundLoadComplete(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject) const;
	
	void PlayPickupSound();
	
	FName CoinPickupAudioTriggerParamName;
};
