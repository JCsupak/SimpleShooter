// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemAbility.generated.h"

class AShooterCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UItemAbility : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemAbility();

	virtual void UseItemAbility(AShooterCharacter* ItemUser);

protected:
	virtual void BeginPlay() override;

		
};
