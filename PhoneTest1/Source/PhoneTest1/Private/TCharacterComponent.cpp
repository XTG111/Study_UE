// Fill out your copyright notice in the Description page of Project Settings.


#include "TCharacterComponent.h"

// Sets default values for this component's properties
UTCharacterComponent::UTCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	Health = 100.0;
}


// Called when the game starts
void UTCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UTCharacterComponent::ApplyHealthChange(float Delta)
{
	if (Health <= 100.0f && Delta < 0.0f) {
		Health += Delta;
		OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	}
	if (Health < 100.0f && Delta > 0.0f) {
		Health += Delta;
		OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	}
	return true;
}

bool UTCharacterComponent::isAlive()
{
	if (Health > 0) {
		return true;
	}
	else {
		return false;
	}
}

