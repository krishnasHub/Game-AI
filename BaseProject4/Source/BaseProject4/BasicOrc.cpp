// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProject4.h"
#include "BasicOrc.h"
#include "Constants.h"


// Sets default values
ABasicOrc::ABasicOrc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerLocationCountAnalysed = 1;

}

// Called when the game starts or when spawned
void ABasicOrc::BeginPlay()
{
	Super::BeginPlay();
	this->IsMoving = false;

}

// Called every frame
void ABasicOrc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicOrc::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

AWaypoint* ABasicOrc::GetCurrentLocation()
{
	return CurrentLocation;
}

void ABasicOrc::MoveOrc()
{
	check(0 && "You must override this Method in your Orc.");
}

AWaypoint* ABasicOrc::GetNextWaypoint()
{
	check(0 && "You must override this Method in your Orc.");
	return NULL;
}

void ABasicOrc::CheckIfOrcReachedWaypoint()
{
	if (this->IsMoving)
	{
		if (this->GetActorLocation().Equals(this->CurrentLocation->GetActorLocation(), 200.5f))
		{
			this->IsMoving = false;
			SayDialogue();
		}
	}
	
}

void ABasicOrc::SayDialogue()
{
	check(0 && "You must override this Method in your Orc.");
	// Play one of Dialogues
	// Set CurrentDialogue as the newly selected dialogue..
}

/* This function will be called from the Blueprint once every 5 times */
void ABasicOrc::AddPlayerLocation(AWaypoint* lastKnownLocation)
{
	if (!lastKnownLocation)
		return;

	PlayerLocations.Add(lastKnownLocation);
}

void ABasicOrc::AddTransportUsed(const FString& transportType)
{
	TransportUsed.Add(new FString(transportType));
}

void ABasicOrc::PrintWaypoints(TArray<AWaypoint*> waypoints)
{
	FString* text = new FString("Found a total of ");
	text->AppendInt(waypoints.Num());
	text->Append(" probable neighbors..: ");
	for (int i = 0; i < waypoints.Num(); ++i)
	{
		text->Append(waypoints[i]->Name);
		text->Append("(");
		text->Append(FString::SanitizeFloat(waypoints[i]->weight));
		text->Append(")");
		if(i != (waypoints.Num() - 1))
			text->Append(", ");
	}
	PrintLogMessage(text);
}


TArray<AWaypoint*> ABasicOrc::GetAllNeighbors(AWaypoint* waypoint)
{
	TArray<AWaypoint*> ret;

	for (int i = 0; i < waypoint->WalkNeighbors.Num(); ++i)
		ret.Add(waypoint->WalkNeighbors[i]);

	for (int i = 0; i < waypoint->CartNeighbors.Num(); ++i)
		ret.Add(waypoint->CartNeighbors[i]);

	for (int i = 0; i < waypoint->FlyNeighbors.Num(); ++i)
		ret.Add(waypoint->FlyNeighbors[i]);


	return ret;
}



/* 
A* to find the shortest path from source to destination, keeping in mind
that you can travel to the same location - Travel in circles if nt careful!
*/
TArray<AWaypoint*> ABasicOrc::GetShortestPathBetween(AWaypoint* source, AWaypoint* destination, TArray<AWaypoint*> travelledList)
{
	TArray<AWaypoint*> ret;
	int Hops = 982043896;
	FString* text = NULL;

	/*
	// Uncomment this only when debugging..

	text = new FString("From ");
	text->Append(source->Name);
	text->Append(" -> ");
	text->Append(destination->Name);
	PrintLogMessage(text);
	*/

	// the 3 terminating conditions for this recursion..

	// 1. If I've already visited you, then return empty..
	if (travelledList.Contains(source))
	{
		/*
		// Uncomment this only when debugging..

		text = new FString("Already been to ");
		text->Append(source->Name);
		PrintLogMessage(text);
		*/

		return ret;
	}
		

	// 2. If the source is the same as the dest, return the source/destination.
	if (source == destination)
	{
		/*
		// Uncomment this only when debugging..

		text = new FString("Finally been to ");
		text->Append(source->Name);
		PrintLogMessage(text);
		*/

		ret.Add(source);
		return ret;
	}

	
	// 3. Check if any of the neighbors are the destination..
	TArray<AWaypoint*> neighbors = GetAllNeighbors(source);
	for (int i = 0; i < neighbors.Num(); ++i)
		if (neighbors[i] == destination)
		{
			/*
			// Uncomment this only when debugging..

			text = new FString("Found in neighbors, ");
			text->Append(source->Name);
			text->Append(" to ");
			text->Append(destination->Name);
			PrintLogMessage(text);
			*/

			ret.Add(source);
			ret.Add(destination);
			return ret;
		}
	
	// PrintLogMessage(new FString("Didnt find any neighbors, looping further!"));

	// 4. Get all shortest paths from neighbors..
	// So that the shortest path from source to destination is the path from source
	// to the shortest path from neighbor's shortest path to the destination.

	// 4.1. Get all the shortest paths for all teh neigbors of source.
	// 4.1.1. neighbors contains the sortest List of Waypoints where the first element is closest
	//        to the destination than the last.
	//        So when we loop for all the elements in neighbors, we are essentially looking for the closest waypoint towards
	//        destination first, followed by the second closest and so on..
	TArray<TArray<AWaypoint*>> listOfPaths;
	for (int i = 0; i < neighbors.Num(); ++i)
	{
		TArray<AWaypoint*> newTravelledList;
		newTravelledList.Append(travelledList);
		newTravelledList.Add(source);
		TArray<AWaypoint*> temp = GetShortestPathBetween(neighbors[i], destination, newTravelledList);
		if (temp.Num() != 0 && temp.Last() == destination) 
		{
			/*
			// Uncomment this only when debugging..

			text = new FString("Found a path from ");
			text->Append(source->Name);
			text->Append(" through ");
			text->Append(temp[0]->Name);
			PrintLogMessage(text);
			*/

			listOfPaths.Add(temp);
		}
	}

	// 4.2. Find the path with the shortest hops for each shortest path from each neighbor.
	for (int i = 0; i < listOfPaths.Num(); ++i)
		if (listOfPaths[i].Num() < Hops)
		{
			Hops = listOfPaths[i].Num();
			ret = listOfPaths[i];
		}
	
	// 5. Append source to the list of shortest path found.
	TArray<AWaypoint*>retVal;
	retVal.Add(source);
	retVal.Append(ret);


	return retVal;
}

// Except for the last entry, remove the rest from this List.
void ABasicOrc::FilterTransportUsed()
{
	TArray<FString*> newArray;
	newArray.Add(TransportUsed.Last());
	TransportUsed = newArray;
}

TArray<AWaypoint*> ABasicOrc::RemoveWaypointsHavingOrcs(TArray<AWaypoint*> neighbors)
{
	TArray<AWaypoint*> ret;

	// Remove waypoonts where Orcs are standing right now..
	for (int i = 0; i < neighbors.Num(); ++i)
		if (!IsWaypointHighlyUnlikely(neighbors[i]))
			ret.Add(neighbors[i]);

	return ret;
}

AWaypoint* ABasicOrc::PredictPlayerPosition()
{
	AWaypoint* retVal;
	FString* text = new FString("Looking for prediction..");
	PrintLogMessage(text);

	// 1. First see if the player is visible.
	if (this->PlayerLocations.Num() == PlayerLocationCountAnalysed)
	{
		//text = new FString("Player Visible!: PlayerLocations.Num = ");
		//text->AppendInt(this->PlayerLocations.Num());
		//text->Append("; PlayerLocationCountAnalysed = ");
		//text->AppendInt(PlayerLocationCountAnalysed);
		PrintLogMessage(new FString("Player Visible.."));

		PlayerLocationCountAnalysed++;
		CanSeePlayer = true;

		FilterTransportUsed();
		return this->PlayerLocations.Last();
	}

	
	AWaypoint* startPoint = this->PlayerLocations.Last();

	// 2.1. Get all poosible neighbors..
	TArray<AWaypoint*> allProbableNeighbors = ClearDuplicateWaypoints(GetAllProbableNeighbors(startPoint, this->TransportUsed));

	// 2.2. Remove Waypoints that have an Orc on it right now.
	// ****** Ignore this for now ******
	TArray<AWaypoint*> moreProbableNeighbors = RemoveWaypointsHavingOrcs(allProbableNeighbors);
	
	// 3.1. Assign Weights to all neighbors..
	AssignWeights(moreProbableNeighbors);

	// Print all neighbors with weights..
	PrintWaypoints(moreProbableNeighbors);

	// 3.2. If there was only 1 Waypoint shortlisted, add it to List of Known Waypoints!
	EnrichPlayerLocations(moreProbableNeighbors);

	// 4. Pick the neighbor with the maximum weightage..
	retVal = GetWaypointWithMaxWeight(moreProbableNeighbors);

	return retVal;
}

// Enrich the PlayerLocations based on more stats
void ABasicOrc::EnrichPlayerLocations(TArray<AWaypoint*> allProbableNeighbors)
{
	// If there's only 1 possible Waypoint tha the Orc can be in, add that to PlayerLocations
	if (allProbableNeighbors.Num() == 1)
	{
		this->PlayerLocations.Add(allProbableNeighbors[0]);
		PlayerLocationCountAnalysed++;
		TransportUsed.Empty();
	}
}
/*
	The most important function
	Measures the probability/weightage of each neighbor based on the 4 conditions.
*/
void ABasicOrc::AssignWeights(TArray<AWaypoint*> neighbors)
{
	// 1. Condition 1: Look for the neighbor that's farthest from Orcs.
	AssignWeightBasedOnOrcLocality(neighbors);

	// 2. Condition 2: Look for the neighbor that has more outlets to other neighbors.
	AssignWeightBasedOnEgress(neighbors);

	// 3. Condition 3: Look for the neighbor that has more outlets that tke it further than others.
	AssignWeightBasedOnEgressRange(neighbors);

	// 4. Condition 4: Look for locations that are closer to the Final destination.
	AssignWeightBasedOnFinalDestination(neighbors);
}

void ABasicOrc::AssignWeightBasedOnFinalDestination(TArray<AWaypoint*> neighbors)
{
	TArray<float> distArray;

	for (int i = 0; i < neighbors.Num(); ++i)
	{
		float diff = FVector::Dist(neighbors[i]->GetActorLocation(), FinalDestination->GetActorLocation());
		distArray.Add(diff);
	}

	float max_dist = GetMaxOf(distArray);

	for (int i = 0; i < neighbors.Num(); ++i)
	{
		neighbors[i]->weight += FINAL_DEST_STRENGTH * (1.0f - (distArray[i] / max_dist));
	}

}

void ABasicOrc::AssignWeightBasedOnEgressRange(TArray<AWaypoint*> neighbors)
{
	TArray<float> egressRangeValues;
	FString* text = NULL;

	// Get the sum of distances from each neighbor to its neighbors.
	// This is a weighted sum giving more value to longer distances by foot, lesser to longer distances by cart
	// and even lesser to longer distances by flying.
	for (int i = 0; i < neighbors.Num(); ++i)
	{
		AWaypoint* neighbor = neighbors[i];
		TArray<AWaypoint*> list;
		float sumDist = 0.0f;

		list = neighbor->WalkNeighbors;
		for (int j = 0; j < list.Num(); ++j)		
			sumDist += EGRESS_RANGE_FACTOR_ROAD * FVector::Dist(list[j]->GetActorLocation(), neighbor->GetActorLocation());

		list = neighbor->CartNeighbors;
		for (int j = 0; j < list.Num(); ++j)
			sumDist += EGRESS_RANGE_FACTOR_CART * FVector::Dist(list[j]->GetActorLocation(), neighbor->GetActorLocation());

		list = neighbor->FlyNeighbors;
		for (int j = 0; j < list.Num(); ++j)
			sumDist += EGRESS_RANGE_FACTOR_FLY * FVector::Dist(list[j]->GetActorLocation(), neighbor->GetActorLocation());
		
		egressRangeValues.Add(sumDist);
	}
		
	// Get the maximum of the egress Range.
	float max_EgressRange = GetMaxOf(egressRangeValues);

	// the weight is the fraction of egressRange for each wrt to the local maxima of the Egress Range.
	for (int i = 0; i < neighbors.Num(); ++i)
		neighbors[i]->weight += RANGE_STRENGTH * egressRangeValues[i] / max_EgressRange;

	text = new FString("Egress Range for each waypoint");
	for (int i = 0; i < egressRangeValues.Num(); ++i)
	{
		text->Append(neighbors[i]->Name);
		text->Append("(");
		text->Append(FString::SanitizeFloat(egressRangeValues[i]));
		text->Append(")");
		text->Append("  ");
	}
	PrintLogMessage(text);
}

/*
	For each neighbor, find the number of other neighbors its connected to.
	If a neighbor is connected to more neighbors, more is its weight.
	TODO: Give more preference to neighbors with carts, then for neighbors with Road
	and finally for nieghbors with Eagles.
*/
void ABasicOrc::AssignWeightBasedOnEgress(TArray<AWaypoint*> neighbors)
{
	TArray<float> egressValues;
	FString* text = NULL;

	// Get the total outlets for each neighbor.
	for (int i = 0; i < neighbors.Num(); ++i)
		egressValues.Add(1.0f * (GetAllNeighbors(neighbors[i]).Num()));

	// Get the maximum outlets of all the neighbors.
	float max_Egress = GetMaxOf(egressValues);

	// The weight is the fraction of each neighbor's egress to the total egress.
	for (int i = 0; i < neighbors.Num(); ++i)
		neighbors[i]->weight += EGRESS_STRENGTH * egressValues[i] / max_Egress;
	
	text = new FString("Egress for each waypoint");
	for (int i = 0; i < egressValues.Num(); ++i)
	{
		text->Append(neighbors[i]->Name);
		text->Append("(");
		text->Append(FString::SanitizeFloat(egressValues[i]));
		text->Append(")");
		text->Append("  ");
	}
	PrintLogMessage(text);

}

/*
	For each Waypoint, calculates the avarage distance to the Orcs in the local vicinity.
	Then, for the list of neighbors, assigns more weightage to the neighbor that 
	is furthest from an orc in general and less weightage to the neighbor that is closer to an orc.
*/
void ABasicOrc::AssignWeightBasedOnOrcLocality(TArray<AWaypoint*> neighbors)
{
	TArray<float> distancesToOrcs;

	for (int i = 0; i < neighbors.Num(); ++i)
	{
		AWaypoint* neighbor = neighbors[i];

		// Get all Orcs that are in the general area of the waypoint..
		TArray<ABasicOrc*> allLocalOrcs = GetallLocalizedOrcs(neighbor);
		// If there are no orcs in this general area, don't worry about the waypoint, it's totaly safe!
		if (allLocalOrcs.Num() == 0)
		{
			distancesToOrcs.Add(0);
		}
		// If there are Orcs, then get the avarage distance of the waypoint
		// from all Orcs in the area and add that to the array distancesToOrcs
		else
		{
			float totalDist = 0;
			for (int j = 0; j < allLocalOrcs.Num(); ++j)
				totalDist += DistanceToOrc(allLocalOrcs[j], neighbor);

			// Avarage distance of waypoint from each Orc.
			totalDist = totalDist / allLocalOrcs.Num();

			distancesToOrcs.Add(totalDist);
		}
	}

	// Now distancesToOrcs holds the avarage distance of each waypoint in neighbors
	// to their local set of Orcs.
	// The waypoint with the lowest of these gets the maximum weightage value.
	float weight_orcDistance = GetMaxOf(distancesToOrcs);

	for (int i = 0; i < neighbors.Num(); ++i)
		neighbors[i]->weight += ORC_LOCALE_STRENGTH * (distancesToOrcs[i] / weight_orcDistance);

	
	// Uncomment this for debugging..

	FString* text = new FString("Distances for each waypoint to Orcs");
	for (int i = 0; i < distancesToOrcs.Num(); ++i) 
	{
		text->Append(neighbors[i]->Name);
		text->Append("(");
		text->Append(FString::SanitizeFloat(distancesToOrcs[i]));
		text->Append(")");
		text->Append("  ");
	}
	PrintLogMessage(text);
	
	
}

float ABasicOrc::GetMaxOf(TArray<float> floatValues)
{
	float ret = -1.0f;

	for (int i = 0; i < floatValues.Num(); ++i)
		if (floatValues[i] > ret)
			ret = floatValues[i];

	return ret;
}

int ABasicOrc::GetMaxOf(TArray<int> intValues)
{
	int ret = -1;

	for (int i = 0; i < intValues.Num(); ++i)
		if (intValues[i] > ret)
			ret = intValues[i];

	return ret;
}

TArray<ABasicOrc*> ABasicOrc::GetallLocalizedOrcs(AWaypoint* waypoint)
{
	TArray<ABasicOrc*> ret;

	for (int i = 0; i < ListOfOtherOrcs.Num(); ++i)
		if (FVector::Dist(ListOfOtherOrcs[i]->GetActorLocation(),
			waypoint->GetTargetLocation()) <= CLUSTER_DISTANCE)
			ret.Add(ListOfOtherOrcs[i]);

	return ret;
}

float ABasicOrc::DistanceToOrc(ABasicOrc* orc, AWaypoint* waypoint)
{
	if (orc && waypoint)
		return FVector::Dist(orc->GetActorLocation(), waypoint->GetActorLocation());

	return -1.0f;
}

AWaypoint* ABasicOrc::GetWaypointWithMaxWeight(TArray<AWaypoint*> waypoints)
{
	float maxWeight = -1.0f;
	AWaypoint* retVal = NULL;

	for (int i = 0; i < waypoints.Num(); ++i)	
		if (waypoints[i]->weight > maxWeight)
		{
			maxWeight = waypoints[i]->weight;
			retVal = waypoints[i];
		}
	

	return retVal;
}


TArray<AWaypoint*> ABasicOrc::GetAllProbableNeighbors(AWaypoint* start, TArray<FString*> transportUsed)
{
	TArray<AWaypoint*> ret;
	TArray<AWaypoint*> list;

	if(transportUsed.Num() == 0)
		return ret;

	list = GetProbableNeighbors(start, transportUsed[0]);
	if (list.Num() == 0)
		return ret;

	ret.Append(list);

	TArray<FString*> tempArray;
	tempArray.Append(transportUsed);
	tempArray.RemoveAt(0);

	if (tempArray.Num() != 0)
	{
		// Clean array ret if there are more neighbors to travel.
		ret.Empty();

		for (int i = 0; i < list.Num(); ++i)
		{
			ret.Append(GetAllProbableNeighbors(list[i], tempArray));
		}
	}

	return ret;
}

/*
	If the waypoint is exactly where the Orc is, or if the Orc is right next to it, it should return true
	***** Ignore this for now ****
*/
bool ABasicOrc::IsWaypointHighlyUnlikely(AWaypoint* neighbor)
{

	for (int i = 0; i < ListOfOtherOrcs.Num(); ++i)
	{
		ABasicOrc* theOrc = ListOfOtherOrcs[i];
		if (theOrc->CurrentLocation == neighbor)
			return true;

		//if (GetAllNeighbors(theOrc->CurrentLocation).Contains(neighbor))
		//	return true;
	}

	return false;
}

/*
	When there are duplicate Waypoints in the possible waypoints, 
	multiply its weight - If its showing up more than once, it has to be an important waypoint!
*/
TArray<AWaypoint*> ABasicOrc::ClearDuplicateWaypoints(TArray<AWaypoint*> waypoints)
{
	TArray<AWaypoint*> ret;
	int len = waypoints.Num();

	for (int i = 0; i < waypoints.Num(); ++i)
		if (!ret.Contains(waypoints[i]))
		{
			waypoints[i]->weight = 1.0f / len;
			ret.Add(waypoints[i]);
		}			
		else
		{
			waypoints[i]->weight *= 2.0f;
		}

	return ret;
}

TArray<AWaypoint*> ABasicOrc::GetProbableNeighbors(AWaypoint* start, FString* transportUsed)
{
	TArray<AWaypoint*> ret;

	if (transportUsed->ToLower().StartsWith(T_TYPE_ROAD))
	{
		for (int i = 0; i < start->WalkNeighbors.Num(); ++i)
			//if(!IsWaypointHighlyUnlikely(start->WalkNeighbors[i]))
				ret.Add(start->WalkNeighbors[i]);
	}
	else if (transportUsed->ToLower().StartsWith(T_TYPE_CART))
	{
		for (int i = 0; i < start->CartNeighbors.Num(); ++i)
			//if (!IsWaypointHighlyUnlikely(start->CartNeighbors[i]))
				ret.Add(start->CartNeighbors[i]);
	}
	else if (transportUsed->ToLower().StartsWith(T_TYPE_FLY))
	{
		for (int i = 0; i < start->FlyNeighbors.Num(); ++i)
			//if (!IsWaypointHighlyUnlikely(start->FlyNeighbors[i]))
				ret.Add(start->FlyNeighbors[i]);
	}

	return ret;
}


void ABasicOrc::PrintLogMessage(FString* message)
{
	if(Constants::SHOW_DEBUG_LOGS)
		GEngine->AddOnScreenDebugMessage(-1, DEBUG_STATEMENT_TIMEOUT_SECS, FColor::Green, *message);

	UE_LOG(YourLog, Log, TEXT("BasicOrc: %s"), *(*message));
}
