// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIC_Base.h"

#include "BehaviorTree/BlackboardComponent.h"


void AAIC_Base::SetState(EAbilities State)
{
	CurrentState = State;

	APawn* PossessedPawn = GetPawn();
	if (!PossessedPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("No Pawn Possessed"));
		return;
	}
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No Blackboard Component"));
		return;
	}

	BlackboardComponent->SetValueAsEnum("States", State);
}
