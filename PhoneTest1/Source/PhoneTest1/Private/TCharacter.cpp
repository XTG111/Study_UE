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
		//Ѫ��Ϊ0�����ܿ���
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		//�������󣬽���ɫ���أ������赲�����˶�����
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

//�������߼�⣬ʹ�ù������������׼����ͬ
FVector ATCharacter::GetTrue_Rotate()
{
	//��ȡ���߼��ĵ�,�����λ���������߷���һ������
	FVector CrossFair_Location = CameraComp->GetComponentLocation() + GetControlRotation().Vector() * 5000;

	//��������������������ȥ�������߼�⣬�������������ײ����ô�����µ�����㣬���򲻱�
	//Hit�洢�����
	FHitResult Hit;

	//���ü����Щ����
	FCollisionQueryParams Params;

	//��Ҫ����Լ�
	Params.AddIgnoredActor(this);

	//���ü���������ײ��������
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	//ȥ�ж��Ƿ���ײ������
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraComp->GetComponentLocation(), CrossFair_Location, ObjectQueryParams, Params);
	if (bBlockingHit) {
		CrossFair_Location = Hit.ImpactPoint;
	}

	return CrossFair_Location;
}

void ATCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	//��ȡ�������λ��
	FVector FinalLoc = GetTrue_Rotate();
	//���÷���λ��
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	//���÷��䳯��
	FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, FinalLoc);

	//����Spawn��������
	FTransform SpawnTM = FTransform(FinalRot, HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	//���ݴ��������������
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



