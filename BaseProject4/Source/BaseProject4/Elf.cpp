// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProject4.h"
#include "Elf.h"
#include "Constants.h"

// Sets default values
AElf::AElf()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DialogueTexts.Add(new FString("Elf: I must not fail!"));
}

FVector AElf::GetCurrentLocation()
{
	if (!CurrentLocation)
		return this->GetActorLocation();

	return this->CurrentLocation->GetActorLocation();
}

void AElf::SetCurrentWaypointAsWaypoint(AWaypoint* waypont)
{
	if (!CurrentLocation)
		CurrentLocation = waypont;
}

AWaypoint* AElf::GetCurrentLocationAsWaypoint()
{
	if (!CurrentLocation)
		return NULL;

	return this->CurrentLocation;
}

/// After running this, except for the first Waypoint, all should be disabled in the map. If not there's
///  something wrong with the way they are connected.
void AElf::DisableAllWaypoints()
{
	// for all the Waypoints that were initially enabled, set them to false.
	for (int i = 0; i < EnabledWaypoints.Num(); ++i)	
		EnabledWaypoints[i]->enabled = false;	

	// Clear the List of Waypoints.
	EnabledWaypoints.Empty();
}

TArray<AWaypoint*> AElf::GetAllNeighbors(AWaypoint* waypoint)
{
	TArray<AWaypoint*> ret;

	if (!waypoint)
		return ret;

	for (int i = 0; i < waypoint->WalkNeighbors.Num(); ++i)
		ret.Add(waypoint->WalkNeighbors[i]);

	for (int i = 0; i < waypoint->CartNeighbors.Num(); ++i)
		ret.Add(waypoint->CartNeighbors[i]);

	for (int i = 0; i < waypoint->FlyNeighbors.Num(); ++i)
		ret.Add(waypoint->FlyNeighbors[i]);


	return ret;
}

/// Helper function..
void AElf::DisableAllNeighborsOf(AWaypoint* location, TArray<AWaypoint*> visitedWaypoints)
{
	if (!location)
		return;

	if (visitedWaypoints.Contains(location))
		return;

	//visitedWaypoints.Add(location);

	location->enabled = false;

	TArray<AWaypoint*> list = GetAllNeighbors(location);

	
	for (int i = 0; i < list.Num(); ++i)
	{
		//if (!list[i]->enabled)
		//	continue;

		TArray<AWaypoint*> temp;
		temp.Append(visitedWaypoints);
		temp.Add(location);

		DisableAllNeighborsOf(list[i], temp);
	}


}

// Helper function..
void AElf::EnableListOfWaypoints(TArray<AWaypoint*> waypoints)
{
	for (int i = 0; i < waypoints.Num(); ++i)
	{
		bool setEnabled = true;

		for (int j = 0; j < ListOfOtherOrcs.Num(); ++j)
			if (waypoints[i] && ListOfOtherOrcs[j]->CurrentLocation)
				if(ListOfOtherOrcs[j]->CurrentLocation != waypoints[i])
					setEnabled &= true;
				else
					setEnabled &= false;

		waypoints[i]->enabled = setEnabled;

		if (setEnabled)
		{
			EnabledWaypoints.Add(waypoints[i]);
		}
	}
}

void AElf::EnableSelectedWaypoints(const FString& transportType)
{
	LastTrasnportUsed = new FString(transportType);

	if (!this->CurrentLocation)
		return;

	// 1. Unselect all waypoints..
	DisableAllWaypoints();

	// 2. Depeding on the transport type, pull all Waypoints from CurrentLocation
	//    and set enabled to true for each in the list.
	if (transportType.Contains(T_TYPE_ROAD))
	{
		EnableListOfWaypoints(this->CurrentLocation->WalkNeighbors);
	}
	else if (transportType.Contains(T_TYPE_CART))
	{
		EnableListOfWaypoints(this->CurrentLocation->CartNeighbors);
	}
	else if (transportType.Contains(T_TYPE_FLY))
	{
		EnableListOfWaypoints(this->CurrentLocation->FlyNeighbors);
	}
	
}

AWaypoint* AElf::GetClosestWaypoint(const FVector& clickedLocation)
{
	AWaypoint* targetWaypoint = NULL;

	// Figure out which Waypoint was clicked..
	TArray<AWaypoint*> neighbors =  GetAllNeighbors(CurrentLocation);
	targetWaypoint = GetSelectedWaypoint(neighbors, clickedLocation);

	return targetWaypoint;
}

bool AElf::MoveToWaypoint(const FVector& clickedLocation)
{
	if (!this->CurrentLocation || totalDucats <= 0.0f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 55, FColor::Green, "CurrentLocation is Null!");

		return false;
	}

	AWaypoint* targetWaypoint = GetClosestWaypoint(clickedLocation);

	// If you have a waypoint clicked, move towards it
	// and set that waypoint as the new Current Location.
	if (targetWaypoint)
	{
		ReduceDucats();
		MoveToWaypoint(targetWaypoint);

		PrintLogMessage(new FString("-----------------------------------------------------------------------------------"));
		return true;
	}
		
	//GEngine->AddOnScreenDebugMessage(-1, 55, FColor::Green, "targetWaypoint is Null!");
	return false;
}

void AElf::ReduceDucats()
{
	if (LastTrasnportUsed)
	{
		if (LastTrasnportUsed->Equals(T_TYPE_ROAD))
			totalDucats -= CostToRoad;
		else if (LastTrasnportUsed->Equals(T_TYPE_CART))
			totalDucats -= costToCart;
		else
			totalDucats -= CostToFly;
	}

	
}

float AElf::CheckEndOfGame()
{
	if (this->IsMoving)
		return 0.0f;

	if (totalDucats <= 0.0f)
		return -1.0f;

	if (CurrentLocation == FinalDestination)
		return 1.0f;

	for (int i = 0; i < this->ListOfOtherOrcs.Num(); ++i)	
		if (!ListOfOtherOrcs[i]->IsMoving && this->CurrentLocation == ListOfOtherOrcs[i]->CurrentLocation)
			return -1.0f;
	
	return 0.0f;
}

float AElf::GetTotalDucats()
{
	return this->totalDucats;
}

void AElf::SayDialogue()
{
	// Play one of Dialogues
	// Set CurrentDialogue as the newly selected dialogue..

	if (Dialogues.Num() == 0 || DialogueTexts.Num() != Dialogues.Num())
		return;

	FMath::SRandInit(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	int ran = FMath::RandRange(0, Dialogues.Num() - 1);
	CurrentDialogue = DialogueTexts[ran];
	UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), Dialogues[ran], GetActorLocation());
}


void AElf::ResetDoneSayingDialogue()
{
	doneSayingDialogue = false;
}

FString AElf::GetDialog()
{
	CurrentDialogue = NULL;

	if (doneSayingDialogue)
		return "";
	
	FMath::SRandInit(UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	int ran = FMath::RandRange(0, ListOfOtherOrcs.Num());
		
	if (ran < ListOfOtherOrcs.Num())
	{
		ListOfOtherOrcs[ran]->CurrentDialogue = NULL;

		ListOfOtherOrcs[ran]->SayDialogue();
		if (ListOfOtherOrcs[ran]->CurrentDialogue) 
		{
			doneSayingDialogue = true;
			return *ListOfOtherOrcs[ran]->CurrentDialogue;
		}
	}
	else
	{
		SayDialogue();
		if (CurrentDialogue)
		{
			doneSayingDialogue = true;
			return *CurrentDialogue;
		}
			
	}

	/*
	if (ran == 0)
		ret = new FString("Elf : I must not fail!");
	else if (ran == 1)
		ret = new FString("Tracker : I will find you.. Arggg..! And Skinn you alive! Boil your bones and feast on your flesh!");
	else if (ran == 2)
		ret = new FString("Trapper : It's only a matter of time.. Patience is the key to outsmart you little one.");
	else if (ran == 3)
		ret = new FString("Protherer : Must help master.. Must bring shiney shiney!");
	else
		ret = new FString("Fuky : Wagaaa...!! Ooozziee.. Arkenssssoo.. Onee..!");
	*/
	
	return "";
}

// Helper function..
AWaypoint* AElf::GetSelectedWaypoint(TArray<AWaypoint*> waypoints, const FVector& clickedLocation)
{
	AWaypoint* targetWaypoint = NULL;
	float minDist = 100.0f;
	float tempDist = 0.0f;
	
	for (int i = 0; i < waypoints.Num(); ++i)
		if (waypoints[i]->enabled && waypoints[i]->GetActorLocation().Equals(clickedLocation, 50.0))
		{
			tempDist = FVector::Dist(waypoints[i]->GetActorLocation(), clickedLocation);
			if (minDist > tempDist)
			{
				targetWaypoint = waypoints[i];
				minDist = tempDist;
			}
		}
	
	return targetWaypoint;
}

void AElf::MoveToWaypoint(AWaypoint* location)
{
	this->CurrentLocation = location;
	DisableAllWaypoints();

	UNavigationSystem::SimpleMoveToLocation(GetController(), location->GetActorLocation());
	IsMoving = true;
	ReadNarrative = false;
}

bool AElf::CheckIfReachedLocation()
{
	if (!this->CurrentLocation)
		return false;

	if (FVector::Dist(this->GetActorLocation(), this->CurrentLocation->GetActorLocation()) <= 200) 
	{
		IsMoving = false;

		if (CurrentLocation->AudioComp && !ReadNarrative)
		{
			UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), CurrentLocation->AudioComp,
				CurrentLocation->GetActorLocation());	
			ReadNarrative = true;
		}
	}

	return !IsMoving;
}

// Called when the game starts or when spawned
void AElf::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElf::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AElf::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AElf::PrintLogMessage(FString* message)
{
	if (Constants::SHOW_DEBUG_LOGS)
		GEngine->AddOnScreenDebugMessage(-1, 50, FColor::Cyan, *message);

	UE_LOG(YourLog, Log, TEXT("Elf: %s"), *(*message));
}