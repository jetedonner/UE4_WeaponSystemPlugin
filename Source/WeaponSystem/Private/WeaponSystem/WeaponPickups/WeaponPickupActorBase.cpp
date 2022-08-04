// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/WeaponPickups/WeaponPickupActorBase.h"

// Sets default values
AWeaponPickupActorBase::AWeaponPickupActorBase()
{
    PrimaryActorTick.bCanEverTick = true;
    
    SetPickUpCount();
//    if(PickUpCount == 0)
//    {
//        if(!WeaponType.IsNull())
//        {
//            FWeaponDefinition* MyWeaponDefinition = WeaponType.GetRow<FWeaponDefinition>("");
//
//            if(MyWeaponDefinition)
//            {
//                PickUpCount = MyWeaponDefinition->PickUpCount;
//            }
//        }
//    }
}

AWeaponPickupActorBase::AWeaponPickupActorBase(const FObjectInitializer& ObjectInitializer) {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    if(!CollisionSphere)
    {
        CollisionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision Sphere Component"));
        CollisionSphere->InitSphereRadius(CollisionRadius);
        CollisionSphere->SetMobility(EComponentMobility::Movable);
        //CollisionSphere->SetSimulatePhysics(true);
        CollisionSphere->SetNotifyRigidBodyCollision(true);
        CollisionSphere->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
        CollisionSphere->SetRelativeLocation_Direct(FVector(0.0f, 0.0f, CollisionRadius));
        CollisionSphere->SetupAttachment(RootComponent);
//        RootComponent = CollisionSphere;
    }
    
    if(!StaticMeshComponent){
        StaticMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Weapon Pickup Mesh"));
        StaticMeshComponent->SetMobility(EComponentMobility::Movable);
        StaticMeshComponent->SetupAttachment(CollisionSphere);
//        RootComponent = StaticMeshComponent;
    }
    
    if(!ParticleSystemComponent)
    {
        ParticleSystemComponent = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleSystemComponent"));
        //ParticleSystemComponent->SetMobility(EComponentMobility::Movable);
        ParticleSystemComponent->SetRelativeLocation_Direct(FVector(0.0f, 0.0f, -CollisionRadius));
        ParticleSystemComponent->SetupAttachment(CollisionSphere);
    }

    if(!RotatingMovement)
    {
        // Create movement component and set rotation rate
        RotatingMovement = ObjectInitializer.CreateDefaultSubobject<URotatingMovementComponent>(this, TEXT("RotatingMovement"));
        RotatingMovement->SetUpdatedComponent(StaticMeshComponent);
        RotatingMovement->RotationRate = RotationRate;
    }
    
    SetPickUpCount();
    
//    if(PickUpCount == 0)
//    {
//        if(!WeaponType.IsNull())
//        {
//            FWeaponDefinition* MyWeaponDefinition = WeaponType.GetRow<FWeaponDefinition>("");
//
//            if(MyWeaponDefinition)
//            {
//                PickUpCount = MyWeaponDefinition->PickUpCount;
//            }
//        }
//    }
}

void AWeaponPickupActorBase::SetPickUpCount()
{
    if(PickUpCount == 0)
    {
        if(!WeaponType.IsNull())
        {
            FWeaponDefinition* MyWeaponDefinition = WeaponType.GetRow<FWeaponDefinition>("");
            
            if(MyWeaponDefinition)
            {
                PickUpCount = MyWeaponDefinition->PickUpCount;
            }
        }
    }
}

void AWeaponPickupActorBase::BeginPlay()
{
    if(RotatingMovement)
    {
        RotatingMovement->RotationRate = RotationRate;
    }
    
    CollisionSphere->OnComponentHit.AddDynamic(this, &AWeaponPickupActorBase::OnHit);
    
//    if(PickUpCount == 0)
//    {
//        if(!WeaponType.IsNull())
//        {
//            FWeaponDefinition* MyWeaponDefinition = WeaponType.GetRow<FWeaponDefinition>("");
//            
//            if(MyWeaponDefinition)
//            {
//                PickUpCount = MyWeaponDefinition->PickUpCount;
//            }
//        }
//    }
    
    Super::BeginPlay();
}

void AWeaponPickupActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponPickupActorBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
//    UDbg::DbgMsg(FString::Printf(TEXT("AWeaponPickupActorBase::OnHit(...) by => %s"), *OtherActor->GetName()), 5.0f, FColor::Red);
    
    AWeaponSystemCharacterBase* PickupCharacter = Cast<AWeaponSystemCharacterBase>(OtherActor);
    if(PickupCharacter)
    {
        this->OnPickup(PickupCharacter);
//        FWeaponDefinition* MyWeaponDefinition = WeaponType.GetRow<FWeaponDefinition>("");
//
//        if(MyWeaponDefinition)
//        {
//            PickupCharacter->WeaponManagerComponent->PickupWeapon(MyWeaponDefinition->WeaponID, PickUpCount);
//            if(PickupSound)
//            {
//                UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation(), FRotator::ZeroRotator, 1.0, 1.0, 0.0f, nullptr, nullptr, true);
//            }
//
//            if(PickupEffect)
//            {
//                FVector PickupEffectLocation = GetActorLocation();
//                PickupEffectLocation.Z -= CollisionRadius;
//
//                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupEffect, PickupEffectLocation, FRotator(1), true, EPSCPoolMethod::AutoRelease, true);
//
//                UDbg::DbgMsg(FString::Printf(TEXT("PickupEffect FOUND")), 5.0f, FColor::Red, true);
//            }
//            else
//            {
//                UDbg::DbgMsg(FString::Printf(TEXT("PickupEffect NOT Found")), 5.0f, FColor::Red);
//            }
//
//            this->Destroy();
//        }
//        else
//        {
//            UDbg::DbgMsg(FString::Printf(TEXT("WeaponDefinition NOT FOUND (You must specify a WeaponDefinition for this Weapon!) => %s"), *WeaponType.RowName.ToString()), 5.0f, FColor::Red);
//        }
    }
    else
    {
        UDbg::DbgMsg(FString::Printf(TEXT("PickupCharacter NOT Found")), 5.0f, FColor::Red);
        AWeaponSystemProjectileBase* PickupProjectile = Cast<AWeaponSystemProjectileBase>(OtherActor);
        if(PickupProjectile)
        {
            UDbg::DbgMsg(FString::Printf(TEXT("AWeaponPickupActorBase::OnHit(...) PROJECTILE => %s"), *OtherActor->GetName()), 5.0f, FColor::Red);
//            AWeaponSystemCharacterBase* PickupCharacter = Cast<AWeaponSystemCharacterBase>(GetWorld()->GetFirstPlayerController());
            
            APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
            
            this->OnPickup(PlayerPawn);
        }
        else
        {
            UDbg::DbgMsg(FString::Printf(TEXT("Hit Pickup is NOT a CHARACTER and NOT a PROJECTILE")), 5.0f, FColor::Red);
        }
    }
}

void AWeaponPickupActorBase::OnPickup(AActor* OtherActor)
{
    AWeaponSystemCharacterBase* PickupCharacter = Cast<AWeaponSystemCharacterBase>(OtherActor);
    if(PickupCharacter)
    {
        FWeaponDefinition* MyWeaponDefinition = WeaponType.GetRow<FWeaponDefinition>("");
        
        if(MyWeaponDefinition)
        {
            PickupCharacter->WeaponManagerComponent->PickupWeapon(MyWeaponDefinition->WeaponID, PickUpCount);
            if(PickupSound)
            {
                UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation(), FRotator::ZeroRotator, 1.0, 1.0, 0.0f, nullptr, nullptr, true);
            }
            
            if(PickupEffect)
            {
                FVector PickupEffectLocation = GetActorLocation();
                PickupEffectLocation.Z -= CollisionRadius;
                
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupEffect, PickupEffectLocation, FRotator(1), true, EPSCPoolMethod::AutoRelease, true);
                
                UDbg::DbgMsg(FString::Printf(TEXT("PickupEffect FOUND")), 5.0f, FColor::Red, true);
            }
            else
            {
                UDbg::DbgMsg(FString::Printf(TEXT("PickupEffect NOT Found")), 5.0f, FColor::Red);
            }
            
            this->Destroy();
        }
        else
        {
            UDbg::DbgMsg(FString::Printf(TEXT("WeaponDefinition NOT FOUND (You must specify a WeaponDefinition for this Weapon!) => %s"), *WeaponType.RowName.ToString()), 5.0f, FColor::Red);
        }
    }
    else
    {
        UDbg::DbgMsg(FString::Printf(TEXT("PickupCharacter NOT NOT Found => %s"), *OtherActor->GetName()), 5.0f, FColor::Yellow);
//        AWeaponSystemProjectileBase* PickupProjectile = Cast<AWeaponSystemProjectileBase>(OtherActor);
//        if(PickupProjectile)
//        {
//            AWeaponSystemCharacterBase* PickupCharacter = Cast<AWeaponSystemCharacterBase>(GetWorld()->GetFirstPlayerController());
//        }
//        else
//        {
//            UDbg::DbgMsg(FString::Printf(TEXT("Hit Pickup is NOT a CHARACTER and NOT a PROJECTILE")), 5.0f, FColor::Red);
//        }
    }
}
