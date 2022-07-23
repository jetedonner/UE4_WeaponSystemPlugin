//
//  HUDFunc.cpp
//  UE4_WeaponSystemDemo
//
//  Created by Kim David Hauser on 24.03.22.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#include "HUDFunc.h"

void UHUDFunc::RotateToPlayer(USceneComponent* RotateComponent, ACharacter* PlayerCharacter)
{
    if(RotateComponent == nullptr)
    {
        UE_LOG(LogSuake3D, Error, TEXT("RotateComponent IS NULL"));
        return;
    }
    
    if(PlayerCharacter == nullptr)
    {
        UE_LOG(LogSuake3D, Error, TEXT("PlayerCharacter IS NULL"));
        return;
    }
    
    FVector MovingScoreWidgetLoc = RotateComponent->GetComponentLocation();
    FVector PlayerLoc = PlayerCharacter->GetRootComponent()->GetComponentLocation();
    FRotator MovingScoreWidgetRot = UKismetMathLibrary::FindLookAtRotation(MovingScoreWidgetLoc, PlayerLoc);
    FRotator MovingScoreWidgetRotNew = FRotator(0, MovingScoreWidgetRot.Yaw, 0);
    
    RotateComponent->SetWorldRotation(MovingScoreWidgetRotNew);
}
