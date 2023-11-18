// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "TMarchantNPC.h"

// Sets default values
ATMarchantNPC::ATMarchantNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATMarchantNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATMarchantNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATMarchantNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

