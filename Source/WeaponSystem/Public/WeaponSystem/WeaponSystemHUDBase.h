//
//  WeaponSystemHUDBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WeaponSystemHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONSYSTEM_API AWeaponSystemHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:
    AWeaponSystemHUDBase();

    /*
    *     VARIABLES
    */
protected:
    // All on screen widgets to add to the HUD on BeginPlay
    UPROPERTY(EditAnywhere, Category="Weapon System")
    TArray<TSubclassOf<class UUserWidget>> AllUIWidgets;

private:
    // Current list of created UI widgets that are constantly active on screen
    TArray<class UUserWidget*> CreatedWidgets;

    /*
    *     METHODS
    */
protected:
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;
};
