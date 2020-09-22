// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemAbility.h"
#include "HealthStationAbility.generated.h"

UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UHealthStationAbility : public UItemAbility
{
	GENERATED_BODY()
	
public:
	UHealthStationAbility();

	virtual void UseItemAbility(AShooterCharacter* ItemUser) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float HealingPerSecond = 15.0f;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealingAvailable = 90.0f;

	float CurrentHealingAvailable;

	void HealUser(AShooterCharacter* ItemUser);
};
