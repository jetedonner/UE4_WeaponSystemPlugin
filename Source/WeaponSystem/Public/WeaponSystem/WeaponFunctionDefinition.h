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
//#include "WeaponSystem/WeaponSystemProjectileBase.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSystem/WeaponSystemProjectileBase.h"
//#include "WeaponSystem/Crosshair/CrosshairUserWidgetBase.h"
#include "WeaponFunctionDefinition.generated.h"

UENUM(BlueprintType)
enum class EWeaponFunction : uint8
{
    Primary                 UMETA(DisplayName = "Primary weapon function"),
    Secondary               UMETA(DisplayName = "Secondary weapon function"),
};

USTRUCT(Blueprintable)
struct FWeaponFunctionDefinition /*: public FTableRowBase*/
{
    GENERATED_USTRUCT_BODY()
    
public:
    
    FWeaponFunctionDefinition()//EWeaponFunction WeaponFunction)//EWeaponFunction WeaponFunction)
    {
        //this->WeaponFunction = WeaponFunction;
        this->WeaponFunction = EWeaponFunction::Primary;
        UE_LOG(LogTemp, Warning, TEXT("FWeaponFunctionDefinition()"));
    }
    
    FWeaponFunctionDefinition(EWeaponFunction _WeaponFunction)//EWeaponFunction WeaponFunction)
    {
        this->WeaponFunction = _WeaponFunction;
        UE_LOG(LogTemp, Warning, TEXT("FWeaponFunctionDefinition(EWeaponFunction _WeaponFunction)"));
    }

//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    int32 WeaponID = -1;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    FString WeaponName = "BaseWeaponDefinition";
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    FString WeaponDescription = "BaseWeaponDescription";
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon System")
    EWeaponFunction WeaponFunction;// = EWeaponFunction::Primary;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    float DamageFactor = 5.0;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    int32 ClipSize = 30;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    int32 PickUpCount = 30;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    float Cadence = 1.0;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    float ReloadTimeout = 1.0;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    FKey ActivationKey;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* ShootingSound;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    class USoundCue* MagEmptySound;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    bool PlayReloadSound = true;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    class USoundCue* ReloadSound;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    UTexture2D* HUDIcon;
    
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
    TSubclassOf<AWeaponSystemProjectileBase> Projectile;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    TSubclassOf<class UUserWidget> CrosshairUserWidget;
};
