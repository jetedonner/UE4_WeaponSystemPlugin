//
//  WeaponSystemCharacterBase.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//


#include "WeaponSystem/WeaponSystemCharacterBase.h"

// Sets default values
AWeaponSystemCharacterBase::AWeaponSystemCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AWeaponSystemCharacterBase::AWeaponSystemCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    
    UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase::AWeaponSystemCharacterBase() WITH ObjectInitializer !!!"));
    
    if(!WeaponManagerComponent){
        
        WeaponManagerComponent = ObjectInitializer.CreateDefaultSubobject<UWeaponManagerComponent>(this, TEXT("Weapon Manager Component"));
        WeaponManagerComponent->bEditableWhenInherited = true;
        
        WeaponManagerComponent->OnProjectileHitDelegate.AddDynamic(this, &AWeaponSystemCharacterBase::ProjectileHit);
        
        WeaponManagerComponent->SetupAttachment(RootComponent);
    }
    
    if(!HealthManagerComponent){
        HealthManagerComponent = ObjectInitializer.CreateDefaultSubobject<UHealthManagerComponent>(this, TEXT("Health Manager Component"));
        HealthManagerComponent->bEditableWhenInherited = true;
        HealthManagerComponent->OnReceivedAnyDamageDelegate.AddDynamic(this, &AWeaponSystemCharacterBase::OnReceivedAnyDamage);
        this->AddOwnedComponent(HealthManagerComponent);
    }
    
    if(!ScoreManagerComponent){
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
        FirstPersonCamera->SetupAttachment(RootComponent, "head"); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
        FirstPersonCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
    }
    
    const ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/WeaponSystem/Materials/DieMaterialCurve.DieMaterialCurve'"));
    TimeLine = FTimeline{};
    FOnTimelineFloat progressFunction{};
    progressFunction.BindUFunction(this, "EffectProgress"); // The function EffectProgress gets called
    TimeLine.AddInterpFloat(Curve.Object, progressFunction, FName{TEXT("EFFECTFADE")});
}

// Called when the game starts or when spawned
void AWeaponSystemCharacterBase::BeginPlay()
{
	Super::BeginPlay();
    
//    this->OnActorHit.AddDynamic(this, &AWeaponSystemCharacterBase::OnActorGotHit);
    Cast<AActor>(this)->OnTakeAnyDamage.AddDynamic(this, &AWeaponSystemCharacterBase::OnTakeAnyDamage);
}

// Called every frame
void AWeaponSystemCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(!IsPlayerControlled())
    {
        return;
    }
    
    if (GetWorld())
    {
        APlayerCameraManager* CameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
        if (CameraManager )
        {
           FHitResult hitResult;
           FVector Start = CameraManager->GetCameraLocation() - FVector(0.0f, 0.0f, 30.0f);
           FVector End = Start + 10000.0 * CameraManager->GetActorForwardVector();
           DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), false, -1, 0, 5.0);

           bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1);

           if (isHit)
           {
               AActor* HitActor = hitResult.GetActor();
               if (HitActor)
               {
                   
                   AWeaponSystemCharacterBase* pChar = Cast<AWeaponSystemCharacterBase>(HitActor);
                   if (pChar && !IsAimedAtChar)
                   {
                       IsAimedAtChar = true;
//                       IsAimedAtTarget = true;
                       UE_LOG(LogSuake3D, Warning, TEXT("I hit a Character! %f - %s"), DeltaTime, *hitResult.Actor->GetName());
                       UCrosshairUserWidgetBase* CurrentCSWidget = Cast<UCrosshairUserWidgetBase>(WeaponManagerComponent->CurrentCSWidget);
                       if(CurrentCSWidget)
                       {
                           //CurrentCSWidget->OnAnimateCrosshair(true);
                           CurrentCSWidget->PlayAimedAtAnimation(true);
                       }
//#include "Blueprint/WidgetBlueprintLibrary.h"
                   }
                   else if(!pChar && IsAimedAtChar)
                   {
                       IsAimedAtChar = false;
//                       IsAimedAtTarget = false;
                       UCrosshairUserWidgetBase* CurrentCSWidget = Cast<UCrosshairUserWidgetBase>(WeaponManagerComponent->CurrentCSWidget);
                       if(CurrentCSWidget)
                       {
//                           CurrentCSWidget->OnAnimateCrosshair(false);
                           CurrentCSWidget->PlayAimedAtAnimation(false);
                       }
                   }
                   
                   if(IsAimedAtChar)
                   {
                       return;
                   }
                   
                   AHitableActorBase* p = Cast<AHitableActorBase>(HitActor);
                   if (p && !IsAimedAtTarget)
                   {
                       IsAimedAtTarget = true;
                       UE_LOG(LogSuake3D, Warning, TEXT("I hit a thing! %f - %s"), DeltaTime, *hitResult.Actor->GetName());
                       UCrosshairUserWidgetBase* CurrentCSWidget = Cast<UCrosshairUserWidgetBase>(WeaponManagerComponent->CurrentCSWidget);
                       if(CurrentCSWidget)
                       {
//                           CurrentCSWidget->OnAnimateCrosshair(true);
                           CurrentCSWidget->PlayAimedAtAnimation(true);
                       }
//#include "Blueprint/WidgetBlueprintLibrary.h"
                   }
                   else if(!p && IsAimedAtTarget)
                   {
                       IsAimedAtTarget = false;
                       UCrosshairUserWidgetBase* CurrentCSWidget = Cast<UCrosshairUserWidgetBase>(WeaponManagerComponent->CurrentCSWidget);
                       if(CurrentCSWidget)
                       {
//                           CurrentCSWidget->OnAnimateCrosshair(false);
                           CurrentCSWidget->PlayAimedAtAnimation(false);
                       }
                   }
               }
           }
        }
    }
    
}

//void APracticeCharacter::Tick(float DeltaTime)
//{
//
//}

// Called to bind functionality to input
void AWeaponSystemCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    if (PlayerInputComponent == nullptr) return;
    
    if(!WeaponManagerComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase->WeaponManagerComponent NOT setup ..."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase->WeaponManagerComponent IS setup ..."));
        
        const FKey PrimaryShootKey = WeaponManagerComponent->PrimaryShootKey;
        
        FInputKeyBinding KBP_PrimaryShootKey(FInputChord(PrimaryShootKey, false, false, false, false), EInputEvent::IE_Pressed);
        KBP_PrimaryShootKey.bConsumeInput = true;
        KBP_PrimaryShootKey.bExecuteWhenPaused = false;

        KBP_PrimaryShootKey.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
        {
            UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase => SHOOTING (primary) STARTED ..."));
            UDbg::DbgMsg(FString(TEXT("SHOOTING PRIMARY!!!")), 5.0f, FColor::Orange);
            WeaponManagerComponent->StartShooting();
//            ActivateWeapon(PrimaryShootKey, WeaponDefinition->WeaponID);
        });
        PlayerInputComponent->KeyBindings.Add(KBP_PrimaryShootKey);
        
        
        // Listen for key released
        FInputKeyBinding KBR_PrimaryShootKey(FInputChord(PrimaryShootKey, false, false, false, false), EInputEvent::IE_Released);
        KBR_PrimaryShootKey.bConsumeInput = true;
        KBR_PrimaryShootKey.bExecuteWhenPaused = false;
        KBR_PrimaryShootKey.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
        {
            UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase => SHOOTING (primary) ENDED ..."));
            UDbg::DbgMsg(FString(TEXT("SHOOTING ENDED PRIMARY!!!")), 5.0f, FColor::Orange);
            WeaponManagerComponent->StopShooting();
//            Delegate.ExecuteIfBound(Key, false);
        });
        InputComponent->KeyBindings.Add(KBR_PrimaryShootKey);
        
        const FKey SecondaryShootKey = WeaponManagerComponent->SecondaryShootKey;
        
        FInputKeyBinding KBP_SecondaryShootKey(FInputChord(SecondaryShootKey, false, false, false, false), EInputEvent::IE_Pressed);
        KBP_SecondaryShootKey.bConsumeInput = true;
        KBP_SecondaryShootKey.bExecuteWhenPaused = false;

        KBP_SecondaryShootKey.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
        {
            UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase => SHOOTING (secondary) STARTED ..."));
            UDbg::DbgMsg(FString(TEXT("SHOOTING SECONDARY!!!")), 5.0f, FColor::Yellow);
            WeaponManagerComponent->StartShooting(EWeaponFunction::Secondary);
//            ActivateWeapon(PrimaryShootKey, WeaponDefinition->WeaponID);
        });
        PlayerInputComponent->KeyBindings.Add(KBP_SecondaryShootKey);
        
        FInputKeyBinding KBR_SecondaryShootKey(FInputChord(SecondaryShootKey, false, false, false, false), EInputEvent::IE_Released);
        KBR_SecondaryShootKey.bConsumeInput = true;
        KBR_SecondaryShootKey.bExecuteWhenPaused = false;
        KBR_SecondaryShootKey.KeyDelegate.GetDelegateWithKeyForManualSet().BindLambda([=](const FKey& Key)
        {
            UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase => SHOOTING (secondary) ENDED ..."));
            UDbg::DbgMsg(FString(TEXT("SHOOTING ENDED SECONDARY!!!")), 5.0f, FColor::Yellow);
            WeaponManagerComponent->StopShooting();
//            Delegate.ExecuteIfBound(Key, false);
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

                    UE_LOG(LogTemp, Warning, TEXT("SettingUp KeyBinding: %s => %s"), *It->GetName(), *tmp);
                    
                    // Listen for key pressed
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
//        UE_LOG(LogTemp, Warning, TEXT("AWeaponSystemCharacterBase::ActivateWeapon: %i ..."), WeaponID);
        WeaponManagerComponent->SetCurrentWeapon(WeaponID);
    }
}

void AWeaponSystemCharacterBase::ProjectileHit(class AActor* ProjectileActor, class AActor* OtherActor, const FVector Location)
{
    AWeaponSystemCharacterBase* OtherCharacter = Cast<AWeaponSystemCharacterBase>(OtherActor);
    if(OtherCharacter)
    {
        if(OtherCharacter->HealthManagerComponent->Died)
        {
            float NewScore = 0.0f;
            this->ScoreManagerComponent->AddScore(OtherCharacter->HitScore, NewScore);
//            UE_LOG(LogSuake3D, Log, TEXT("NEW SCORE: %f"), this->ScoreManagerComponent->Score);
            
            
        }
    }
}

void AWeaponSystemCharacterBase::EffectProgress(float Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("EffectProgress: timeline: %f value:%f"), TimeLine.GetPlaybackPosition(), Value));
}

//void AWeaponSystemCharacterBase::StartTimeline()
//{
//    GetWorldTimerManager().ClearTimer(TimerStartHandle);
//    
//    TimeLine.PlayFromStart();
//    
//    GetWorldTimerManager().SetTimer(TimerHandle, this, &AWeaponSystemCharacterBase::TickTimeline, DELTATIME, true, 0.0f);
//    
//}

void AWeaponSystemCharacterBase::TickTimeline()
{
    if (TimeLine.IsPlaying())
    {
        TimeLine.TickTimeline(DELTATIME);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(TimerHandle);
//        SetLifeSpan(0);
    }
}

void AWeaponSystemCharacterBase::OnReceivedAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if(HealthManagerComponent)
    {
//        bool DidDie = false;
//        HealthManagerComponent->ApplyDamage(Damage, DidDie);
        
        Cast<UFloatingHealthBarWidget>(FloatingHealthBar->GetUserWidgetObject())->Health = HealthManagerComponent->Health;
        
        UE_LOG(LogSuake3D, Warning, TEXT("AWeaponSystemCharacterBase::OnReceivedAnyDamage() => Died: %s"), (this->HealthManagerComponent->Died ? TEXT("true") : TEXT("false")));
        
    //    this->OnActorGotHit(this, DamageCauser, );
        
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

void AWeaponSystemCharacterBase::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if(HealthManagerComponent)
    {
        if(!this->HealthManagerComponent->Died)
        {
            HealthManagerComponent->ApplyDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
            
            UE_LOG(LogSuake3D, Warning, TEXT("AWeaponSystemCharacterBase::OnActorGotHit_Implementation() => Died: %s"), (this->HealthManagerComponent->Died ? TEXT("true") : TEXT("false")));
            
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
    if(HealthManagerComponent)
    {
//        bool DidDie = false;
//        HealthManagerComponent->ApplyDamage(Damage, DidDie);
        
        UE_LOG(LogSuake3D, Warning, TEXT("AWeaponSystemCharacterBase::OnActorGotHit_Implementation() => Died: %s"), (this->HealthManagerComponent->Died ? TEXT("true") : TEXT("false")));
        
        if(this->HealthManagerComponent->Died)
        {
            if(DieSound)
            {
                UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, DieSound, GetActorLocation(), FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
            }
        }
    }
}
