//
//  UtilityTimer.cpp
//  UE4_WeaponSystemDemo_Index
//
//  Created by Kim David Hauser on 24.03.22.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#include "Utils/UtilityTimer.h"

UtilityTimer::UtilityTimer()
{

}

UtilityTimer::~UtilityTimer()
{
    
}

uint64 UtilityTimer::UnixTimeNow()
{
    return FDateTime::Now().GetTicks();
}

void UtilityTimer::Tick()
{
    TickTime = UnixTimeNow();
}

uint32 UtilityTimer::Tock()
{
    if(TickTime > 0)
    {
        TockTime = UnixTimeNow();
        return TockTime - TickTime;
    }
    else
    {
        return 0;
    }
}

void UtilityTimer::Reset()
{
    TickTime = 0;
    TockTime = 0;
}
