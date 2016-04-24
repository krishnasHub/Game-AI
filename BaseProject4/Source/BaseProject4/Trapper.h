// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicOrc.h"
#include "Trapper.generated.h"

/**
 * 
 */
UCLASS()
class BASEPROJECT4_API ATrapper : public ABasicOrc
{
	GENERATED_BODY()
public:

	bool ReachedFinalDestination = false;

	int HopCountAwayFromDestination = 0;

	int MAX_HOP_COUNT = 2;

	float DEBUG_STATEMENT_TIMEOUT_SECS = 50.0f;

	UFUNCTION(BlueprintCallable, Category = "Orc Movement")
		virtual void MoveOrc() override;

	virtual AWaypoint* GetNextWaypoint() override;

	ATrapper();

	void CalculateFinalDestination(AWaypoint* predictedWaypoint);

	AWaypoint* PredictedDestinationFor(AWaypoint* predictedWaypoint);

	AWaypoint* NextWaypointTowards(AWaypoint* destination);

	FString GetDialogText(FString* text);

	void LogText(FString* text);

	void PrintPath(TArray<AWaypoint*> path);
	
	virtual void SayDialogue() override;
	
};
