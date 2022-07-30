//
//  Dbg.h
//  UE4_WeaponSystemDemo
//
//  Created by Kim David Hauser on 24.03.22.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utils/DevHelper.h"
#include "Dbg.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONSYSTEM_API UDbg : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    
    UFUNCTION(BlueprintCallable, Category="Debug", meta=(DevelopmentOnly))
    static void DbgMsg(const FString& DebugMessage, float Duration = 5.f, FColor TextColor = FColor::Red, const bool ConsoleOnly = false);
    
};
