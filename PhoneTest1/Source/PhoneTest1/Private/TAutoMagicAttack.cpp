// Fill out your copyright notice in the Description page of Project Settings.


#include "TAutoMagicAttack.h"
#include "TCharacter.h"
#include <Kismet/KismetMathLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"

// Sets default values
ATAutoMagicAttack::ATAutoMagicAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>("Arrow");

}

// Called when the game starts or when spawned
void ATAutoMagicAttack::BeginPlay()
{
	Super::BeginPlay();
	SpawnMagicProjectile();

}

void ATAutoMagicAttack::SpawnMagicProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<ATMagicProjectile>(ProjectileClass, Matrix, SpawnParams);
}

FRotator ATAutoMagicAttack::SpawnRotation()
{
	FVector PawnLoc = UGameplayStatics::GetPlayerPawn(GetWorld(),0)->GetActorLocation();
	FVector ArrowLoc = this->GetActorLocation();
	FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(ArrowLoc,PawnLoc);
	return FinalRot;
}

// Called every frame
void ATAutoMagicAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rot = SpawnRotation();
	Matrix = FTransform(Rot, GetActorLocation());
}

