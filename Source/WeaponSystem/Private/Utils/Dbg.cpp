//
//  Dbg.cpp
//  UE4_WeaponSystemDemo
//
//  Created by Kim David Hauser on 24.03.22.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#include "Utils/Dbg.h"

void UDbg::DbgMsg(const FString& DebugMessage, float Duration, FColor TextColor, const bool ConsoleOnly)
{
    UDevHelper* DevHelper = NewObject<UDevHelper>(UDevHelper::StaticClass());
    
    if(DevHelper->DebugMode)
    {
        UE_LOG(LogSuake3D, Log, TEXT("%s"), *DebugMessage);
        
        if(!ConsoleOnly && DevHelper->OnScreeLog)
        {
            GEngine->AddOnScreenDebugMessage(-1, Duration, TextColor, DebugMessage);
        }
    }
}
