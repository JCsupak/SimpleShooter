// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	ShotsRemaining = MaxShots;
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	if (!bIsReloading)
	{
		if (ShotsRemaining > 0)
		{
			Shoot();
		}
		else
		{
			StartReloadTimer();
		}
	}
	else
	{
		return;
	}
}

void AGun::StartReloadTimer()
{
	bIsReloading = true;
	UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, TEXT("MuzzleFlashSocket"));
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AGun::Reload, ReloadDelay);
}

float AGun::GetReloadTimerPercentage() const
{
	return GetWorld()->GetTimerManager().GetTimerElapsed(ReloadTimer) / ReloadDelay;
}

void AGun::Reload()
{
	bIsReloading = false;
	ShotsRemaining = MaxShots;
	UGameplayStatics::SpawnSoundAttached(WeaponCockingSound, Mesh, TEXT("MuzzleFlashSocket"));
}

int AGun::GetShotsRemaining() const
{
	return ShotsRemaining;
}

void AGun::Shoot()
{
	--ShotsRemaining;
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return;

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection, OwnerController);

	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			UGameplayStatics::SpawnSoundAtLocation(HitActor, ImpactSound, HitActor->GetActorLocation());
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection, AController* OwnerController)
{
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	FVector End = Location + Rotation.Vector() * MaxRange;
	
	ShotDirection = -Rotation.Vector();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;

	return OwnerPawn->GetController();
}

bool AGun::GetIsReloading() const
{
	return bIsReloading;
}