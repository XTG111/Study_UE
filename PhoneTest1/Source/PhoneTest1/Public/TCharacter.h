// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTCharacterComponent;

UCLASS()
class PHONETEST1_API ATCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComp;

	FTimerHandle TimerHandle_PrimaryAttack;

	//攻击动画和粒子模板
	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		UTCharacterComponent* CharacterComp;

	//使用虚初始化组件绑定死亡事件
	virtual void PostInitializeComponents();

	UFUNCTION(BlueprintCallable)
	void isDied(AActor* Instigetor, UTCharacterComponent* OtherComp, float Health, float Delta);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);

	void MoveRight(float value);

	void PrimaryAttack_Anim();
	
	FVector GetTrue_Rotate();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
	void PrimaryAttack();
};
