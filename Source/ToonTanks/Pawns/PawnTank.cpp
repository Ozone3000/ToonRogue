// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerControllerRef = Cast<APlayerController>(GetController());
    MoveDirection = FVector(0, 0, 0);
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    Move();
    Rotate();

    if (PlayerControllerRef)
    {
        FHitResult TraceHitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
        FVector HitLocation = TraceHitResult.ImpactPoint;

        RotateTurret(HitLocation);
    }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("MoveSideways", this, &APawnTank::CalculateSidewaysInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
}
void APawnTank::CalculateMoveInput(float Value) 
{
    MoveDirection = FVector(Value, MoveDirection.Y, 0);
}

void APawnTank::CalculateSidewaysInput(float Value)
{
    MoveDirection = FVector(MoveDirection.X, Value, 0);
}

void APawnTank::CalculateRotateInput(float Value) 
{
    float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
    FRotator Rotation = FRotator(0, RotateAmount, 0);
    RotationDirection = FQuat(Rotation);
}

void APawnTank::Move()
{
    SetActorRotation(FRotator(0,0,0));
    MoveDirection.Normalize();
    float speed = MoveSpeed * GetWorld()->DeltaTimeSeconds;
    MoveDirection *= FVector(speed, speed, 0);
    AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
    if (MoveDirection.Size() > 0.1f)
    {
        MoveDirectionRot = MoveDirection;
    }

    FVector Target = this->GetActorLocation() + MoveDirectionRot;
    FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Target);
    //AddActorLocalRotation(RotationDirection, true);
    FRotator InterpRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), PlayerRot, 1.0f, 5.0f);
    SetActorRotation(InterpRot);
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();
    // Hide player TODO - Create new function to handle this
    bIsPlayerAlive = false;

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

bool APawnTank::GetIsPlayerAlive() 
{
    return bIsPlayerAlive;
}