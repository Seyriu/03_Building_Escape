// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	ActorThatOpens=GetWorld()->GetFirstPlayerController()->GetPawn();
	//find the owning actor
	Owner= GetOwner();
}

void UOpenDoor::OpenDoor()
{	
	//create a rotator
	FRotator NewRotator = FRotator(0.0f, OpenAngle, 0.0f);
	//set the door rotation
	Owner->SetActorRotation(NewRotator);
}

void UOpenDoor::CloseDoor()
{
	//set the door rotation using a rotator
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// poll the TriggerVolume every frame
	//if the ActorThatOpens is in the volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime= GetWorld()->GetRealTimeSeconds(); //GetRealTimeSeconds counts the seconds since the (Game) world was created
	}
	//Check if it is time to close the door
	if ((GetWorld()->GetRealTimeSeconds()-LastDoorOpenTime)>DoorCloseDelay)
	{
		CloseDoor();
	}
	
}

