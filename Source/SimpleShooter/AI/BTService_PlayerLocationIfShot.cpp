// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfShot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"

UBTService_PlayerLocationIfShot::UBTService_PlayerLocationIfShot()
{
	NodeName = TEXT("Update Player Location If Shot");
}

void UBTService_PlayerLocationIfShot::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AShooterCharacter* AICharacter = Cast<AShooterCharacter>(OwnerComp.GetOwner());
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AICharacter == nullptr || PlayerPawn == nullptr)
	{
		return;
	}
	
}