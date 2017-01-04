// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called when the game starts
	virtual void BeginPlay() override;	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	//Returns the coordinates of the point indicating where the grabbed items will be held

private:
	//how far ahead of the player can  cm.
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle=nullptr;
	UInputComponent* InputComponent=nullptr;
	//Ray-Cast and grab what's in reach
	void Grab();
	//Release what the pawn is holding
	void Release();
	//Find (assumed) attached physics handle
	void FindPhysicsHandleComponent();
	//Setup (assumed) attached input component (only appears at run time)
	void SetupInputComponent();
	//Draw a red trace in the world to visualize where the pawn can reach
	void DrawReachDebugLine(FVector, FVector);
	//Return hit for the first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	//Returns current end of reach line
	FVector GetReachLineStart();
	//Returns current end of reach line
	FVector GetReachLineEnd();
};
