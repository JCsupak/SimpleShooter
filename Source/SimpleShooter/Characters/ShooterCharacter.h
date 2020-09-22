// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float ViewRotationRate = 70;

	void MoveForward(float AxisValue);
	void MoveStrafing(float AxisValue);
	void LookVerticalRate(float AxisValue);
	void LookHorizontalRate(float AxisValue);
	void Walk();
	void StopWalk();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;
	UPROPERTY()
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere)
	float Health;
	UPROPERTY(EditDefaultsOnly)
	float WalkSpeedMultiplier = 0.4f;

	bool bIsWalking = false;

protected:
	virtual void BeginPlay() override;

public:	
	AShooterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot();
	void Reload();
	bool IsFullHealth() const;
	void Heal(float HealingAmount);

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	int GetShotsRemaining() const;

	UFUNCTION(BlueprintPure)
	float GetReloadProgress() const;
};
