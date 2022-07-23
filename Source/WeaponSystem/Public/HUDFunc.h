//
//  HUDFunc.h
//  UE4_WeaponSystemDemo
//
//  Created by Kim David Hauser on 24.03.22.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Utils/GlobalDefinitions.h"
#include "HUDFunc.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONSYSTEM_API UHUDFunc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    /*Starts an analytics session without any custom attributes specified */
    UFUNCTION(BlueprintCallable, Category="Widgets")
    static void RotateToPlayer(USceneComponent* RotateComponent, ACharacter* PlayerCharacter);

};
