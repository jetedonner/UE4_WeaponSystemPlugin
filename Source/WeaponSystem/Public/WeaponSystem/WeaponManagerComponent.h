//
//  WeaponManagerComponent.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponDefinition.h"
#include "WeaponComponentBase.h"
#include "WeaponPickups/WeaponPickupActorBase.h"
#include "Utils/GlobalDefinitions.h"
#include "Utils/UtilityTimer.h"
#include "Blueprint/UserWidget.h"
#include "WeaponManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponReloadingDelegate, UWeaponComponentBase*, WeaponActorComponent);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONSYSTEM_API UWeaponManagerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManagerComponent();

protected:
    
    FTimerHandle ReloadingEndTimerHandle;
    FTimerHandle ReloadingStartTimerHandle;

    bool IsShooting = false;
    FTimerHandle ShootingTimerHandle;
    
    class UtilityTimer* TimerUtil = new UtilityTimer();
    
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System", meta=(RequiredAssetDataTags="RowStructure=WeaponDefinition"))
    class UDataTable* WeaponTypeTable;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool IsAimedAtHitable = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool IsAimedAtChar = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool IsAimedAtPickup = false;

    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void IsAimedAt(bool& IsAimedAtTarget, FHitResult& HitResult);
//    {
//        IsAimedAtTarget = isHit;//(IsAimedAtHitable || IsAimedAtChar || IsAimedAtPickup);
//    }
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool isHit = false;
    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category="Weapon System")
    bool IsAimedAtInt() {
        bool bRet = false;
        FHitResult HitResult;
        this->IsAimedAt(bRet, HitResult);
        return bRet;
    }
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FHitResult MyHitResult;
    
//    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
//    bool GetIsReloading() { return (CurrentWeapon != nullptr ? CurrentWeapon->IsReloading : false ); }
//
//    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
//    void SetIsReloading(const bool NewVal)
//    {
//        if(CurrentWeapon != nullptr)
//        {
//            CurrentWeapon->IsReloading = NewVal;
//        }
//    }
//
//    UPROPERTY(BlueprintGetter=GetIsReloading, BlueprintSetter=SetIsReloading, Category="Weapon System")
//    bool IsReloading;// = false;
//    int32 ClipAmmoCount;
//
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon System")
    bool IsReloading = false;
    
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon System")
//    bool IsShooting = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FVector MuzzleOffset;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    TArray<TSubclassOf<class UWeaponComponentBase>> WeaponsArray;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    TArray<class UWeaponComponentBase*> WeaponsArrayImpl;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    UWeaponComponentBase* CurrentWeapon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    UUserWidget* CurrentCSWidget;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* WeaponChangeSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FKey PrimaryShootKey;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    FKey SecondaryShootKey;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    EWeaponFunction CurrentWeaponFunction;
    
    // Internal Functions
    void GetCurrentWeaponFunctionDefinitionInt(FWeaponFunctionDefinition& WeaponFunctionDefinition);
    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category="Weapon System")
    FWeaponFunctionDefinition GetCurrentWeaponFunctionDefinition() {
        FWeaponFunctionDefinition WeaponFunctionDefinitionRet;
        GetCurrentWeaponFunctionDefinitionInt(WeaponFunctionDefinitionRet);
        return WeaponFunctionDefinitionRet;
    }
    
    void SetCurrentWeapon(int32 WeaponID, bool PlayAudio = true);
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void PickupWeapon(int32 PickedupWeaponID, int32 AmmoCount);
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void FireShot();
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void ProjectileFired(class AActor* ProjectileActor);
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void StartShooting(EWeaponFunction WeaponFunction = EWeaponFunction::Primary);
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void StopShooting();
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void StartReloading();
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void FinishReloading();
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void ProjectileHit(class AActor* ProjectileActor, class AActor* OtherActor, const FVector Location);
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FProjectileFireDelegate OnProjectileFireDelegate;
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FProjectileHitDelegate OnProjectileHitDelegate;
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FWeaponReloadingDelegate OnWeaponReloading;
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FWeaponStoppedShootingDelegate OnWeaponStoppedShootingDelegate;
    
//    UFUNCTION(BlueprintImplementableEvent,  Category = "Weapon System")
//    void OnChangeWeapon(int32 WeaponID);
};
