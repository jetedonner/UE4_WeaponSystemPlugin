//
//  HitableSphereActorBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 31.03.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "WeaponSystem/Hitables/HitableActorBase.h"
#include "Components/SphereComponent.h"
#include "HitableSphereActorBase.generated.h"

UCLASS()
class WEAPONSYSTEM_API AHitableSphereActorBase : public AHitableActorBase
{
	GENERATED_BODY()
	
public:
    
    AHitableSphereActorBase();
    AHitableSphereActorBase(const FObjectInitializer& ObjectInitializer);

};
