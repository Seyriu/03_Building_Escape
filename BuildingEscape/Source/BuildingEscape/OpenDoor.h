// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h" 

///Makes an entire new class, a new type. Event, broadcast.
///Declare Serializable, one to many(Broadcast), something else is going to work it for him (Blueprint).
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpen;
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnClose;
private:
	//UPROPERTY(VisibleAnywhere)
	//float OpenAngle = -90.0f;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate=nullptr;
	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.f;

	AActor* Owner=nullptr;
	//Returns total mass in kg
	float GetTotalMassOfActorsOnPlate();
};
