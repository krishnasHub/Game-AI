// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicOrc.h"
#include "Tracker.generated.h"

/**
 * 
 */
UCLASS()
class BASEPROJECT4_API ATracker : public ABasicOrc
{
	GENERATED_BODY()
	
public:

	float DEBUG_STATEMENT_TIMEOUT_SECS = 50.0f;

	UFUNCTION(BlueprintCallable, Category = "Orc Movement")
		virtual void MoveOrc() override;

	virtual AWaypoint* GetNextWaypoint() override;

	ATracker();

	AWaypoint* NextWaypointTowards(AWaypoint* destination);

	FString GetDialogText(FString* text);

	void sayDialog(FString* text);
	
	void PrintPath(TArray<AWaypoint*> path);

	virtual void SayDialogue() override;
	
};
