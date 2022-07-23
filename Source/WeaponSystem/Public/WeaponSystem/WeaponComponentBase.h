//
//  WeaponComponentBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "WeaponDefinition.h"
#include "WeaponSystemProjectileBase.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "WeaponComponentBase.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_RetVal_OneParam(bool, FWeaponStartedShootingDelegate, EWeaponFunction, WeaponFunction);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileFireDelegate, AActor*, ProjectileActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponStoppedShootingDelegate, UWeaponComponentBase*, WeaponComponent);

//UENUM(BlueprintType)
//enum class EWeaponFunction : uint8
//{
//    Primary                 UMETA(DisplayName = "Primary weapon function"),
//    Secondary               UMETA(DisplayName = "Secondary weapon function"),
//};

UCLASS(Blueprintable)
class WEAPONSYSTEM_API UWeaponComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
    
	UWeaponComponentBase();

protected:

	virtual void BeginPlay() override;

public:
    
    FTimerHandle ReloadingEndTimerHandle;
    FTimerHandle ReloadingStartTimerHandle;
    
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetIsReloading(const bool NewVal)
    {
        if(!IsReloading && NewVal)
        {
            IsReloading = NewVal;
            StartReloading();
        }
        else if(IsReloading && !NewVal)
        {
            IsReloading = NewVal;
            FinishReloading();
        }
    }
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class UAudioComponent* ShotAudioComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class AWeaponSystemProjectileBase* SpawnedRefSec;
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void StartReloading();
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void FinishReloading();

    UPROPERTY(BlueprintSetter=SetIsReloading, Category="Weapon System")
    bool IsReloading;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool ReadyForNewShot = true;
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(RequiredAssetDataTags="RowStructure=WeaponDefinition"), Category="Weapon System")
    FDataTableRowHandle WeaponTypeDef;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    int32 AmmoCount;
    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category="Weapon System")
    int32 GetClipAmmoCount() { return AmmoCount % WeaponDefinition()->ClipSize; }
    
    UPROPERTY(BlueprintGetter=GetClipAmmoCount, Category="Weapon System")
    int32 ClipAmmoCount;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void GetWeaponID(int32& WeaponID);
    
    int32 WeaponID();
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void GetWeaponDefinition(FWeaponDefinition& WeaponDef);
    
    FWeaponDefinition* WeaponDefinition();

//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    FVector MuzzleOffset;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    TSubclassOf<AWeaponSystemProjectileBase> Projectile;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    TSubclassOf<AWeaponSystemProjectileBase> ProjectileSecondary;
		
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void FireShot(EWeaponFunction WeaponFunction = EWeaponFunction::Primary);
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void ExecFireShot(EWeaponFunction WeaponFunction = EWeaponFunction::Primary);
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void ProjectileHit(class AActor* ProjectileActor, class AActor* OtherActor, const FVector Location);
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FProjectileFireDelegate OnProjectileFireDelegate;
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FProjectileHitDelegate OnProjectileHitDelegate;
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FWeaponStoppedShootingDelegate OnWeaponStoppedShootingDelegate;
    
//    UPROPERTY(BlueprintAssignable, Category="Weapon System")
//    FWeaponStartedShootingDelegate OnWeaponStartedShootingDelegate;
    
    UFUNCTION(BlueprintNativeEvent, Category="Weapon System")
    void OnStartedShooting(EWeaponFunction WeaponFunction, bool& Handled);

    void OnStartedShooting_Implementation(EWeaponFunction WeaponFunction, bool& Handled);
};
