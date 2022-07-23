//
//  WeaponSystemProjectileBase.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/WeaponSystemProjectileBase.h"

AWeaponSystemProjectileBase::AWeaponSystemProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

AWeaponSystemProjectileBase::AWeaponSystemProjectileBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    
    if(!CollisionSphere)
    {
        CollisionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision Sphere Component"));
        CollisionSphere->InitSphereRadius(20.0f);
        CollisionSphere->SetMobility(EComponentMobility::Movable);
        CollisionSphere->SetSimulatePhysics(true);
        CollisionSphere->SetNotifyRigidBodyCollision(true);
        CollisionSphere->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
        CollisionSphere->SetCollisionProfileName(TEXT("Projectile"));
        RootComponent = CollisionSphere;
    }
    
    if(!StaticMeshComponent){
        StaticMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Projectile Mesh"));
        StaticMeshComponent->SetMobility(EComponentMobility::Movable);
        StaticMeshComponent->SetupAttachment(CollisionSphere);
    }
    
    if(!ProjectileMovement)
    {
        ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>( this, TEXT( "ProjectileMovementComp" ) );
        ProjectileMovement->UpdatedComponent = CollisionSphere;
        ProjectileMovement->SetUpdatedComponent(CollisionSphere);
        ProjectileMovement->InitialSpeed = 12000.f;
        ProjectileMovement->MaxSpeed = 12000.f;
        ProjectileMovement->bRotationFollowsVelocity = true;
        ProjectileMovement->bShouldBounce = false;
        ProjectileMovement->ProjectileGravityScale = 0.0f;
    }
}

void AWeaponSystemProjectileBase::BeginPlay()
{
	Super::BeginPlay();
    CollisionSphere->OnComponentHit.AddDynamic(this, &AWeaponSystemProjectileBase::OnHit);
    CollisionSphere->SetGenerateOverlapEvents(true);
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponSystemProjectileBase::BeginOverlap);
}

void AWeaponSystemProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponSystemProjectileBase::FireInDirection(const FVector& ShootDirection, bool& WasHandled)
{
    if(OnProjectileCustomFireDelegate.IsBound())
    {
        UDbg::DbgMsg(FString::Printf(TEXT("OnProjectileCustomFireDelegate.IsBound()")), 5.0f, FColor::Red);
        // Hello stay your man and wait for it!!!
        this->OnProjectileCustomFireDelegate.Broadcast(this, ShootDirection);
        WasHandled = true;
    }
    else
    {
        ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
        
        if(ShotSound)
        {
            UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, ShotSound, GetActorLocation(), FRotator::ZeroRotator, 1.0, 1.0, 0.0f, nullptr, nullptr, true);
        }
        WasHandled = false;
    }
}

void AWeaponSystemProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    AHitableActorBase* TestActor = Cast<AHitableActorBase>(OtherActor);
    FString ResultStr = "No";
    if(TestActor && (OtherComp->GetName().Equals(TEXT("BoxCollision")) || OtherComp->GetName().Equals(TEXT("SphereCollision")) || OtherComp->GetName().Equals(TEXT("MeshComponent"))))
    {
        ResultStr = "Yes";
        TestActor->ExecActorHitHandler(this, Hit);
    }

    if(OtherActor->GetClass() != this->GetClass())
    {
        CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
        if(OtherActor->CanBeDamaged())
        {
            UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, ImpactTargetSound, Hit.Location, FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
        }
        else
        {
            UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, ImpactFailSound, Hit.Location, FRotator::ZeroRotator, 2.0, 1.0, 0.0f, nullptr, nullptr, true);
        }
        
        if(ImpactEffect && ImpactEffect->IsValid())
        {
            UNiagaraComponent* NewEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                this,
                ImpactEffect,
                GetActorLocation(),
                FRotator(1),
                FVector(1),
                true,
                true,
                ENCPoolMethod::AutoRelease,
                true);
        }
        else if(ImpactEffectOld)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffectOld, GetActorLocation(), FRotator(1), true, EPSCPoolMethod::AutoRelease, true);
        }
        
        UGameplayStatics::ApplyDamage(OtherActor, WeaponTypeDef.DataTable->FindRow<FWeaponDefinition>(WeaponTypeDef.RowName, "")->DamageFactor, GetWorld()->GetFirstPlayerController(), this, UDamageType::StaticClass());
        
//        this->OnProjectileHitDelegate.Broadcast(this, OtherActor, Hit.Location);
        
        if(ImpactDecalMaterialStructs.Num() > 0)
        {
            int32 Idx = FMath::RandRange(0, ImpactDecalMaterialStructs.Num() - 1);
            
            UMaterialInterface* Decal = Cast<UMaterialInterface>(ImpactDecalMaterialStructs[Idx].ImpactDecalMaterial);
            
            FVector DecalSize = ImpactDecalMaterialStructs[Idx].DecalSize;
            FVector DecalLocation = Hit.ImpactPoint;
            
            FRotator RandomDecalRotation = UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal);
            RandomDecalRotation.Roll += FMath::RandRange(-180.0f, 180.0f);
            
            
            float RandDecalLifeSpan = FMath::RandRange(DecalLifeSpanMin, DecalLifeSpanMax);
            
            ImpactDecalObject = UGameplayStatics::SpawnDecalAttached(Decal, DecalSize, OtherComp, "", DecalLocation, RandomDecalRotation, EAttachLocation::KeepWorldPosition, RandDecalLifeSpan);
            
//            float DefDecalFadeOutDuration = 0.0f;
            if(DecalFadeOutEffect)
            {
//                DefDecalFadeOutDuration = DecalFadeOutDuration;
                ImpactDecalObject->SetFadeOut(RandDecalLifeSpan - DecalFadeOutDuration, DecalFadeOutDuration, false);
            }
//            ImpactDecalObject->SetFadeScreenSize(0.00001f);
//            ImpactDecalObject->FadeScreenSize = 0.00001f;
        }
        
        this->OnProjectileHitDelegate.Broadcast(this, OtherActor, Hit.Location);
        
        if(DestroyOnHit)
        {
            this->Destroy();
        }
    }
    else
    {
        this->Destroy();
    }
}

void AWeaponSystemProjectileBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
   // Overlap
    UDbg::DbgMsg(FString::Printf(TEXT("Projectile BeginOverlap => Yesss!!!")), 5.0f, FColor::Green);
}

void AWeaponSystemProjectileBase::OutsideWorldBounds()
{
    Super::OutsideWorldBounds();
    this->Destroy();
}
