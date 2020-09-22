// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthStationAbility.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"

UHealthStationAbility::UHealthStationAbility()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthStationAbility::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealingAvailable = MaxHealingAvailable;
}

void UHealthStationAbility::UseItemAbility(AShooterCharacter* ItemUser)
{
	Super::UseItemAbility(ItemUser);
	HealUser(ItemUser);
}

void UHealthStationAbility::HealUser(AShooterCharacter* ItemUser)
{
	if (CurrentHealingAvailable > 0 && !ItemUser->IsFullHealth())
	{
		float HealAmount = GetWorld()->GetDeltaSeconds() * HealingPerSecond;
		CurrentHealingAvailable -= HealAmount;
		ItemUser->Heal(HealAmount);
	}
}