// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

UCLASS(BlueprintType)
class BASEPROJECT4_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:
	FString T_TYPE_ROAD = "road";
	FString T_TYPE_CART = "cart";
	FString T_TYPE_FLY = "fly";

	float weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint Name")
		FString Name;

	
		static const float RoadCost;	
		static const float CartCost;	
		static const float FlyCost;

	// Sets default values for this actor's properties
	AWaypoint();

	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio Component")
		USoundCue* AudioComp;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint Path")
		bool enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint Path")
		TArray<AWaypoint*> WalkNeighbors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint Path")
		TArray<AWaypoint*> CartNeighbors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint Path")
		TArray<AWaypoint*> FlyNeighbors;
	
	UFUNCTION(BlueprintCallable, Category = "Waypoint Hover")
		void HoverOverMe(const bool entered);


	FVector GetLocationVector();
	
};
