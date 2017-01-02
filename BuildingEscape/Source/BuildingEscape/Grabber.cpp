// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	/// Get player(pawn) viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, //The word OUT was defined at the top to do nothing, 
		OUT PlayerViewPointRotation  //just to remind us that the parameters are passed by reference
	);
	///Log out to test 
	/*UE_LOG(LogTemp, Warning, TEXT("The grabber is watching from %s towards %s"), 
		*PlayerViewPointLocation.ToString(),
		*PlayerViewPointRotation.ToString()
	);*/

	///When a rotator returns a vector, it will be as if the position would be one unit away from the point of origin.
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector()*Reach);
	///Draw a red trace in the world to visualize
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd, 
		FColor(255,0,0),
		false,///we don't want the line to persist over time. The line is redrawn every frame.
		0.f, ///Duration of the line in case we wanted it to stay up after the frame.
		0.f,
		5.f
	);
	///Setup query parameters.We want a simple trace, not a heavy complex one with all the details, hence the false.
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());///we ignore ourselves because the ray-cast starts in our character and would always be colliding

	///Line Trace (ray-cast) out to reach distance (imagine Cyclop laser ray)
	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = LineTraceHit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("I've just hit the %s Physics Body!"),
			*(ActorHit->GetName())
		);
	}
	//see what we hit
}

