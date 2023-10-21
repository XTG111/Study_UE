// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;

//c++实现自定义事件
DECLARE_EVENT(ATMagicProjectile, AutoSpawn)

UCLASS()
class PHONETEST1_API ATMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//球体体积，粒子的模型放置
	UPROPERTY(VisibleAnywhere)
		USphereComponent* SphereComp;

	//抛体运动组件
	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MovementComp;

	//粒子的状态，后续可以集合在粒子组件中
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* EffectComp;

	//添加粒子效果
	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* HitFX;

	//受到粒子攻击扣血
	UFUNCTION()
		void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//粒子碰撞后销毁
	UFUNCTION()
		void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normallpulse, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AutoSpawn ObjToSpawn;

};
