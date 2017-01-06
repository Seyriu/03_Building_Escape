// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

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
	//find the owning actor
	Owner= GetOwner();
	if (!PressurePlate) { UE_LOG(LogTemp, Warning, TEXT("Pressure Plate property value not entered in the OpenDoor component")); }
}

void UOpenDoor::OpenDoor()
{	
	//create a rotator
	//FRotator NewRotator = FRotator(0.0f, OpenAngle, 0.0f);
	//set the door rotation
	//Owner->SetActorRotation(NewRotator);
	OnOpenRequest.Broadcast();
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
	if (GetTotalMassOfActorsOnPlate()>30.f) //TODO Change 50.f into a property
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

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	//Find all of the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//iterate through them adding all their masses
	for (const auto& OverlappingActor : OverlappingActors)
	{
		UE_LOG(LogTemp,Warning,TEXT("Overlapping Actor: %s"),*(OverlappingActor->GetName()))
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

