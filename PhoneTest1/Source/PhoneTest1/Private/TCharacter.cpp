// Fill out your copyright notice in the Description page of Project Settings.


#include "TCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "TCharacterComponent.h"
#include "Kismet\GameplayStatics.h"

// Sets default values
ATCharacter::ATCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//the view set
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	//move under view vector
	SpringArmComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;//need #include "GameFramework/CharacterMovementComponent.h"
	bUseControllerRotationYaw = false;

	CharacterComp = CreateDefaultSubobject<UTCharacterComponent>("CharacterComp");

}

// Called when the game starts or when spawned
void ATCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CharacterComp->OnHealthChanged.AddDynamic(this, &ATCharacter::isDied);
}

void ATCharacter::isDied(AActor* Instigetor, UTCharacterComponent* OtherComp, float Health, float Delta)
{
	if (Health <= 0.0f && Delta < 0.0f) {
		//血量为0，不能控制
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		//当死亡后，将角色隐藏，避免阻挡粒子运动方向
		ACharacter* myPawn = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		myPawn->SetActorEnableCollision(false);
	}
}

// Called every frame
void ATCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//move forward or back func  change Yaw
void ATCharacter::MoveForward(float value) {
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), value);
}

//move right or left func change Y axis
void ATCharacter::MoveRight(float value) {
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

//利用射线检测，使得攻击粒子落点与准星相同
FVector ATCharacter::GetTrue_Rotate()
{
	//获取射线检测的点,从相机位置沿着视线方向一定距离
	FVector CrossFair_Location = CameraComp->GetComponentLocation() + GetControlRotation().Vector() * 5000;

	//接下来就是利用这条线去进行射线检测，如果与物体有碰撞，那么返回新的这个点，否则不变
	//Hit存储检测结果
	FHitResult Hit;

	//设置检测哪些物体
	FCollisionQueryParams Params;

	//不要检测自己
	Params.AddIgnoredActor(this);

	//设置检测的物体碰撞结束条件
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	//去判断是否碰撞了物体
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraComp->GetComponentLocation(), CrossFair_Location, ObjectQueryParams, Params);
	if (bBlockingHit) {
		CrossFair_Location = Hit.ImpactPoint;
	}

	return CrossFair_Location;
}

void ATCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	//获取粒子落点位置
	FVector FinalLoc = GetTrue_Rotate();
	//设置发射位置
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	//设置发射朝向
	FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, FinalLoc);

	//利用Spawn生成粒子
	FTransform SpawnTM = FTransform(FinalRot, HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	//依据传入的类生成粒子
	GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
}

void ATCharacter::PrimaryAttack()
{
	SpawnProjectile(ProjectileClass);
}

void ATCharacter::PrimaryAttack_Anim()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ATCharacter::PrimaryAttack, 0.2f);
}

// Called to bind functionality to input
void ATCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//bind move forward or back
	PlayerInputComponent->BindAxis("MoveForward", this, &ATCharacter::MoveForward);

	//bind move right or left
	PlayerInputComponent->BindAxis("MoveRight", this, &ATCharacter::MoveRight);

	//view yaw rotate
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	//view pitch rotate
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	//primaryAttack
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ATCharacter::PrimaryAttack_Anim);

}



