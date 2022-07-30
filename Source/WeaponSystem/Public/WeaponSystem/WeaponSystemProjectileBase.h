//
//  WeaponSystemProjectileBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
//#include "WeaponDefinition.h"
#include "DecalStruct.h"
#include "Utils/GlobalDefinitions.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "GameFramework/DamageType.h"
#include "Sound/SoundCue.h"
#include "NiagaraSystem.h"
#include "Materials/Material.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
//#include "Hitables/HitableActorBase.h"
#include "Math/UnrealMathUtility.h"
#include "WeaponSystemProjectileBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FProjectileHitDelegate, AActor*, ProjectileActor, AActor*, OtherActor, FVector, Location);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileCustomFireDelegate, AActor*, ProjectileActor, FVector, ShootDirection);

UCLASS(Blueprintable)
class WEAPONSYSTEM_API AWeaponSystemProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponSystemProjectileBase();
    AWeaponSystemProjectileBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void OutsideWorldBounds() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(RequiredAssetDataTags="RowStructure=WeaponDefinition"), Category="Weapon System")
    FDataTableRowHandle WeaponTypeDef;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* ShotSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class UAudioComponent* ShotAudioComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* ImpactTargetSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* ImpactFailSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    class USphereComponent* CollisionSphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    class UStaticMeshComponent* StaticMeshComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System", meta = ( AllowPrivateAccess = "true" ))
    class UProjectileMovementComponent* ProjectileMovement;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    bool DestroyOnHit = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    bool AutomaticRecharge = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    UNiagaraSystem* ImpactEffect;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    UParticleSystem* ImpactEffectOld;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    TArray<FDecalStruct> ImpactDecalMaterialStructs;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class UDecalComponent* ImpactDecalObject;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    FDecalStruct ImpactDecalDefinition;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
//    float DecalLifeSpan = 10.0f;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System|Decal", meta=(ClampMin="2.5"))
//    float DecalLifeSpanMin = 5.0f;
//    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System|Decal")
//    float DecalLifeSpanMax = 15.0f;
//    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System|Decal")
//    bool DecalFadeOutEffect = true;
//    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System|Decal", meta=(EditCondition="DecalFadeOutEffect"))
//    float DecalFadeOutDuration = 2.5f;
    
    UFUNCTION()
    void FireInDirection(const FVector& ShootDirection, bool& WasHandled);
    
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    // Overlap
    UFUNCTION()
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FProjectileHitDelegate OnProjectileHitDelegate;
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FProjectileCustomFireDelegate OnProjectileCustomFireDelegate;
    
};
