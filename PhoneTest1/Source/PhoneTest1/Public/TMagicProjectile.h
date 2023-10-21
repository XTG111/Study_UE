// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;

//c++ʵ���Զ����¼�
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

	//������������ӵ�ģ�ͷ���
	UPROPERTY(VisibleAnywhere)
		USphereComponent* SphereComp;

	//�����˶����
	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MovementComp;

	//���ӵ�״̬���������Լ��������������
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* EffectComp;

	//�������Ч��
	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* HitFX;

	//�ܵ����ӹ�����Ѫ
	UFUNCTION()
		void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//������ײ������
	UFUNCTION()
		void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normallpulse, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AutoSpawn ObjToSpawn;

};
