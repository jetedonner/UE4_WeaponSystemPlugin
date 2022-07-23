//
//  DecalStruct.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 03.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Materials/MaterialInterface.h"
#include "DecalStruct.generated.h"

/**
 *  Decal Definitions
 */
USTRUCT(Blueprintable)
struct FDecalStruct
{
    GENERATED_USTRUCT_BODY()

public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FVector DecalSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    UMaterialInterface* ImpactDecalMaterial;

};
