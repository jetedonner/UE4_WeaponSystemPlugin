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
#include "WeaponFunctionDefinition.h"
//#include "WeaponSystem/WeaponSystemProjectileBase.h"
#include "WeaponSystem/Crosshair/CrosshairUserWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "WeaponDefinition.generated.h"

USTRUCT(Blueprintable)
struct FWeaponDefinition : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()
    
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    int32 WeaponID = -1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FString WeaponName = "BaseWeaponDefinition";
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FString WeaponDescription = "BaseWeaponDescription";
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    float DamageFactor = 5.0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    int32 ClipSize = 30;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    int32 PickUpCount = 30;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    float Cadence = 1.0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    float ReloadTimeout = 1.0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FKey ActivationKey;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    class USoundCue* ShootingSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* MagEmptySound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool PlayReloadSound = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* ReloadSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    UTexture2D* HUDIcon;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    TArray<FWeaponFunctionDefinition> WeaponFunctionDefinitions;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FWeaponFunctionDefinition PrimaryWeaponFunctionDefinition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FWeaponFunctionDefinition SecondaryWeaponFunctionDefinition = FWeaponFunctionDefinition(EWeaponFunction::Secondary);
    
//    SecondaryWeaponFunctionDefinition->WeaponFunction = EWeaponFunction::Secondary;
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    TSubclassOf<AWeaponSystemProjectileBase> Projectile;

//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    class UCrosshairUserWidgetBase* CrosshairUserWidgetNG;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    UMaterialInterface* CrosshairMaterial;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    UCrosshairUserWidgetBase* CrosshairUserWidget;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    bool TesetVarForTeset = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    TSubclassOf<class UUserWidget> CrosshairUserWidget;
};
