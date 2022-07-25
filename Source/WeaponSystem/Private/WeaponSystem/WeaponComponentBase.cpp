//
//  WeaponComponentBase.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/WeaponComponentBase.h"

UWeaponComponentBase::UWeaponComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UWeaponComponentBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
//void UWeaponComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

int32 UWeaponComponentBase::WeaponID()
{
    return WeaponDefinition()->WeaponID;
}

void UWeaponComponentBase::GetWeaponID(int32& WeaponID)
{
    WeaponID = this->WeaponID();
}

FWeaponDefinition* UWeaponComponentBase::WeaponDefinition()
{
    return WeaponTypeDef.DataTable->FindRow<FWeaponDefinition>(WeaponTypeDef.RowName, "");
}

void UWeaponComponentBase::GetWeaponDefinition(FWeaponDefinition& WeaponDef)
{
    WeaponDef = *this->WeaponDefinition();
}

void UWeaponComponentBase::FireShot(EWeaponFunction WeaponFunction)
{
    this->CurrentWeaponFunction = WeaponFunction;
    bool Handled = false;
    this->OnStartedShooting(WeaponFunction, Handled);
    if(!Handled)
    {
        ExecFireShot(WeaponFunction);
    }
}
    
void UWeaponComponentBase::OnStartedShooting_Implementation(EWeaponFunction WeaponFunction, bool& Handled){
    Handled = true;
    
    UDbg::DbgMsg(FString::Printf(TEXT("UWeaponComponentBase::OnStartedShooting_Implementation")), 5.0f, FColor::Purple);
    ExecFireShot(WeaponFunction);
}

void UWeaponComponentBase::OnStoppedShooting_Implementation(EWeaponFunction WeaponFunction, bool& Handled){
    Handled = true;
    
    UDbg::DbgMsg(FString::Printf(TEXT("UWeaponComponentBase::OnStoppedShooting_Implementation")), 5.0f, FColor::Red);
    //ExecFireShot(WeaponFunction);
}

//FRotator MuzzleRotation

void UWeaponComponentBase::GetMuzzleRotationInt(FRotator& MuzzleRotationRet)
{
    FVector  CameraLoc;
    FRotator CameraRot;
    
    AActor* ActorRef = GetWorld()->GetFirstPlayerController()->GetPawn();
    
    AWeaponSystemCharacterBase* WSActor = Cast<AWeaponSystemCharacterBase>(ActorRef);
    
    ActorRef->GetActorEyesViewPoint(CameraLoc, CameraRot);
    MuzzleRotationRet = CameraRot;
//    return CameraRot;
}

void UWeaponComponentBase::ExecFireShot(EWeaponFunction WeaponFunction)
{
    UE_LOG(LogSuake3D, Warning, TEXT("UWeaponComponentBase::FireShot() !!!!!!!"));
    
    FVector  CameraLoc;
    FRotator CameraRot;
    
    AActor* ActorRef = GetWorld()->GetFirstPlayerController()->GetPawn();
    
    AWeaponSystemCharacterBase* WSActor = Cast<AWeaponSystemCharacterBase>(ActorRef);
    
    ActorRef->GetActorEyesViewPoint(CameraLoc, CameraRot);
    
    CameraLoc = ActorRef->GetActorLocation();
//    CameraRot = ActorRef->GetActorRotation();
    
    if(WSActor)
    {
//        WSActor->FirstPersonCamera;
        CameraLoc = WSActor->FirstPersonCamera->GetComponentLocation();
        CameraLoc += WSActor->FirstPersonCamera->GetForwardVector() * 205.0f;
    }
    
    FRotator MuzzleRotationRetNG = CameraRot;//MuzzleRotation;//CameraRot;
    
    FVector MuzzleOffset;
    MuzzleOffset.Set(0.0f, 0.0f, 0.0f);
    FVector MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = ActorRef;
    SpawnParams.Instigator = ActorRef->GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
//    UE_LOG(LogSuake3D, Warning, TEXT("MuzzleOffset: %s"), *MuzzleOffset.ToString());
//    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MuzzleOffset: %s"), *MuzzleOffset.ToString()));
//    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MuzzleLocation: %s"), *MuzzleLocation.ToString()));
    
    if(WeaponFunction == EWeaponFunction::Secondary)
    {
        return;
        
//        AWeaponSystemProjectileBase* SpawnedRefSecLoc = (AWeaponSystemProjectileBase*) GetWorld()->SpawnActor<AWeaponSystemProjectileBase>(ProjectileSecondary, MuzzleLocation, CameraRot, SpawnParams);
        if(!ProjectileSecondary)
        {
            return;
        }
        
        this->SpawnedRefSec = (AWeaponSystemProjectileBase*) GetWorld()->SpawnActor<AWeaponSystemProjectileBase>(ProjectileSecondary, MuzzleLocation, CameraRot, SpawnParams);
        
        if(!SpawnedRefSec->AutomaticRecharge)
        {
            ReadyForNewShot = false;
        }
        
        if(SpawnedRefSec->ShotSound)
        {
            UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, SpawnedRefSec->ShotSound, SpawnedRefSec->GetActorLocation(), FRotator::ZeroRotator, 1.0, 1.0, 0.0f, nullptr, nullptr, true);
            ShotAudioComponent = AudioComponent;
        }
        
        SpawnedRefSec->OnProjectileHitDelegate.AddDynamic(this, &UWeaponComponentBase::ProjectileHit);
        
        this->OnProjectileFireDelegate.Broadcast(SpawnedRefSec);
        
        UDbg::DbgMsg(FString(TEXT("SHOOTING SECONDARY => CALL EVENT!!!")), 5.0f, FColor::Green);
        return;
    }
    
    AWeaponSystemProjectileBase* SpawnedRef = (AWeaponSystemProjectileBase*) GetWorld()->SpawnActor<AWeaponSystemProjectileBase>(Projectile, MuzzleLocation, CameraRot, SpawnParams);
    
    if(SpawnedRef == NULL)
    {
        UE_LOG(LogSuake3D, Warning, TEXT("WE HAVENT GOT SpawnedRef OBJECT!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("WE HAVENT GOT SpawnedRef OBJECT!")));
    }
    else
    {
        FVector origin;
        FVector boxExtent;
        SpawnedRef->GetActorBounds(false, origin, boxExtent);
        
//        UE_LOG(LogSuake3D, Warning, TEXT("Projectile origin: %s"), *origin.ToString());
//        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Projectile origin: %s"), *origin.ToString()));
//
//        UE_LOG(LogSuake3D, Warning, TEXT("Projectile box: %s"), *boxExtent.ToString());
//        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Projectile box: %s"), *boxExtent.ToString()));
        
        FVector LaunchDirection = MuzzleRotationRetNG.Vector();
        SpawnedRef->OnProjectileHitDelegate.AddDynamic(this, &UWeaponComponentBase::ProjectileHit);
        bool WasHandled = false;
        SpawnedRef->FireInDirection(LaunchDirection, WasHandled);
        if(!WasHandled)
        {
            this->OnProjectileFireDelegate.Broadcast(SpawnedRef);
            //UDbg::DbgMsg(FString::Printf(TEXT("FireInDirection => HANDLED!!!")), 5.0f, FColor::Green);
        }
        else
        {
            UDbg::DbgMsg(FString::Printf(TEXT("FireInDirection => NOT HANDLED!!!")), 5.0f, FColor::Green);
            SpawnedRef->Destroy();
        }
    }
}

void UWeaponComponentBase::ProjectileHit(class AActor* ProjectileActor, class AActor* OtherActor, const FVector Location)
{
    UE_LOG(LogSuake3D, Warning, TEXT("UWeaponComponentBase::ProjectileHit() !!!"));
    this->OnProjectileHitDelegate.Broadcast(ProjectileActor, OtherActor, Location);
}

void UWeaponComponentBase::StartReloading()
{
//    if(IsShooting)
//    {
//        GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
//    }
    
    if(IsReloading)
    {
        GetWorld()->GetTimerManager().ClearTimer(ReloadingStartTimerHandle);
        
        if(WeaponDefinition()->PlayReloadSound)
        {
            UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, WeaponDefinition()->ReloadSound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
        }
        GetWorld()->GetTimerManager().SetTimer(ReloadingEndTimerHandle, this, &UWeaponComponentBase::FinishReloading, WeaponDefinition()->ReloadTimeout, true, WeaponDefinition()->ReloadTimeout);
    }
}

void UWeaponComponentBase::FinishReloading()
{
    if(IsReloading)
    {
        IsReloading = false;
        ReadyForNewShot = true;
        GetWorld()->GetTimerManager().ClearTimer(ReloadingEndTimerHandle);
    }
}
