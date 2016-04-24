// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProject4.h"
#include "Waypoint.h"


// Sets default values
AWaypoint::AWaypoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	this->enabled = false;
}

const float AWaypoint::RoadCost = 20.0f;
const float AWaypoint::CartCost = 75.0f;
const float AWaypoint::FlyCost = 250.0f;

void AWaypoint::HoverOverMe(const bool entered)
{
	if (entered)
	{
		

		for (int i = 0; i < WalkNeighbors.Num(); ++i)
		{
			DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				WalkNeighbors[i]->GetActorLocation(),
				FColor(255, 0, 0, 100),
				false, -1, 0,
				12.333);

			DrawDebugSphere(
				GetWorld(),
				WalkNeighbors[i]->GetActorLocation(),
				24,
				32,
				FColor(255, 0, 0)
				);
		}

		for (int i = 0; i < CartNeighbors.Num(); ++i)
		{
			DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				CartNeighbors[i]->GetActorLocation(),
				FColor(0, 255, 0, 100),
				false, -1, 0,
				12.333);

			DrawDebugSphere(
				GetWorld(),
				CartNeighbors[i]->GetActorLocation(),
				24,
				32,
				FColor(0, 255, 0)
				);
		}

		for (int i = 0; i < FlyNeighbors.Num(); ++i)
		{
			DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				FlyNeighbors[i]->GetActorLocation(),
				FColor(0, 0, 255, 100),
				false, -1, 0,
				12.333);

			DrawDebugSphere(
				GetWorld(),
				FlyNeighbors[i]->GetActorLocation(),
				24,
				32,
				FColor(0, 0, 255)
				);
		}
		

		
	}
}


FVector AWaypoint::GetLocationVector()
{
	return this->GetActorLocation();
}