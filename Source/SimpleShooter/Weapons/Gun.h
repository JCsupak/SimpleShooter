// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	AGun();
	virtual void Tick(float DeltaTime) override;

	void PullTrigger();
	void StartReloadTimer();
	float GetReloadTimerPercentage() const;
	int GetShotsRemaining() const;
	bool GetIsReloading() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;
	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;
	UPROPERTY(EditAnywhere)
	float MaxRange = 2500;
	UPROPERTY(EditAnywhere)
	float Damage = 10;
	UPROPERTY(EditAnywhere)
	int MaxShots = 12;
	UPROPERTY(EditAnywhere)
	float ReloadDelay = 1.8f;
	UPROPERTY(EditAnywhere)
	USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere)
	USoundBase* WeaponCockingSound;

	int ShotsRemaining;
	bool bIsReloading = false;
	FTimerHandle ReloadTimer;

	void Shoot();
	void Reload();
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection, AController* OwnerController);
	AController* GetOwnerController() const;
};
