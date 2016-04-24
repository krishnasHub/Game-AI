// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Waypoint.h"
#include "BasicOrc.h"

#include "GameFramework/Character.h"
#include "Elf.generated.h"

UCLASS()
class BASEPROJECT4_API AElf : public ACharacter
{
	GENERATED_BODY()

public:

	FString T_TYPE_ROAD = "road";
	FString T_TYPE_CART = "cart";
	FString T_TYPE_FLY  = "fly";

	float CostToRoad = 10.0f;
	float costToCart = 70.0f;
	float CostToFly = 250.0f;

	FString* LastTrasnportUsed;

	bool IsMoving = false;
	bool ReadNarrative = false;

	UPROPERTY(EditAnywhere, Category = "Elf Dialogues")
		bool doneSayingDialogue = false;

	TArray<AWaypoint*> EnabledWaypoints;

	UPROPERTY(EditAnywhere, Category = "Elf Dialogues")
		TArray<USoundCue*> Dialogues;

	TArray<FString*> DialogueTexts;
	FString* CurrentDialogue;

	UPROPERTY(EditAnywhere, Category = "Elf Location")
		AWaypoint* CurrentLocation;

	UPROPERTY(EditAnywhere, Category = "Total Money")
		float totalDucats = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Elf Location")
		AWaypoint* FinalDestination;

	UFUNCTION(BlueprintCallable, Category = "Elf Dialogues")
		void ResetDoneSayingDialogue();

	UFUNCTION(BlueprintCallable, Category = "Elf Location")
		AWaypoint* GetCurrentLocationAsWaypoint();

	UFUNCTION(BlueprintCallable, Category = "Elf Movement")
		bool CheckIfReachedLocation();

	UFUNCTION(BlueprintCallable, Category = "Elf Location")
		void SetCurrentWaypointAsWaypoint(AWaypoint* waypont);

	UFUNCTION(BlueprintCallable, Category = "Elf Movement")
		FVector GetCurrentLocation();

	UPROPERTY(EditAnywhere, Category = "Player Tracking")
		TArray<ABasicOrc*> ListOfOtherOrcs;

	UFUNCTION(BlueprintCallable, Category = "Elf Movement")
		void DisableAllWaypoints();

	UFUNCTION(BlueprintCallable, Category = "Elf Movement")
		void EnableSelectedWaypoints(const FString& transportType);


	UFUNCTION(BlueprintCallable, Category = "Elf Movement")
		bool MoveToWaypoint(const FVector& clickedLocation);

	UFUNCTION(BlueprintCallable, Category = "Elf Movement")
		AWaypoint* GetClosestWaypoint(const FVector& clickedLocation);

	UFUNCTION(BlueprintCallable, Category = "End Game")
		float CheckEndOfGame();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
		FString GetDialog();

	UFUNCTION(BlueprintCallable, Category = "Ducats")
		float GetTotalDucats();

	// Sets default values for this character's properties
	AElf();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void ReduceDucats();

	TArray<AWaypoint*> GetAllNeighbors(AWaypoint* waypoint);

	// Helper functions..
	void DisableAllNeighborsOf(AWaypoint* location, TArray<AWaypoint*> visitedWaypoints);

	void EnableListOfWaypoints(TArray<AWaypoint*> waypoints);

	void MoveToWaypoint(AWaypoint* location);

	AWaypoint* GetSelectedWaypoint(TArray<AWaypoint*> waypoints, const FVector& clickedLocation);
	
	void PrintLogMessage(FString* message);

	void SayDialogue();
};
