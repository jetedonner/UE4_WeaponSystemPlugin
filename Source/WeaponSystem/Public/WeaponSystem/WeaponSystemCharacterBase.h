//
//  WeaponSystemCharacterBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "WeaponManagerComponent.h"
//#include "WeaponDefinition.h"
#include "Health/HealthManagerComponent.h"
#include "Score/ScoreManagerComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Utils/GlobalDefinitions.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "WeaponSystemCharacterBase.generated.h"

//WidgetBlueprint'/WeaponSystem/Widgets/FloatingHealthBar.FloatingHealthBar'

UCLASS()
class WEAPONSYSTEM_API AWeaponSystemCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWeaponSystemCharacterBase();
    AWeaponSystemCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
//    virtual void Tick(float DeltaTime);

public:
    
    FTimeline TimeLine;
    FTimerHandle TimerHandle;
//    FTimerHandle TimerStartHandle;
    
    const float DELTATIME = 0.1f;

//    void StartTimeline();
    void TickTimeline();

    UFUNCTION()
    void EffectProgress(float Value);
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FirstPersonCamera;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    UWeaponManagerComponent* WeaponManagerComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health System")
    UHealthManagerComponent* HealthManagerComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Score System")
    UScoreManagerComponent* ScoreManagerComponent;
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Score System")
//    class UUserWidget* FloatingHealthBar;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Score System")
    float HitScore = 250.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool IsAimedAtTarget = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool IsAimedAtChar = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    bool IsAimedAtPickup = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class USoundCue* DieSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon System")
    class UWidgetComponent* FloatingHealthBar;
    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void ActivateWeapon(FKey Param, int32 WeaponID);
    
    UFUNCTION(BlueprintCallable, Category="Weapon System")
    void ProjectileHit(class AActor* ProjectileActor, class AActor* OtherActor, const FVector Location);
    
//    UFUNCTION(BlueprintCallable, Category="Health System")
//    void ApplyDamage(float Damage, bool DidDie);
    
    UFUNCTION()
    void OnReceivedAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
    
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
    
    UFUNCTION(BlueprintNativeEvent, Category="Event|Hit")
    void OnActorGotHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

    void OnActorGotHit_Implementation(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
