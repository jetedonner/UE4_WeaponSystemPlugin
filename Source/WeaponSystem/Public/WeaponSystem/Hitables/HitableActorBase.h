//
//  HitableActorBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 31.03.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/GlobalDefinitions.h"
#include "WeaponSystem/WeaponSystemCharacterBase.h"
#include "Score/MovingScoreWidgetBase.h"
#include "HitableActorBase.generated.h"

UCLASS(Abstract, Blueprintable)
class WEAPONSYSTEM_API AHitableActorBase : public AActor
{
    GENERATED_BODY()
    
public:
    
    AHitableActorBase();
    AHitableActorBase(const FObjectInitializer& ObjectInitializer);
    
protected:

    virtual void BeginPlay() override;

public:

    virtual void Tick(float DeltaTime) override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    bool AllowMultipleHits = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    bool ByPassTriggeringActors = true;
    
    bool IsHit = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    float HitScore = 100.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    bool DestroyOnHit = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    bool ShowMovingScore = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfoTriggerBox", meta = (AllowPrivateAccess = "true"))
    TArray<TSubclassOf<AWeaponSystemCharacterBase>> TriggeringActors;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    class UShapeComponent* CollisionComponent;
    
    UFUNCTION()
    void AttachMesh();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    class UStaticMeshComponent* MeshComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    class UWidgetComponent* MovingScoreWidgetComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    TSubclassOf<UUserWidget> MovingScoreWidgetClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    UMovingScoreWidgetBase* MovingScoreWidgetBase;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event|Hit")
    int32 MovingScoreWidgetIdx = 0;
    
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    UFUNCTION(BlueprintCallable, Category="Event|Hit")
    virtual void ExecActorHitHandler(AActor* OtherActor, const FHitResult& Hit);
    
    UFUNCTION(BlueprintNativeEvent, Category="Event|Hit")
    void OnComponentGotHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
    
    void OnComponentGotHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

};
