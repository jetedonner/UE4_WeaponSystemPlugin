//
//  WeaponManagerComponent.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/WeaponManagerComponent.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    if(WeaponTypeTable)
    {
        WeaponTypeTable->RowStruct = FWeaponDefinition::StaticStruct();
    }
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
    
    for(TSubclassOf<UWeaponComponentBase> Weapon: WeaponsArray)
    {
        UWeaponComponentBase* NewWeapon = Cast<UWeaponComponentBase>(Weapon->GetDefaultObject());
        
        UWeaponComponentBase* NewWeaponImpl = NewObject<UWeaponComponentBase>(this, NewWeapon->GetClass(), *NewWeapon->GetClass()->GetName());
            
        NewWeaponImpl->OnProjectileFireDelegate.AddDynamic(this, &UWeaponManagerComponent::ProjectileFired);
        
        NewWeaponImpl->OnProjectileHitDelegate.AddDynamic(this, &UWeaponManagerComponent::ProjectileHit);
        NewWeaponImpl->RegisterComponent();
        WeaponsArrayImpl.AddUnique(NewWeaponImpl);
    }
    this->SetCurrentWeapon(1, false);
}

void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    APawn* MyOwner = Cast<APawn>(GetOwner());
    if(!MyOwner->IsPlayerControlled())
    {
        return;
    }
    
//    if (GetWorld())
//    {
        APlayerCameraManager* CameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
        if (CameraManager)
        {
//           FHitResult hitResult;
           FVector Start = CameraManager->GetCameraLocation() - FVector(0.0f, 0.0f, 30.0f);
           FVector End = Start + 10000.0 * CameraManager->GetActorForwardVector();

           isHit = GetWorld()->LineTraceSingleByChannel(MyHitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1);
            
           if (isHit)
           {
               AActor* HitActor = MyHitResult.GetActor();
               if (HitActor)
               {
                   AWeaponSystemCharacterBase* pChar = Cast<AWeaponSystemCharacterBase>(HitActor);
                   if (pChar && !IsAimedAtChar)
                   {
                       IsAimedAtChar = true;
                       UCrosshairUserWidgetBase* CurrentCSWidgetNew = Cast<UCrosshairUserWidgetBase>(this->CurrentCSWidget);
                       if(CurrentCSWidgetNew)
                       {
                           CurrentCSWidgetNew->PlayAimedAtAnimation(true);
                       }
                   }
                   else if(!pChar && IsAimedAtChar)
                   {
                       IsAimedAtChar = false;
                       UCrosshairUserWidgetBase* CurrentCSWidgetNew = Cast<UCrosshairUserWidgetBase>(this->CurrentCSWidget);
                       if(CurrentCSWidgetNew)
                       {
                           CurrentCSWidgetNew->PlayAimedAtAnimation(false);
                       }
                   }
                   
                   if(IsAimedAtChar)
                   {
                       return;
                   }
                   
                   AHitableActorBase* p = Cast<AHitableActorBase>(HitActor);
                   if (p && !IsAimedAtHitable)
                   {
                       IsAimedAtHitable = true;
                       UCrosshairUserWidgetBase* CurrentCSWidgetNew = Cast<UCrosshairUserWidgetBase>(this->CurrentCSWidget);
                       if(CurrentCSWidgetNew)
                       {
                           CurrentCSWidgetNew->PlayAimedAtAnimation(true);
                       }
                   }
                   else if(!p && IsAimedAtHitable)
                   {
                       IsAimedAtHitable = false;
                       UCrosshairUserWidgetBase* CurrentCSWidgetNew = Cast<UCrosshairUserWidgetBase>(this->CurrentCSWidget);
                       if(CurrentCSWidgetNew)
                       {
                           CurrentCSWidgetNew->PlayAimedAtAnimation(false);
                       }
                   }
                   
                   if(IsAimedAtHitable)
                   {
                       return;
                   }
                   
                   AWeaponPickupActorBase* pick = Cast<AWeaponPickupActorBase>(HitActor);
                   if (pick && !IsAimedAtPickup)
                   {
                       IsAimedAtPickup = true;
                       UCrosshairUserWidgetBase* CurrentCSWidgetNew = Cast<UCrosshairUserWidgetBase>(this->CurrentCSWidget);
                       if(CurrentCSWidgetNew)
                       {
                           CurrentCSWidgetNew->PlayAimedAtAnimation(true);
                       }
                   }
                   else if(!pick && IsAimedAtPickup)
                   {
                       IsAimedAtPickup = false;
                       UCrosshairUserWidgetBase* CurrentCSWidgetNew = Cast<UCrosshairUserWidgetBase>(this->CurrentCSWidget);
                       if(CurrentCSWidgetNew)
                       {
                           CurrentCSWidgetNew->PlayAimedAtAnimation(false);
                       }
                   }
                   
               }
           }
            else
            {
//                HitResult = nullptr;
                IsAimedAtChar = false;
                IsAimedAtHitable = false;
                IsAimedAtPickup = false;
            }
        }
//    }
}

void UWeaponManagerComponent::GetCurrentWeaponFunctionDefinitionInt(FWeaponFunctionDefinition& WeaponFunctionDefinition)
{
    if(CurrentWeaponFunction == EWeaponFunction::Primary)
    {
        WeaponFunctionDefinition = CurrentWeapon->WeaponDefinition()->PrimaryWeaponFunctionDefinition;
    }
    else
    {
        WeaponFunctionDefinition = CurrentWeapon->WeaponDefinition()->SecondaryWeaponFunctionDefinition;
    }
}

void UWeaponManagerComponent::PickupWeapon(int32 PickedupWeaponID, int32 AmmoCount)
{
    UWeaponComponentBase* const* DaWeapon = WeaponsArrayImpl.FindByPredicate( [&](UWeaponComponentBase* Result){ return PickedupWeaponID == Result->WeaponID(); } );
    
    if(DaWeapon)
    {
        UDbg::DbgMsg(FString::Printf(TEXT("PickupWeapon EXISTS AND FOUND")), 5.0f, FColor::Green);
        if(AmmoCount >= 0)
        {
            (*DaWeapon)->AmmoCount += AmmoCount;
        }
        
        if(CurrentWeapon->WeaponID() != PickedupWeaponID)
        {
            IsAimedAtChar = false;
            IsAimedAtHitable = false;
            IsAimedAtPickup = false;
            if(IsShooting)
            {
                StopShooting();
            }
        }
        
        SetCurrentWeapon(PickedupWeaponID, false);
    }
}

void UWeaponManagerComponent::SetCurrentWeapon(int32 WeaponID, bool PlayAudio)
{
    if(!CurrentWeapon || CurrentWeapon->WeaponID() != WeaponID)
    {
        IsAimedAtChar = false;
        IsAimedAtHitable = false;
        IsAimedAtPickup = false;
        for(UWeaponComponentBase* Weapon: WeaponsArrayImpl)
        {
            if(Weapon->WeaponID() == WeaponID)
            {
                if(IsReloading)
                {
                    IsReloading = false;
                }
                
                if(IsShooting)
                {
                    GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
                }
                
                CurrentWeapon = Weapon;
                
                AWeaponSystemCharacterBase* MyOwner = Cast<AWeaponSystemCharacterBase>(this->GetOwner());
                if(CurrentWeapon && MyOwner && MyOwner->IsPlayerControlled())
                {
                    if(CurrentCSWidget)
                    {
                        CurrentCSWidget->RemoveFromViewport();
                        CurrentCSWidget->Destruct();
                        
                        UDbg::DbgMsg(FString::Printf(TEXT("Removing Current CS Widget")), 5.0f, FColor::Green);
                    }
                    
                    TSubclassOf<class UUserWidget> CSWidgetClass = CurrentWeapon->WeaponDefinition()->CrosshairUserWidget;

                        if(CSWidgetClass)
                        {
                            UDbg::DbgMsg(FString::Printf(TEXT("HAS New CS Widget")), 5.0f, FColor::Green);
                            
                            CurrentCSWidget = CreateWidget<UUserWidget>(GetWorld(), CSWidgetClass);
                            if (CurrentCSWidget)
                            {
                                UDbg::DbgMsg(FString::Printf(TEXT("Adding Current CS Widget")), 5.0f, FColor::Green);
                                CurrentCSWidget->AddToViewport();
                            }
                        }
                }
                
                if(WeaponChangeSound && PlayAudio)
                {
                    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, this->WeaponChangeSound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
                }
                break;
            }
        }
    }
}

void UWeaponManagerComponent::IsAimedAt(bool& IsAimedAtTarget, FHitResult& HitResult)
{
    IsAimedAtTarget = isHit;
    HitResult = MyHitResult;
}

void UWeaponManagerComponent::ProjectileFired(class AActor* ProjectileActor)
{
    this->OnProjectileFireDelegate.Broadcast(ProjectileActor);
}

void UWeaponManagerComponent::StartShooting(EWeaponFunction WeaponFunction)
{
    if(IsReloading)
    {
        if(!IsShooting)
        {
            IsShooting = true;
        }
        return;
    }
    
    if(CurrentWeapon != NULL)
    {
        CurrentWeaponFunction = WeaponFunction;
        
        uint32 Tock = TimerUtil->Tock();
        
        float CurrCadence = CurrentWeapon->WeaponFunctionDefinition().Cadence;
        
        if(!IsShooting){
            if(Tock == 0)
            {
                IsShooting = true;
                TimerUtil->Tick();
                
                GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UWeaponManagerComponent::FireShot, CurrCadence, true, 0.0f);
                return;
            }
        }

        if(!IsShooting && (Tock == 0 || (Tock / 10000000.0f) >= CurrCadence))
        {
            IsShooting = true;
            TimerUtil->Tick();

            GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UWeaponManagerComponent::FireShot, CurrCadence, true, 0.0f);
            if(Tock == 0){
                FireShot();
            }
        }else if(!IsShooting && (Tock / 10000000.0f) < CurrCadence){
            IsShooting = true;
            GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UWeaponManagerComponent::FireShot, CurrCadence, true, CurrCadence - (Tock / 10000000.0f));
        }
    }
}

void UWeaponManagerComponent::StopShooting()
{
    if(IsShooting)
    {
        IsShooting = false;
        GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
    }
    
    if(!CurrentWeapon->ReadyForNewShot)
    {
        CurrentWeapon->ReadyForNewShot = true;
    }
    
    if(CurrentWeapon->ShotAudioComponent)
    {
        CurrentWeapon->ShotAudioComponent->Stop();
        CurrentWeapon->ShotAudioComponent = nullptr;
    }
    
    if(CurrentWeapon->SpawnedRefSec)
    {
        CurrentWeapon->SpawnedRefSec->Destroy();
        CurrentWeapon->SpawnedRefSec = nullptr;
    }
    
    CurrentWeapon->OnWeaponStoppedShootingDelegate.Broadcast(CurrentWeapon);
    
    bool Handled = false;
    CurrentWeapon->OnStoppedShooting(CurrentWeapon->CurrentWeaponFunction, Handled);
    if(!Handled)
    {
        
    }
}

void UWeaponManagerComponent::StartReloading()
{
    if(IsShooting)
    {
        GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
    }
    
    if(IsReloading)
    {
        GetWorld()->GetTimerManager().ClearTimer(ReloadingStartTimerHandle);

        if(CurrentWeapon->WeaponDefinition()->PlayReloadSound)
        {
            UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, CurrentWeapon->WeaponDefinition()->ReloadSound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
        }
        GetWorld()->GetTimerManager().SetTimer(ReloadingEndTimerHandle, this, &UWeaponManagerComponent::FinishReloading, CurrentWeapon->WeaponDefinition()->ReloadTimeout, true, CurrentWeapon->WeaponDefinition()->ReloadTimeout);
    }
}

void UWeaponManagerComponent::FinishReloading()
{
    if(IsReloading)
    {
        IsReloading = false;
        GetWorld()->GetTimerManager().ClearTimer(ReloadingEndTimerHandle);
        
        if(IsShooting)
        {
            GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UWeaponManagerComponent::FireShot, CurrentWeapon->WeaponFunctionDefinition().Cadence, true, 0.0f);
        }
    }
}

void UWeaponManagerComponent::FireShot()
{
    if(IsReloading || !CurrentWeapon->ReadyForNewShot)
    {
        return;
    }
    
    if(CurrentWeapon)
    {
        int32 AmmoCnt = CurrentWeapon->AmmoCount;

        if(AmmoCnt > 0 || AmmoCnt == -1)
        {
            if(AmmoCnt > -1)
            {
                CurrentWeapon->AmmoCount--;
                AmmoCnt = CurrentWeapon->AmmoCount;
            }
            
            CurrentWeapon->FireShot(CurrentWeaponFunction);

            if(AmmoCnt > -1)
            {
                if(AmmoCnt > 0 && (AmmoCnt % CurrentWeapon->WeaponDefinition()->ClipSize) == 0)
                {
                    IsReloading = true;
                    GetWorld()->GetTimerManager().SetTimer(ReloadingStartTimerHandle, this, &UWeaponManagerComponent::StartReloading, 0.01f, true, 0.01f);
                    this->OnWeaponReloading.Broadcast(CurrentWeapon);
                }
            }
        }
        else
        {
            if(CurrentWeapon->WeaponDefinition()->MagEmptySound)
            {
                UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, CurrentWeapon->WeaponDefinition()->MagEmptySound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
            }
        }
    }
}

void UWeaponManagerComponent::ProjectileHit(class AActor* ProjectileActor, class AActor* OtherActor, const FVector Location)
{
    this->OnProjectileHitDelegate.Broadcast(ProjectileActor, OtherActor, Location);
}
