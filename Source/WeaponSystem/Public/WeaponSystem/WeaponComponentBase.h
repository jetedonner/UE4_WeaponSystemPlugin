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

UCLASS(Blueprintable, Config=Game)
class WEAPONSYSTEM_API UWeaponComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
    
	UWeaponComponentBase();

    UWeaponComponentBase(const FObjectInitializer& ObjectInitializer);
    
    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    float PluginExampleVariable = 123.456f;
//
//    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    float ExampleVar;

protected:

	virtual void BeginPlay() override;
    
    
    
//    FVec GetMuzzleRotation();

public:
    
    
    
    FTimerHandle ReloadingEndTimerHandle;
    FTimerHandle ReloadingStartTimerHandle;

//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    float ExampleTestNG;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    float KimsVar = 123.0f;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    float JeteSCHOICEReload = 123.0f;
//
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    float ANOTHERTestVar = 123.0f;
    
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
    
    // Internal Functions
    void GetMuzzleRotationInt(FRotator& MuzzleRotation, FVector& MuzzleLocRet);
    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category="Weapon System")
    FRotator GetMuzzleRotation() {
        FRotator MuzzleRotationRet;
        FVector MuzzleLocRet;
        GetMuzzleRotationInt(MuzzleRotationRet, MuzzleLocRet);
        return MuzzleRotationRet;
    }
    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category="Weapon System")
    FVector GetMuzzleLocation() {
        FRotator MuzzleRotationRet;
        FVector MuzzleLocRet;
        GetMuzzleRotationInt(MuzzleRotationRet, MuzzleLocRet);
        return MuzzleLocRet;
    }
    
    UPROPERTY(BlueprintGetter=GetMuzzleRotation, Category="Weapon System")
    FRotator MuzzleRotation;
    
    UPROPERTY(BlueprintGetter=GetMuzzleLocation, Category="Weapon System")
    FVector MuzzleLocation;
    
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
	
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
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
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    EWeaponFunction CurrentWeaponFunction;
    
    UFUNCTION(BlueprintNativeEvent, Category="Weapon System")
    void OnStartedShooting(EWeaponFunction WeaponFunction, bool& Handled);

    void OnStartedShooting_Implementation(EWeaponFunction WeaponFunction, bool& Handled);
    
    UFUNCTION(BlueprintNativeEvent, Category="Weapon System")
    void OnStoppedShooting(EWeaponFunction WeaponFunction, bool& Handled);

    void OnStoppedShooting_Implementation(EWeaponFunction WeaponFunction, bool& Handled);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool TesetVarForTeset = true;
};
