// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProject4.h"
#include "Fuky.h"
#include "Constants.h"


AFuky::AFuky()
{
	this->OrcName = new FString("Fluky: ");
	this->IsMoving = false;
}

AWaypoint* AFuky::GetNextWaypoint()
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

		text = new FString("Error Condition: Moving to Predicted Waypoint - ");
		text->Append(predictedLocation->Name);
		this->sayDialog(text);

		return predictedLocation;
	}

	this->sayDialog(new FString("Error Condition: Moving to default Location."));
	return this->CurrentLocation->WalkNeighbors[0];
}


/*
Given the predicted location of the Player and the List of
older player locations get the waypoint that probably gives the
next path
*/
AWaypoint* AFuky::PredictedDestinationFor(AWaypoint* predictedWaypoin)
{
	AWaypoint* ret = NULL;

	if (!FinalDestination)
		return predictedWaypoin;

	// Get all neighbors for the predicted waypoint.
	TArray<AWaypoint*> list = GetAllNeighbors(predictedWaypoin);

	float minDist = 999999999999999999.0f;

	// Chose a random number between 0 and list.Num()
	int random = list.Num() / 2;
	ret = list[random];

	return predictedWaypoin;
}

void AFuky::MoveOrc()
{
	this->sayDialog(&((new FString("Responding to transportation type: "))->Append(*(TransportUsed.Last(0)))));
	this->CurrentLocation = GetNextWaypoint();
	

	UNavigationSystem::SimpleMoveToLocation(GetController(), this->CurrentLocation->GetActorLocation());
	this->IsMoving = true;
}


AWaypoint* AFuky::NextWaypointTowards(AWaypoint* destination)
{
	FString* text = NULL;

	if (CurrentLocation == destination)
	{
		return destination;
	}

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
		sayDialog(text);
		return shortestPath[1];
	}

	// If there is no shortest path from CurrentLocation to Destination (which should not happen!)
	// then return null.
	sayDialog(new FString("Error Condition: No Path found!"));
	return NULL;
}

void AFuky::PrintPath(TArray<AWaypoint*> path)
{
	FString* text = new FString("Printing Path with ");
	text->AppendInt(path.Num());
	text->Append(" hops: ");

	for (int i = 0; i < path.Num(); ++i)
	{
		text->Append(path[i]->Name);
		text->Append(", ");
	}

	sayDialog(text);
}


FString AFuky::GetDialogText(FString* text)
{
	FString* retVal = new FString("");
	retVal->Append(*this->OrcName);
	retVal->Append(*text);

	return *retVal;
}

void AFuky::sayDialog(FString* text)
{
	if (Constants::SHOW_DEBUG_LOGS)
		GEngine->AddOnScreenDebugMessage(-1, DEBUG_STATEMENT_TIMEOUT_SECS, FColor::Blue, this->GetDialogText(text));

	UE_LOG(YourLog, Log, TEXT("Fuky: %s"), *(*text));
}





void AFuky::SayDialogue()
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