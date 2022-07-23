//
//  UtilityTimer.h
//  UE4_WeaponSystemDemo
//
//  Created by Kim David Hauser on 24.03.22.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"
#include <stdio.h>

class WEAPONSYSTEM_API UtilityTimer
{
    
public:
    UtilityTimer();
    ~UtilityTimer();
    
    uint64 UnixTimeNow();

    void Tick();
    
    uint32 Tock();
    
    void Reset();
    
    uint64 TickTime = 0;
    uint64 TockTime = 0;
};
