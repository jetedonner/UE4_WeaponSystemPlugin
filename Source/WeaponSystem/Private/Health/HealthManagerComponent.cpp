//
//  HealthManagerComponent.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "Health/HealthManagerComponent.h"

UHealthManagerComponent::UHealthManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthManagerComponent::ApplyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    float NewHealth = 0.0f;
    DecreaseHealth(Damage, NewHealth);
    
    UE_LOG(LogTemp, Warning, TEXT("UHealthManagerComponent::ApplyDamageNG: NewHealth: %f"), Health);
    
    this->OnReceivedAnyDamageDelegate.Broadcast(Damage, DamageType, InstigatedBy, DamageCauser);
}

void UHealthManagerComponent::IncreaseHealth(float Value, float& NewHealth)
{
    NewHealth = (Health += Value);
}

void UHealthManagerComponent::DecreaseHealth(float Value, float& NewHealth)
{
    NewHealth = (Health -= Value);
    Died = (Health <= 0.0f);
}
