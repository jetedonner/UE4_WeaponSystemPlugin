//
//  CrosshairUserWidgetBase.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 05.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/Crosshair/CrosshairUserWidgetBase.h"

//UCrosshairUserWidgetBase::UCrosshairUserWidgetBase():Super()
//{
//    
//}

//void UCrosshairUserWidgetBase::BeginPlay()
//{
//    Super::BeginPlay();
//}

void UCrosshairUserWidgetBase::NativeConstruct()
{
    FillAnimationsMap();
    
    Super::NativeConstruct();
}

void UCrosshairUserWidgetBase::OnAnimateCrosshair_Implementation(bool AlternativeAnimation)
{
    
}

void UCrosshairUserWidgetBase::FillAnimationsMap()
{
    AnimationsMap.Empty();
    
    FProperty* Prop = GetClass()->PropertyLink;

    // Run through all properties of this class to find any widget animations
    while (Prop != nullptr)
    {
        // Only interested in object properties
        if (Prop->GetClass() == FObjectProperty::StaticClass())
        {
            FObjectProperty* ObjProp = CastField<FObjectProperty>(Prop);

            // Only want the properties that are widget animations
            if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
            {
                UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

                UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);

                if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
                {
                    FName AnimName = WidgetAnim->MovieScene->GetFName();
                    AnimationsMap.Add(AnimName, WidgetAnim);
                    
                    UDbg::DbgMsg(FString::Printf(TEXT("Adding Widget Animation to Map => %s"), *AnimName.ToString()), 5.0f, FColor::Red);
                }
            }
        }

        Prop = Prop->PropertyLinkNext;
    }
}

UWidgetAnimation* UCrosshairUserWidgetBase::GetAnimationByName(FName AnimationName) const
{
    UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimationName);
    if (WidgetAnim)
    {
        return *WidgetAnim;
    }
    return nullptr;
}


bool UCrosshairUserWidgetBase::PlayAnimationByName(FName AnimationName,
    float StartAtTime,
    int32 NumLoopsToPlay,
    EUMGSequencePlayMode::Type PlayMode,
    float PlaybackSpeed)
{
    UWidgetAnimation* WidgetAnim = GetAnimationByName(AnimationName);
    if (WidgetAnim)
    {
        PlayAnimation(WidgetAnim, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
        return true;
    }
    return false;
}

void UCrosshairUserWidgetBase::PlayAimedAtAnimation(bool AlternativeAnimation)
{
    EUMGSequencePlayMode::Type PlayMode = (AlternativeAnimation ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
    PlayAnimationByName(AimedAnimationName, 0.0f, 1, PlayMode, 1.0f);
}

//void UCrosshairUserWidgetBase::DrawLine(FPaintContext Context, FVector2D PositionA, FVector2D PositionB) const
//{
//    UWidgetBlueprintLibrary::DrawLine(Context, PositionA, PositionB, Tint, bAntiAlias, Thickness);
//}
