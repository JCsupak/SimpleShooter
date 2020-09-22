// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemAbility.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"

UItemAbility::UItemAbility()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UItemAbility::BeginPlay()
{
	Super::BeginPlay();
	
}

void UItemAbility::UseItemAbility(AShooterCharacter* ItemUser)
{
	
}