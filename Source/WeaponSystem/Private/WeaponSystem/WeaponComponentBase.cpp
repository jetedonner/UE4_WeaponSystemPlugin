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
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.SetTickFunctionEnable(true);
    bAutoActivate = true;
    bAllowReregistration = true;
    bAutoRegister = true;
    
//    SaveConfig();
//    LoadConfig();
}

UWeaponComponentBase::UWeaponComponentBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.SetTickFunctionEnable(true);
    bAutoActivate = true;
    bAllowReregistration = true;
    bAutoRegister = true;
    
//    SaveConfig();
//    LoadConfig();
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
    ExecFireShotNG(WeaponFunction);
}

void UWeaponComponentBase::OnStoppedShooting_Implementation(EWeaponFunction WeaponFunction, bool& Handled){
    Handled = true;
    
    UDbg::DbgMsg(FString::Printf(TEXT("UWeaponComponentBase::OnStoppedShooting_Implementation")), 5.0f, FColor::Red);
    //ExecFireShot(WeaponFunction);
}

//FRotator MuzzleRotation

void UWeaponComponentBase::GetMuzzleRotationInt(FRotator& MuzzleRotationRet, FVector& MuzzleLocRet)
{
    FVector  CameraLoc;
    FRotator CameraRot;
    
    AActor* ActorRef = GetWorld()->GetFirstPlayerController()->GetPawn();
    AWeaponSystemCharacterBase* WSActor = Cast<AWeaponSystemCharacterBase>(ActorRef);
    ActorRef->GetActorEyesViewPoint(CameraLoc, CameraRot);
    CameraLoc = ActorRef->GetActorLocation();
    
    if(WSActor)
    {
        CameraLoc = WSActor->FirstPersonCamera->GetComponentLocation();
        CameraLoc += WSActor->FirstPersonCamera->GetForwardVector() * 25.0f;
    }
    
    FVector MuzzleOffset;
    MuzzleOffset.Set(0.0f, 0.0f, 0.0f);
    MuzzleLocRet = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
    MuzzleRotationRet = CameraRot;
}

void UWeaponComponentBase::ExecFireShotNG(EWeaponFunction WeaponFunction)
{
    FVector  MyMuzzleLocation;
    FRotator MyMuzzleRotation;
    
    GetMuzzleRotationInt(MyMuzzleRotation, MyMuzzleLocation);
    
    AActor* ActorRef = GetWorld()->GetFirstPlayerController()->GetPawn();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = ActorRef;
    SpawnParams.Instigator = ActorRef->GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    if(WeaponFunction == EWeaponFunction::Primary)
    {
//        return;
    }
    else
    {
        
        AWeaponSystemProjectileBase* SpawnedRef = (AWeaponSystemProjectileBase*) GetWorld()->SpawnActor<AWeaponSystemProjectileBase>(WeaponDefinition()->SecondaryWeaponFunctionDefinition.Projectile, MyMuzzleLocation, MyMuzzleRotation, SpawnParams);
        
        UDbg::DbgMsg(FString::Printf(TEXT("UWeaponComponentBase: -> SpawnActor Vector: %s"), *MyMuzzleLocation.ToString()), 5.0f, FColor::Red);
        
        if(SpawnedRef)
        {
//            FVector origin;
//            FVector boxExtent;
//            SpawnedRef->GetActorBounds(false, origin, boxExtent);
            
            FVector LaunchDirection = MyMuzzleRotation.Vector();
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
}

void UWeaponComponentBase::ExecFireShot(EWeaponFunction WeaponFunction)
{
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
        CameraLoc += WSActor->FirstPersonCamera->GetForwardVector() * 25.0f;
    }
    
    FRotator MuzzleRotationRetNG = CameraRot;//MuzzleRotation;//CameraRot;
    
    FVector MuzzleOffset;
    MuzzleOffset.Set(0.0f, 0.0f, 0.0f);
    FVector MuzzleLocationRetNG = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = ActorRef;
    SpawnParams.Instigator = ActorRef->GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    if(WeaponFunction == EWeaponFunction::Secondary)
    {
        return;
    }
//        
////        AWeaponSystemProjectileBase* SpawnedRefSecLoc = (AWeaponSystemProjectileBase*) GetWorld()->SpawnActor<AWeaponSystemProjectileBase>(ProjectileSecondary, MuzzleLocation, CameraRot, SpawnParams);
//        if(!ProjectileSecondary)
//        {
//            return;
//        }
//        
//        this->SpawnedRefSec = (AWeaponSystemProjectileBase*) GetWorld()->SpawnActor<AWeaponSystemProjectileBase>(ProjectileSecondary, MuzzleLocationRetNG, CameraRot, SpawnParams);
//        
//        if(!SpawnedRefSec->AutomaticRecharge)
//        {
//            ReadyForNewShot = false;
//        }
//        
//        if(SpawnedRefSec->ShotSound)
//        {
//            UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, SpawnedRefSec->ShotSound, SpawnedRefSec->GetActorLocation(), FRotator::ZeroRotator, 1.0, 1.0, 0.0f, nullptr, nullptr, true);
//            ShotAudioComponent = AudioComponent;
//        }
//        
//        SpawnedRefSec->OnProjectileHitDelegate.AddDynamic(this, &UWeaponComponentBase::ProjectileHit);
//        
//        this->OnProjectileFireDelegate.Broadcast(SpawnedRefSec);
//        
//        UDbg::DbgMsg(FString(TEXT("SHOOTING SECONDARY => CALL EVENT!!!")), 5.0f, FColor::Green);
//        return;
//    }
    
    AWeaponSystemProjectileBase* SpawnedRef = (AWeaponSystemProjectileBase*) GetWorld()->SpawnActor<AWeaponSystemProjectileBase>(Projectile, MuzzleLocationRetNG, CameraRot, SpawnParams);
    
    UDbg::DbgMsg(FString::Printf(TEXT("UWeaponComponentBase: -> SpawnActor Vector: %s"), *MuzzleLocationRetNG.ToString()), 5.0f, FColor::Red);
    
    if(SpawnedRef == NULL)
    {
        UDbg::DbgMsg(FString::Printf(TEXT("We have NO new SPAWNED OBJECT")), 5.0f, FColor::Red);
    }
    else
    {
        FVector origin;
        FVector boxExtent;
        SpawnedRef->GetActorBounds(false, origin, boxExtent);
        
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
