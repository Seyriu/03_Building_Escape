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
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

//Find (assumed) attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	///look for attached Physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		///Physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: No Physics handle found in %s"), *GetOwner()->GetName())
	}
}

//Setup (assumed) attached input component(only appears at run time)
void UGrabber::SetupInputComponent()
{
	///look for attached Input Component (only appears at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component was found in %s"), *GetOwner()->GetName())
			///bind the input action. 'This' is the grabber component. UGrabber is the class, Grab is the method. passed by reference.
			///We provide the method that's going to play after the key press.
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: No Input component found in %s"), *GetOwner()->GetName())
	}
}

//line trace and see if we reach any actor with physics body collision channel set
void UGrabber::Grab()
{
	///LINE TRACE and see if we reach any actor with physics body collision channel set
	FHitResult LineTraceHit = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = LineTraceHit.GetComponent();//UPrimitiveComponent* type
	auto ActorHit = LineTraceHit.GetActor();
	//if we hit something
	if (ActorHit) ///same as ActorHit != nullptr
	{
		//attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			false///allow rotation
		);
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	///if the physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation, 
			OUT PlayerViewPointRotation  
		);
		FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector()*Reach);
		PhysicsHandle->SetTargetLocation(LineTraceEnd);///move the object that we're holding (set in front of my pawn as of the debug line)
	}
}


//Draw a red trace in the world to visualize where the pawn can reach
void UGrabber::DrawReachDebugLine(FVector PlayerViewPointLocation, FVector LineTraceEnd)
{
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,///we don't want the line to persist over time. The line is redrawn every frame.
		0.f, ///Duration of the line in case we wanted it to stay up after the frame.
		0.f,
		5.f
	);
	return;
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get player(pawn) viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, //The word OUT was defined at the top to do nothing, 
		OUT PlayerViewPointRotation  //just to remind us that the parameters are passed by reference
	);
	///When a rotator returns a vector, it will be as if the position would be one unit away from the point of origin.
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector()*Reach);
	DrawReachDebugLine(PlayerViewPointLocation, LineTraceEnd);
	///Setup instance with query parameters.We want a simple trace, not a heavy complex one with all the details, hence the false.
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());///we ignore ourselves because the ray-cast starts in our character and would always be colliding
	FHitResult LineTraceHit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),///Filtering based on what type of object I hit
		TraceParameters
	);
	return LineTraceHit;
}
