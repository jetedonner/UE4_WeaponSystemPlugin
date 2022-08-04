//
//  WeaponDefinition.h
//  UE4_WeaponSystemDemo
//
//  Created by Kim David Hauser on 24.03.22.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "InputCoreTypes.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSystem/WeaponSystemProjectileBase.h"
#include "WeaponFunctionDefinition.generated.h"

UENUM(BlueprintType)
enum class EWeaponFunction : uint8
{
    Primary                 UMETA(DisplayName = "Primary weapon function"),
    Secondary               UMETA(DisplayName = "Secondary weapon function"),
};

USTRUCT(Blueprintable)
struct FWeaponFunctionDefinition
{
    GENERATED_USTRUCT_BODY()
    
public:
    
    FWeaponFunctionDefinition()
    {
        this->WeaponFunction = EWeaponFunction::Primary;
    }
    
    FWeaponFunctionDefinition(EWeaponFunction _WeaponFunction)
    {
        this->WeaponFunction = _WeaponFunction;
    }
    
    UPROPERTY(BlueprintReadOnly, Category="Weapon System")
    EWeaponFunction WeaponFunction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    float DamageFactor = 5.0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    float Cadence = 1.0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* ShootingSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    TSubclassOf<AWeaponSystemProjectileBase> Projectile;
    
};
