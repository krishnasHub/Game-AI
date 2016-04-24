// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Waypoint.h"

#include "GameFramework/Character.h"
#include "BasicOrc.generated.h"

UCLASS()
class BASEPROJECT4_API ABasicOrc : public ACharacter
{
	GENERATED_BODY()

public:

	FString T_TYPE_ROAD = "road";
	FString T_TYPE_CART = "cart";
	FString T_TYPE_FLY  = "fly";

	float DEBUG_STATEMENT_TIMEOUT_SECS = 150.0f;

	float CLUSTER_DISTANCE = 10000.0f;

	float ORC_LOCALE_STRENGTH = 0.4f;
	float EGRESS_STRENGTH = 0.35f;
	float RANGE_STRENGTH = 0.45f;
	float FINAL_DEST_STRENGTH = 0.6f;


	float EGRESS_RANGE_FACTOR_ROAD = 0.65f;
	float EGRESS_RANGE_FACTOR_CART = 0.4f;
	float EGRESS_RANGE_FACTOR_FLY = 0.2f;

	// Variables neded by this Base Class for all Orcs..
	TArray<AWaypoint*> PlayerLocations;

	bool CanSeePlayer = false;

	int PlayerLocationCountAnalysed;

	// The name of the transportation used:
	// It is one of:
	// Foot
	// Cart
	// Fly
	TArray<FString*> TransportUsed;

	// Every Orc must have some name..
	FString* OrcName;

	UPROPERTY(EditAnywhere, Category = "Orc Dialogues")
		TArray<USoundCue*> Dialogues;

	TArray<FString*> DialogueTexts;
	FString* CurrentDialogue;

	// To see if this Orc is moving or not.
	UPROPERTY(EditAnywhere, Category = "Orc Location")
		bool IsMoving;

	UPROPERTY(EditAnywhere, Category = "Orc Location")
		AWaypoint* CurrentLocation;

	UPROPERTY(EditAnywhere, Category = "Player Tracking")
		TArray<ABasicOrc*> ListOfOtherOrcs;

	UPROPERTY(EditAnywhere, Category = "Player Tracking")
		AWaypoint* FinalDestination;

	virtual void MoveOrc();

	UFUNCTION(BlueprintCallable, Category = "Orc Movement")
		void CheckIfOrcReachedWaypoint();

	UFUNCTION(BlueprintCallable, Category = "Player Tracking")
		void AddPlayerLocation(AWaypoint* lastKnownLocation);

	UFUNCTION(BlueprintCallable, Category = "Player Tracking")
		void AddTransportUsed(const FString& transportType);

	UFUNCTION(BlueprintCallable, Category = "Orc Movement")
	AWaypoint* GetCurrentLocation();

	/* Functions used for prediction */

	AWaypoint* PredictPlayerPosition();

	TArray<AWaypoint*> GetProbableNeighbors(AWaypoint* start, FString* transportUsed);
	TArray<AWaypoint*> GetAllProbableNeighbors(AWaypoint* start, TArray<FString*> transportUsed);	
	float DistanceToOrc(ABasicOrc* orc, AWaypoint* waypoint);
	TArray<ABasicOrc*> GetallLocalizedOrcs(AWaypoint* waypoint);
	TArray<AWaypoint*> RemoveWaypointsHavingOrcs(TArray<AWaypoint*> neighbors);
	void AssignWeights(TArray<AWaypoint*> neighbors);
	void AssignWeightBasedOnOrcLocality(TArray<AWaypoint*> neighbors);
	void AssignWeightBasedOnEgress(TArray<AWaypoint*> neighbors);
	void AssignWeightBasedOnEgressRange(TArray<AWaypoint*> neighbors);
	void AssignWeightBasedOnFinalDestination(TArray<AWaypoint*> neighbors);
	bool IsWaypointHighlyUnlikely(AWaypoint* neighbor);
	void FilterTransportUsed();
	void EnrichPlayerLocations(TArray<AWaypoint*> allProbableNeighbors);
	

	TArray<AWaypoint*> ClearDuplicateWaypoints(TArray<AWaypoint*> waypoints);
	AWaypoint* GetWaypointWithMaxWeight(TArray<AWaypoint*> waypoints);
	void PrintWaypoints(TArray<AWaypoint*> waypoints);
	float GetMaxOf(TArray<float> floatValues);
	int GetMaxOf(TArray<int> intValues);
	/* End of functions used in prediction */

	virtual void SayDialogue();

	// Finds the List of Waypoints to pick in order to reach a destination Waypoint from the Source Waypoint.
	// Basically is nothing but Depth first search over the graph..
	TArray<AWaypoint*> GetShortestPathBetween(AWaypoint* source, AWaypoint* destination, TArray<AWaypoint*> travelledList);

	TArray<AWaypoint*> GetAllNeighbors(AWaypoint* waypoint);

	// This function decides the next Waypoint to pick for the ORC.
	virtual AWaypoint* GetNextWaypoint();

	// Sets default values for this character's properties
	ABasicOrc();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	void PrintLogMessage(FString* message);
	
};
