//
//  ScoreManagerComponent.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "Score/ScoreManagerComponent.h"

UScoreManagerComponent::UScoreManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UScoreManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UScoreManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UScoreManagerComponent::AddScore(float Value, float& NewScore)
{
    Score += Value;
    NewScore = Score;
}

void UScoreManagerComponent::ResetScore()
{
    Score = 0.0f;
}
