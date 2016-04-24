// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicOrc.h"
#include "Protherer.generated.h"

/**
 * 
 */
UCLASS()
class BASEPROJECT4_API AProtherer : public ABasicOrc
{
	GENERATED_BODY()
	
public:

	float DEBUG_STATEMENT_TIMEOUT_SECS = 50.0f;

	UFUNCTION(BlueprintCallable, Category = "Orc Movement")
		virtual void MoveOrc() override;

	virtual AWaypoint* GetNextWaypoint() override;

	AProtherer();

	AWaypoint* PredictedDestinationFor(AWaypoint* predictedWaypoint);

	AWaypoint* NextWaypointTowards(AWaypoint* destination);

	FString GetDialogText(FString* text);

	void sayDialog(FString* text);

	void PrintPath(TArray<AWaypoint*> path);
	
	virtual void SayDialogue() override;
	
};
