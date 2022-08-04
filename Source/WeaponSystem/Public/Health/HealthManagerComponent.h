//
//  HealthManagerComponent.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "Utils/Dbg.h"
#include "HealthManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FReceivedAnyDamageDelegate, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONSYSTEM_API UHealthManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health System")
    float Health = 100.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health System")
    bool Died = false;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health System")
    TSubclassOf<class UUserWidget> FloatingHealthBar;
    
    UFUNCTION(BlueprintCallable, Category="Health System")
    void IncreaseHealth(float Value, float& NewHealth);
    
    UFUNCTION(BlueprintCallable, Category="Health System")
    void DecreaseHealth(float Value, float& NewHealth);
    
    UPROPERTY(BlueprintAssignable, Category="Health System")
    FReceivedAnyDamageDelegate OnReceivedAnyDamageDelegate;
    
    UFUNCTION(BlueprintCallable, Category="Health System")
    void ApplyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
    
    virtual void Activate(bool bReset /* = false */) override;
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
