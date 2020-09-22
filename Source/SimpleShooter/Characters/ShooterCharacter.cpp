// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "SimpleShooter/Weapons/Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooter/SimpleShooterGameModeBase.h"
#include "SimpleShooter/AI/ShooterAIController.h"

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	Health = MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveStrafe"), this, &AShooterCharacter::MoveStrafing);
	
	PlayerInputComponent->BindAxis(TEXT("LookVertical"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookHorizontal"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis(TEXT("LookVerticalRate"), this, &AShooterCharacter::LookVerticalRate);
	PlayerInputComponent->BindAxis(TEXT("LookHorizontalRate"), this, &AShooterCharacter::LookHorizontalRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Reload);

	PlayerInputComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Walk);
	PlayerInputComponent->BindAction(TEXT("Walk"), EInputEvent::IE_Released, this, &AShooterCharacter::StopWalk);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	if (bIsWalking)
	{
		AxisValue = AxisValue * WalkSpeedMultiplier;
	}
	
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveStrafing(float AxisValue)
{
	if (bIsWalking)
	{
		AxisValue = AxisValue * WalkSpeedMultiplier;
	}

	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookVerticalRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * ViewRotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookHorizontalRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * ViewRotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}

void AShooterCharacter::Reload()
{
	if (!Gun->GetIsReloading())
	{
		Gun->StartReloadTimer();
	}
}

bool AShooterCharacter::IsFullHealth() const
{
	if (Health < MaxHealth)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void AShooterCharacter::Heal(float HealingAmount)
{
	Health += HealingAmount;
}

void AShooterCharacter::Walk()
{
	bIsWalking = true;
}

void AShooterCharacter::StopWalk()
{
	bIsWalking = false;
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	AShooterAIController* AIController = Cast<AShooterAIController>(GetController());
	if (AIController != nullptr)
	{
		AIController->ShotByPlayer();
	}

	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

int AShooterCharacter::GetShotsRemaining() const
{
	return Gun->GetShotsRemaining();
}

float AShooterCharacter::GetReloadProgress() const
{
	return Gun->GetReloadTimerPercentage();
}