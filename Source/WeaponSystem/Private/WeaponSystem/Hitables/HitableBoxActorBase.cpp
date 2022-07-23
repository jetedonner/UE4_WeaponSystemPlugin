//
//  HitableBoxActorBase.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 31.03.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/Hitables/HitableBoxActorBase.h"

// Sets default values
AHitableBoxActorBase::AHitableBoxActorBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

AHitableBoxActorBase::AHitableBoxActorBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    
    if(!CollisionComponent)
    {
        CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
        Cast<UBoxComponent>(CollisionComponent)->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f), true);
        CollisionComponent->bEditableWhenInherited = true;
        CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
        RootComponent = CollisionComponent;
        AttachMesh();
    }
}
