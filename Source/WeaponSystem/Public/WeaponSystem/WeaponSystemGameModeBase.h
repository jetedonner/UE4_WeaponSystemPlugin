//
//  WeaponSystemGameModeBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 31.03.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WeaponSystemHUDBase.h"
#include "WeaponSystemGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONSYSTEM_API AWeaponSystemGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    AWeaponSystemGameModeBase();
    
};
