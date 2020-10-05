// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerItemActivator.generated.h"


class UPhysicsHandleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UPlayerItemActivator : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerItemActivator();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float PlayerReach = 450.0f;

	FVector PlayerPointerLocation;
	FRotator PlayerPointerRotation;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void SetupOwnerComponents();
	void SetupInputComponent(AActor* MyActor);
	void FindPhysicsHandle(AActor* MyActor);
	void UpdatePlayerPointer();
	void ContinueObjectInteraction();
	FVector GetPlayerGrabPosition() const;
	FHitResult GetFirstPhysicsBodyInReach() const;
	void ActivateItem();
	void Release();
	void DisplayPlayerReach(); //Debug method


public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
