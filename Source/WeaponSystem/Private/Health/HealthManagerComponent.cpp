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
    
    //UDbg::DbgMsg(FString::Printf(TEXT("ADDDIUNG HEALTH MGR TO ACTOOOOOOOOR => %s"), *GetOwner()->GetName()), 5.0f, FColor::Red);
}

void UHealthManagerComponent::Activate(bool bReset /* = false */)
{
    Super::Activate(bReset);
    UDbg::DbgMsg(FString::Printf(TEXT("ADDDIUNG HEALTH MGR TO ACTOOOOOOOOR => %s"), *GetOwner()->GetName()), 5.0f, FColor::Red);
    
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if(OwnerCharacter)
    {
        OwnerCharacter->GetMesh()->OnComponentHit.AddDynamic(this, &UHealthManagerComponent::OnHit);
    }
}

void UHealthManagerComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UDbg::DbgMsg(FString::Printf(TEXT("OnHit OnHit OnHit OnHit OnHit OnHit OnHit => %s"), *GetOwner()->GetName()), 15.0f, FColor::Purple);
    
//    AWeaponSystemProjectileBase* ProjectileBase = Cast<AWeaponSystemProjectileBase>(OtherActor);
//    if(ProjectileBase)
//    {
//        OwnerCharacter->GetMesh()->OnComponentHit.AddDynamic(this, &UHealthManagerComponent::OnHit);
        //ApplyDamage(GetOwner(), 5);
        float NewHealth = 0.0f;
        DecreaseHealth(15.0f, NewHealth);
    
    UDbg::DbgMsg(FString::Printf(TEXT("NEeeeeeeeeeeeeeeeWWWWWWWWW HHHHHHHEEEEEAAALLLLHHHHTTTTT => %f"), NewHealth), 15.0f, FColor::Purple);
//    }
    //this->ExecActorHitHandler(OtherActor, Hit);
    //this->OnComponentGotHit(HitComponent, OtherActor, NormalImpulse, Hit);
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
    
//    UE_LOG(LogTemp, Warning, TEXT("UHealthManagerComponent::ApplyDamageNG: NewHealth: %f"), Health);
    
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
