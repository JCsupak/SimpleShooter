// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerItemActivator.h"
#include "SimpleShooter/Items/ItemAbility.h"
#include "ShooterCharacter.h"

#define OUT

UPlayerItemActivator::UPlayerItemActivator()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerItemActivator::BeginPlay()
{
	Super::BeginPlay();
	
	SetupOwnerComponents();
}

void UPlayerItemActivator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePlayerPointer();
	ContinueObjectInteraction();
}

void UPlayerItemActivator::SetupOwnerComponents()
{
	AActor* MyActor = GetOwner();
	SetupInputComponent(MyActor);
	FindPhysicsHandle(MyActor);
}

void UPlayerItemActivator::SetupInputComponent(AActor* MyActor)
{
	InputComponent = MyActor->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Activate", IE_Pressed, this, &UPlayerItemActivator::ActivateItem);
		InputComponent->BindAction("Activate", IE_Released, this, &UPlayerItemActivator::Release);
	}
}

void UPlayerItemActivator::FindPhysicsHandle(AActor* MyActor)
{
	PhysicsHandle = MyActor->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing an expected UPhysicsHandleComponent"), *GetOwner()->GetName());
		//TODO Add a UPhysicsHandle component
	}
}

void UPlayerItemActivator::UpdatePlayerPointer()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerPointerLocation, OUT PlayerPointerRotation);
}

void UPlayerItemActivator::ContinueObjectInteraction()
{
	if (PhysicsHandle->GetGrabbedComponent())
	{
		UItemAbility* ItemAbility = PhysicsHandle->GetGrabbedComponent()->GetOwner()->FindComponentByClass<UItemAbility>();
		if (ItemAbility)
		{
			AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwner());
			ItemAbility->UseItemAbility(Character);
		}
		else
		{
			PhysicsHandle->SetTargetLocation(GetPlayerGrabPosition());
		}
	}
}

FVector UPlayerItemActivator::GetPlayerGrabPosition() const
{
	FVector PlayerReachDirection = PlayerPointerRotation.Vector();
	FVector PlayerReachLength = PlayerReachDirection * PlayerReach;
	FVector PlayerReachEndPosition = PlayerPointerLocation + PlayerReachLength;

	return PlayerReachEndPosition;
}

FHitResult UPlayerItemActivator::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FVector PlayerReachEndPosition = GetPlayerGrabPosition();

	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerPointerLocation, PlayerReachEndPosition, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
	
	return Hit;
}

void UPlayerItemActivator::ActivateItem()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	if (HitResult.GetActor())
	{
		UItemAbility* Ability = HitResult.GetActor()->FindComponentByClass<UItemAbility>();
		if (Ability != nullptr)
		{	
			AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwner());
			Ability->UseItemAbility(Character);
		}

		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
		FVector ObjectGrabPosition = GetPlayerGrabPosition();
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			ObjectGrabPosition
		);
	}
}

void UPlayerItemActivator::Release()
{
	PhysicsHandle->ReleaseComponent();
}

void UPlayerItemActivator::DisplayPlayerReach()
{
	FVector LineTraceEndPosition = GetPlayerGrabPosition();

	DrawDebugLine(GetWorld(), PlayerPointerLocation, LineTraceEndPosition, FColor::Red, false, -1.0f, 0, 5.0f);
}