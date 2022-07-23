//
//  WeaponManagerComponent.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//


#include "WeaponSystem/WeaponManagerComponent.h"

DEFINE_LOG_CATEGORY(LogSuake3D);

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
    //	PrimaryComponentTick.bCanEverTick = true;
    if(WeaponTypeTable)
    {
        WeaponTypeTable->RowStruct = FWeaponDefinition::StaticStruct();
    }
}

// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
    
    for(TSubclassOf<UWeaponComponentBase> Weapon: WeaponsArray)
    {
        UWeaponComponentBase* NewWeapon = Cast<UWeaponComponentBase>(Weapon->GetDefaultObject());
        
        UWeaponComponentBase* NewWeaponImpl = NewObject<UWeaponComponentBase>(this, NewWeapon->GetClass(), *NewWeapon->GetClass()->GetName());
            
        NewWeaponImpl->OnProjectileFireDelegate.AddDynamic(this, &UWeaponManagerComponent::ProjectileFired);
        
        NewWeaponImpl->OnProjectileHitDelegate.AddDynamic(this, &UWeaponManagerComponent::ProjectileHit);
            
        WeaponsArrayImpl.AddUnique(NewWeaponImpl);
    }
    
    this->SetCurrentWeapon(1, false);
}

// Called every frame
void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
        
        SetCurrentWeapon(PickedupWeaponID, false);
    }
    
//    if (WeaponsArrayImpl.ContainsByPredicate([&](UWeaponComponentBase* Result) {return PickedupWeaponID == Result->WeaponID(); })) {
//        UDbg::DbgMsg(FString::Printf(TEXT("PickupWeapon EXISTS")), 5.0f, FColor::Green);
//    }
//    else
//    {
//        UDbg::DbgMsg(FString::Printf(TEXT("PickupWeapon DOES NOT EXISTS")), 5.0f, FColor::Green);
////        WeaponsArrayImpl
//    }
}

void UWeaponManagerComponent::SetCurrentWeapon(int32 WeaponID, bool PlayAudio)
{
    if(!CurrentWeapon || CurrentWeapon->WeaponID() != WeaponID)
    {
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
                
                UE_LOG(LogTemp, Warning, TEXT("UWeaponManagerComponent::SetCurrentWeapon: %i ..."), WeaponID);
                
                CurrentWeapon = Weapon;
                
                if(CurrentWeapon)
                {
                    // Get the widget class
                    
                    if(CurrentCSWidget)
                    {
                        CurrentCSWidget->RemoveFromViewport();
                        CurrentCSWidget->Destruct();
                        
                        UDbg::DbgMsg(FString::Printf(TEXT("Removing Current CS Widget")), 5.0f, FColor::Green);
                    }
                    
                    TSubclassOf<class UUserWidget> CSWidgetClass = CurrentWeapon->WeaponDefinition()->CrosshairUserWidget;

                    // ... then to display you widget do

                    // Check if class was found
                        if(CSWidgetClass)
                        {
                            UDbg::DbgMsg(FString::Printf(TEXT("HAS New CS Widget")), 5.0f, FColor::Green);
                            // Create the widget
//                            UUserWidget* CSWidget = CreateWidget<UUserWidget>(GetWorld(), CSWidgetClass, FName(TEXT("CSWidget")));
                            
                            CurrentCSWidget = CreateWidget<UUserWidget>(GetWorld(), CSWidgetClass, FName(TEXT("CSWidget")));
//                            // Check if widget was created
                            if (CurrentCSWidget)
                            {
                                UDbg::DbgMsg(FString::Printf(TEXT("Adding Current CS Widget")), 5.0f, FColor::Green);
                                CurrentCSWidget->AddToViewport();
//                                bShowMouseCursor = true;
                            }
                        }
                }
                
                if(WeaponChangeSound && PlayAudio)
                {
                    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, this->WeaponChangeSound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
                }
//                if(IsShooting)
//                {
//                    GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UWeaponManagerComponent::FireShot, CurrentWeapon->WeaponDefinition()->Cadence, true, 0.0f);
//                }
                break;
            }
        }
    }
}

void UWeaponManagerComponent::ProjectileFired(class AActor* ProjectileActor)
{
    UE_LOG(LogSuake3D, Warning, TEXT("UA_WeaponManagerComponent::ProjectileFired() !!!!!!!"));
    
    this->OnProjectileFireDelegate.Broadcast(ProjectileActor);
}

void UWeaponManagerComponent::StartShooting(EWeaponFunction WeaponFunction)
{
    
    
    UE_LOG(LogTemp, Warning, TEXT("UWeaponManagerComponent::StartShooting()"));
    
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
//        CurrentWeapon->MuzzleOffset = MuzzleOffset;
        uint32 Tock = TimerUtil->Tock();

        if(!IsShooting){
            if(Tock == 0)
            {
                UE_LOG(LogSuake3D, Warning, TEXT("Tock == 0 => Firing first shot after change / init!"));
                IsShooting = true;
                TimerUtil->Tick();
                GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UWeaponManagerComponent::FireShot, CurrentWeapon->WeaponDefinition()->Cadence, true, 0.0f);
                return;
            }
        }

        if(!IsShooting && (Tock == 0 || (Tock / 10000000.0f) >= CurrentWeapon->WeaponDefinition()->Cadence))
        {
            IsShooting = true;
            TimerUtil->Tick();

            GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UWeaponManagerComponent::FireShot, CurrentWeapon->WeaponDefinition()->Cadence, true, 0.0f);
            if(Tock == 0){
                FireShot();
            }
        }else if(!IsShooting && (Tock / 10000000.0f) < CurrentWeapon->WeaponDefinition()->Cadence){
            IsShooting = true;
            GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UWeaponManagerComponent::FireShot, CurrentWeapon->WeaponDefinition()->Cadence, true, CurrentWeapon->WeaponDefinition()->Cadence - (Tock / 10000000.0f));
        }
    }
}

void UWeaponManagerComponent::StopShooting()
{
    UDbg::DbgMsg(FString(TEXT("STOPPING SHOOTING")), 5.0f, FColor::Yellow);
    if(IsShooting)
    {
        IsShooting = false;
        GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
    }
    
    if(!CurrentWeapon->ReadyForNewShot)
    {
        CurrentWeapon->ReadyForNewShot = true;
    }
    
    //OnWeaponStoppedShootingDelegate.Broadcast(CurrentWeapon);
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
}

void UWeaponManagerComponent::StartReloading()
{
    if(IsShooting)
    {
        GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
    }
    
    if(IsReloading)
    {
//        IsReloading = true;
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
            GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UWeaponManagerComponent::FireShot, CurrentWeapon->WeaponDefinition()->Cadence, true, 0.0f);
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
            
//            CurrentWeapon->MuzzleOffset = GetComponentLocation();
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
    UE_LOG(LogSuake3D, Warning, TEXT("UWeaponManagerComponent::ProjectileHit() !!!"));
    
    this->OnProjectileHitDelegate.Broadcast(ProjectileActor, OtherActor, Location);
}
