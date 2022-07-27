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
        UDbg::DbgMsg(FString::Printf(TEXT("RotateComponent IS NULL")), 5.0f, FColor::Red);
        return;
    }
    
    if(PlayerCharacter == nullptr)
    {
        UDbg::DbgMsg(FString::Printf(TEXT("PlayerCharacter IS NULL")), 5.0f, FColor::Red);
        return;
    }
    
    FVector MovingScoreWidgetLoc = RotateComponent->GetComponentLocation();
    FVector PlayerLoc = PlayerCharacter->GetRootComponent()->GetComponentLocation();
    FRotator MovingScoreWidgetRot = UKismetMathLibrary::FindLookAtRotation(MovingScoreWidgetLoc, PlayerLoc);
    FRotator MovingScoreWidgetRotNew = FRotator(0, MovingScoreWidgetRot.Yaw, 0);
    
    RotateComponent->SetWorldRotation(MovingScoreWidgetRotNew);
}
