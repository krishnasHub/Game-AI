// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProject4.h"
#include "Trapper.h"
#include "Constants.h"




ATrapper::ATrapper()
{
	this->OrcName = new FString("Gogron/Trapper: ");
	this->IsMoving = false;
	ReachedFinalDestination = false;

	DialogueTexts.Add(new FString("Cheering!"));
}

AWaypoint* ATrapper::GetNextWaypoint()
{
	AWaypoint* predictedLocation = PredictPlayerPosition();
	AWaypoint* nextWaypoint;
	FString* text;

	if (predictedLocation)
	{
		//this->sayDialog(new FString("Found Predicted Location."));
		if (CanSeePlayer)
		{
			nextWaypoint = NextWaypointTowards(predictedLocation);
			CanSeePlayer = false;
		}
		else
		{
			nextWaypoint = NextWaypointTowards(PredictedDestinationFor(predictedLocation));
		}

		

		if (nextWaypoint)
			return nextWaypoint;

		text = new FString("Weird Condition: Moving to Predicted Waypoint - ");
		text->Append(predictedLocation->Name);
		this->LogText(text);

		return predictedLocation;
	}

	this->LogText(new FString("Weird Condition: Moving to default Location."));
	return this->CurrentLocation->WalkNeighbors[0];
}


void ATrapper::CalculateFinalDestination(AWaypoint* predictedWaypoint)
{
	// Based on predictedWaypoint and PlayerLocations, set the value of FinalDestination..
}


/*
	Given the predicted location of the Player and the List of 
	older player locations get the waypoint that probably gives the 
	next path
*/
AWaypoint* ATrapper::PredictedDestinationFor(AWaypoint* predictedWaypoin)
{
	if (!FinalDestination)
		return predictedWaypoin;

	if (this->CurrentLocation == FinalDestination)
		ReachedFinalDestination = true;

	if (HopCountAwayFromDestination == MAX_HOP_COUNT)
	{
		HopCountAwayFromDestination = -1;
		ReachedFinalDestination = false;
	}

	HopCountAwayFromDestination++;

	if (!ReachedFinalDestination)
	{
		return FinalDestination;
	}
	else
	{
		return predictedWaypoin;
	}
}

void ATrapper::MoveOrc()
{
	this->LogText(&((new FString("Responding to transportation type: "))->Append(*(TransportUsed.Last(0)))));
	this->CurrentLocation = GetNextWaypoint();
	

	UNavigationSystem::SimpleMoveToLocation(GetController(), this->CurrentLocation->GetActorLocation());
	this->IsMoving = true;
}


AWaypoint* ATrapper::NextWaypointTowards(AWaypoint* destination)
{
	FString* text = NULL;

	/*
	text = new FString("Finding path from ");
	text->Append(CurrentLocation->Name);
	text->Append(" to ");
	text->Append(destination->Name);
	sayDialog(text);
	*/

	// 1. Get the shortest path from CurrentLocation to destination..
	TArray<AWaypoint*> emptyList;
	TArray<AWaypoint*> shortestPath = GetShortestPathBetween(CurrentLocation, destination, emptyList);

	// If there is a shortest path, return the frst element from it!.
	if (shortestPath.Num() > 1)
	{
		//sayDialog(new FString("Found Shortest Path! Here it is:"));
		PrintPath(shortestPath);

		text = new FString("Going to the first node of the shortest path ");
		text->Append(shortestPath[1]->Name);
		LogText(text);
		return shortestPath[1];
	}

	// If there is no shortest path from CurrentLocation to Destination (which should not happen!)
	// then return null.
	LogText(new FString("No promising path found.."));
	return NULL;
}

void ATrapper::PrintPath(TArray<AWaypoint*> path)
{
	FString* text = new FString("Printing Path with ");
	text->AppendInt(path.Num());
	text->Append(" hops: ");

	for (int i = 0; i < path.Num(); ++i)
	{
		text->Append(path[i]->Name);
		text->Append(", ");
	}

	LogText(text);
}


FString ATrapper::GetDialogText(FString* text)
{
	FString* retVal = new FString("");
	retVal->Append(*this->OrcName);
	retVal->Append(*text);

	return *retVal;
}

void ATrapper::LogText(FString* text)
{
	if (Constants::SHOW_DEBUG_LOGS)
		GEngine->AddOnScreenDebugMessage(-1, DEBUG_STATEMENT_TIMEOUT_SECS, FColor::Yellow, this->GetDialogText(text));

	UE_LOG(YourLog, Log, TEXT("Trapper: %s"), *(*text));
}


void ATrapper::SayDialogue()
{
	// Play one of Dialogues
	// Set CurrentDialogue as the newly selected dialogue..

	if (Dialogues.Num() == 0 || DialogueTexts.Num() != Dialogues.Num())
		return;

	FMath::SRandInit(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	int ran = FMath::RandRange(0, Dialogues.Num() - 1);
	CurrentDialogue = new FString(GetDialogText(DialogueTexts[ran]));
	UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), Dialogues[ran], GetActorLocation());
	
}