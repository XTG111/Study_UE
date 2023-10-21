// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TMagicProjectile.h"
#include "TAutoMagicAttack.generated.h"



class UArrowComponent;
class ATMagicProjectile;

UCLASS()
class PHONETEST1_API ATAutoMagicAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATAutoMagicAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<ATMagicProjectile> ProjectileClass;

	//设置一个箭头组件
	UPROPERTY(EditAnywhere)
		UArrowComponent* ArrowComp;

	UPROPERTY(EditAnywhere)
		ATMagicProjectile* MagicProjectile;

	UFUNCTION()
		void SpawnMagicProjectile();
	UFUNCTION()
		FRotator SpawnRotation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere)
	FRotator Rot;
	UPROPERTY(VisibleAnywhere)
		FTransform Matrix;

};
