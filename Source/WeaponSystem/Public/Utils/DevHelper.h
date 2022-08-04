//
//  DevHelper.h
//  UE4_WeaponSystemDemo
//
//  Created by Kim David Hauser on 27.07.22.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DevHelper.generated.h"

/**
 *
 */
UCLASS(Config=Game)
class WEAPONSYSTEM_API UDevHelper : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
    
public:
    
    UDevHelper();
    
    UPROPERTY(Config)
    bool OnScreeLog = true;
    
    UPROPERTY(Config)
    bool DebugMode = true;
};
