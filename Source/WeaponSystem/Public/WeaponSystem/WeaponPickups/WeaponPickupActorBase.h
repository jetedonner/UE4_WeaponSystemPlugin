//
//  WeaponPickupActorBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 02.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponPickupActorBase.generated.h"

UCLASS()
class WEAPONSYSTEM_API AWeaponPickupActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickupActorBase();
    AWeaponPickupActorBase(const FObjectInitializer& ObjectInitializer);
    
    void SetPickUpCount();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    class UStaticMeshComponent* StaticMeshComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    class USphereComponent* CollisionSphere;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    class URotatingMovementComponent* RotatingMovement;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    FRotator RotationRate = FRotator(0, 300, 0);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    float CollisionRadius = 100.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    UParticleSystemComponent* ParticleSystemComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon System")
    UParticleSystem* PickupEffect;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(RequiredAssetDataTags="RowStructure=WeaponDefinition"), Category="Weapon System")
    FDataTableRowHandle WeaponType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    int32 PickUpCount = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* PickupSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void OnPickup(AActor* OtherActor);
};
