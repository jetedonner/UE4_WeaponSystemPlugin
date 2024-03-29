//
//  WeaponSystemCharacterBase.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/WeaponSystemCharacterBase.h"

AWeaponSystemCharacterBase::AWeaponSystemCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

AWeaponSystemCharacterBase::AWeaponSystemCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    
    if(!WeaponManagerComponent)
    {
        WeaponManagerComponent = ObjectInitializer.CreateDefaultSubobject<UWeaponManagerComponent>(this, TEXT("Weapon Manager Component"));
        WeaponManagerComponent->bEditableWhenInherited = true;
        
        WeaponManagerComponent->OnProjectileHitDelegate.AddDynamic(this, &AWeaponSystemCharacterBase::ProjectileHit);
        
        WeaponManagerComponent->SetupAttachment(RootComponent);
    }
    
    if(!HealthManagerComponent)
    {
        HealthManagerComponent = ObjectInitializer.CreateDefaultSubobject<UHealthManagerComponent>(this, TEXT("Health Manager Component"));
        HealthManagerComponent->bEditableWhenInherited = true;
        HealthManagerComponent->OnReceivedAnyDamageDelegate.AddDynamic(this, &AWeaponSystemCharacterBase::OnReceivedAnyDamage);
        this->AddOwnedComponent(HealthManagerComponent);
    }
    
    if(!ScoreManagerComponent)
    {
        ScoreManagerComponent = ObjectInitializer.CreateDefaultSubobject<UScoreManagerComponent>(this, TEXT("Score Manager Component"));
        ScoreManagerComponent->bEditableWhenInherited = true;
        this->AddOwnedComponent(ScoreManagerComponent);
    }
    
    if(!FloatingHealthBar)
    {
        FloatingHealthBar = CreateDefaultSubobject<UWidgetComponent>(FName("FloatingHealthBar Component"));
        
        static ConstructorHelpers::FClassFinder<UUserWidget> FloatingHealthBarWidget(TEXT("/WeaponSystem/Widgets/FloatingHealthBar"));
        
        FloatingHealthBar->bEditableWhenInherited = true;
        FloatingHealthBar->SetWidgetClass(FloatingHealthBarWidget.Class);
        FloatingHealthBar->SetWidgetSpace(EWidgetSpace::World);
        FloatingHealthBar->SetupAttachment(this->GetRootComponent());
    }
    
    if(!FirstPersonCamera)
    {
        FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
        FirstPersonCamera->SetupAttachment(RootComponent, "head");
        FirstPersonCamera->bUsePawnControlRotation = true;
    }
    
    const ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/WeaponSystem/Materials/DieMaterialCurve.DieMaterialCurve'"));
    TimeLine = FTimeline{};
    FOnTimelineFloat progressFunction{};
    progressFunction.BindUFunction(this, "EffectProgress");
    TimeLine.AddInterpFloat(Curve.Object, progressFunction, FName{TEXT("EFFECTFADE")});
}

void AWeaponSystemCharacterBase::BeginPlay()
{
	Super::BeginPlay();
    
    Cast<AActor>(this)->OnTakeAnyDamage.AddDynamic(this, &AWeaponSystemCharacterBase::OnTakeAnyDamageNG);
}

void AWeaponSystemCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponSystemCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    if (PlayerInputComponent == nullptr) return;
    
    if(!WeaponManagerComponent)
    {
//        UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase->WeaponManagerComponent NOT setup ..."));
    }
    else
    {
        const FKey PrimaryShootKey = WeaponManagerComponent->PrimaryShootKey;
        
        FInputKeyBinding KBP_PrimaryShootKey(FInputChord(PrimaryShootKey, false, false, false, false), EInputEvent::IE_Pressed);
        KBP_PrimaryShootKey.bConsumeInput = true;
        KBP_PrimaryShootKey.bExecuteWhenPaused = false;

        KBP_PrimaryShootKey.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
        {
//            UDbg::DbgMsg(FString(TEXT("SHOOTING PRIMARY!!!")), 5.0f, FColor::Orange);
            WeaponManagerComponent->StartShooting();
        });
        PlayerInputComponent->KeyBindings.Add(KBP_PrimaryShootKey);
        
        
        // Listen for key released
        FInputKeyBinding KBR_PrimaryShootKey(FInputChord(PrimaryShootKey, false, false, false, false), EInputEvent::IE_Released);
        KBR_PrimaryShootKey.bConsumeInput = true;
        KBR_PrimaryShootKey.bExecuteWhenPaused = false;
        KBR_PrimaryShootKey.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
        {
//            UDbg::DbgMsg(FString(TEXT("SHOOTING ENDED PRIMARY!!!")), 5.0f, FColor::Orange);
            WeaponManagerComponent->StopShooting();
        });
        InputComponent->KeyBindings.Add(KBR_PrimaryShootKey);
        
        const FKey SecondaryShootKey = WeaponManagerComponent->SecondaryShootKey;
        
        FInputKeyBinding KBP_SecondaryShootKey(FInputChord(SecondaryShootKey, false, false, false, false), EInputEvent::IE_Pressed);
        KBP_SecondaryShootKey.bConsumeInput = true;
        KBP_SecondaryShootKey.bExecuteWhenPaused = false;

        KBP_SecondaryShootKey.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
        {
//            UDbg::DbgMsg(FString(TEXT("SHOOTING SECONDARY!!!")), 5.0f, FColor::Yellow);
            WeaponManagerComponent->StartShooting(EWeaponFunction::Secondary);
        });
        PlayerInputComponent->KeyBindings.Add(KBP_SecondaryShootKey);
        
        FInputKeyBinding KBR_SecondaryShootKey(FInputChord(SecondaryShootKey, false, false, false, false), EInputEvent::IE_Released);
        KBR_SecondaryShootKey.bConsumeInput = true;
        KBR_SecondaryShootKey.bExecuteWhenPaused = false;
        KBR_SecondaryShootKey.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
        {
            UDbg::DbgMsg(FString(TEXT("SHOOTING ENDED SECONDARY!!!")), 5.0f, FColor::Yellow);
            WeaponManagerComponent->StopShooting();
        });
        InputComponent->KeyBindings.Add(KBR_SecondaryShootKey);
        
        for (int i = 0; i <  WeaponManagerComponent->WeaponsArray.Num(); i++)
        {
            TSubclassOf<class UWeaponComponentBase> It = WeaponManagerComponent->WeaponsArray[i];
            
            if(It->IsChildOf(UWeaponComponentBase::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract))
            {
                if(It->GetClass() != UWeaponComponentBase::StaticClass() && !It->GetName().StartsWith(TEXT("SKEL_")))
                {
                    UWeaponComponentBase* SpawnedWeapon = NewObject<UWeaponComponentBase>(WeaponManagerComponent->GetOwner(), It->GetDefaultObject()->GetClass());

                    FWeaponDefinition* WeaponDefinition = SpawnedWeapon->WeaponDefinition();
                    const FKey Key = WeaponDefinition->ActivationKey;
                    
                    FString tmp = EKeys::GetKeyDetails(Key)->GetDisplayName(true).ToString();

                    FInputKeyBinding KBP(FInputChord(Key, false, false, false, false), EInputEvent::IE_Pressed);
                    KBP.bConsumeInput = true;
                    KBP.bExecuteWhenPaused = false;

                    KBP.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
                    {
                        ActivateWeapon(Key, WeaponDefinition->WeaponID);
                    });
                    PlayerInputComponent->KeyBindings.Add(KBP);
                    
                    //    // Listen for key released
                    //    FInputKeyBinding KBR(FInputChord(Key, false, false, false, false), EInputEvent::IE_Released);
                    //    KBR.bConsumeInput = true;
                    //    KBR.bExecuteWhenPaused = false;
                    //    KBR.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
                    //    {
                    //        Delegate.ExecuteIfBound(Key, false);
                    //    });
                    //    InputComponent->KeyBindings.Add(KBR);
                }
            }
        }
    }
}

void AWeaponSystemCharacterBase::ActivateWeapon(FKey Param, int32 WeaponID)
{
    if(!WeaponManagerComponent)
    {
//        UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase::ActivateWeapon: %i (WeaponManagerComponent == NULL) !!!"), WeaponID);
    }
    else
    {
        if(WeaponManagerComponent->CurrentWeapon->WeaponID() != WeaponID)
        {
            IsAimedAtChar = false;
            IsAimedAtTarget = false;
            IsAimedAtPickup = false;
            WeaponManagerComponent->SetCurrentWeapon(WeaponID);
        }
    }
}

void AWeaponSystemCharacterBase::ProjectileHit(class AActor* ProjectileActor, class AActor* OtherActor, const FVector Location)
{
    UDbg::DbgMsg(FString(TEXT("AWeaponSystemCharacterBase::ProjectileHit!!!")), 5.0f, FColor::Yellow);
    AWeaponSystemCharacterBase* OtherCharacter = Cast<AWeaponSystemCharacterBase>(OtherActor);
    if(OtherCharacter)
    {
        if(OtherCharacter->HealthManagerComponent->Died)
        {
            float NewScore = 0.0f;
            this->ScoreManagerComponent->AddScore(OtherCharacter->HitScore, NewScore);
        }
    }
}

void AWeaponSystemCharacterBase::EffectProgress(float Value)
{

}

void AWeaponSystemCharacterBase::TickTimeline()
{
    if (TimeLine.IsPlaying())
    {
        TimeLine.TickTimeline(DELTATIME);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(TimerHandle);
    }
}

void AWeaponSystemCharacterBase::OnReceivedAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    UDbg::DbgMsg(FString(TEXT("AWeaponSystemCharacterBase::OnReceivedAnyDamage!!!")), 5.0f, FColor::Yellow);
    if(HealthManagerComponent)
    {
        Cast<UFloatingHealthBarWidget>(FloatingHealthBar->GetUserWidgetObject())->Health = HealthManagerComponent->Health;

        if(this->HealthManagerComponent->Died)
        {
            if(DieSound)
            {
                UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, DieSound, GetActorLocation(), FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
            }
            
            TimeLine.PlayFromStart();
            
            GetWorldTimerManager().SetTimer(TimerHandle, this, &AWeaponSystemCharacterBase::TickTimeline, DELTATIME, true, 1.85f);
        }
    }
}

void AWeaponSystemCharacterBase::OnTakeAnyDamageNG(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    UDbg::DbgMsg(FString(TEXT("AWeaponSystemCharacterBase::OnTakeAnyDamageNG!!!")), 5.0f, FColor::Yellow);
    if(HealthManagerComponent)
    {
        if(!this->HealthManagerComponent->Died)
        {
            HealthManagerComponent->ApplyDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
            
            if(this->HealthManagerComponent->Died)
            {
                if(DieSound)
                {
                    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, DieSound, GetActorLocation(), FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
                }
            }
        }
    }
}

void AWeaponSystemCharacterBase::OnActorGotHit_Implementation(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    UDbg::DbgMsg(FString(TEXT("AWeaponSystemCharacterBase::OnActorGotHit_Implementation!!!")), 5.0f, FColor::Yellow);
    if(HealthManagerComponent)
    {
        if(this->HealthManagerComponent->Died)
        {
            if(DieSound)
            {
                UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, DieSound, GetActorLocation(), FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
            }
        }
    }
}
