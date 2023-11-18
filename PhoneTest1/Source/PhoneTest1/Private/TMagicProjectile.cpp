// Fill out your copyright notice in the Description page of Project Settings.


#include "TMagicProjectile.h"
#include "Components\SphereComponent.h"
#include "Particles\ParticleSystemComponent.h"
#include "GameFramework\ProjectileMovementComponent.h"
#include "TCharacterComponent.h"
#include "Kismet\GameplayStatics.h"

// Sets default values
ATMagicProjectile::ATMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	SphereComp->SetCollisionProfileName("Projectile");

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATMagicProjectile::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ATMagicProjectile::OnActorHit);

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Content/StarterContent/Particles/P_Fire.P_Fire_C"));
	//if (ParticleAsset.Succeeded())
	//{
	//	HitFX = ParticleAsset.Object;
	//}
}

// Called when the game starts or when spawned
void ATMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	ObjToSpawn.Broadcast();
	
}

void ATMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator()) {
		//获取OtherActor的CharacterComp属性
		UTCharacterComponent* CharacterComp = Cast<UTCharacterComponent>(OtherActor->GetComponentByClass(UTCharacterComponent::StaticClass()));
		if (CharacterComp) {
			CharacterComp->ApplyHealthChange(-20.0f);
			Destroy();
		}
	}
}

void ATMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normallpulse, const FHitResult& SweepResult)
{
	//寻找粒子效果	
	FVector Loc = this->GetActorLocation();
	FRotator Rot = this->GetActorRotation();

	if (OtherActor && OtherActor != GetInstigator()) {
		//UGameplayStatics::SpawnEmitterAtLocation(this->EffectComp, HitFX, Loc, Rot);
		Destroy();
	}
}

// Called every frame
void ATMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

