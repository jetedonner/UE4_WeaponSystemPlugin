//
//  CircleCrosshairUserWidget.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 09.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/Crosshair/CircleCrosshairUserWidget.h"

void UCircleCrosshairUserWidget::NativeConstruct()
{
    FillAnimationsMap();
    
    Super::NativeConstruct();
    
    FVector2D MySize = FVector2D(Radius * 2, Radius * 2);
//    DesignTimeSize = MySize; //FVector2D(Radius * 2, Radius * 2);
    SetDesiredSizeInViewport(MySize);
}

int32 UCircleCrosshairUserWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
    
//    FVector2D Size = GetDesiredSize();
//    UDbg::DbgMsg(FString::Printf(TEXT("ViewPortSize => %f / %f"), Size.X, Size.Y), 5.0f, FColor::Red);
    
    FVector2D CenterPositionNGNG = FVector2D(Radius + (Thickness), Radius + (Thickness));
    
    FVector2D VectForRotation = FVector2D(0.0f, Radius + (Thickness));
    
    float PartDegrees = 360.0f / NumSegments;
    
    for (int i = 0; i < NumSegments; i++) {
        
        FVector2D PositionA = UKismetMathLibrary::GetRotated2D(VectForRotation, PartDegrees * (i + 1)) + CenterPositionNGNG;
        
        FVector2D PositionB = UKismetMathLibrary::GetRotated2D(VectForRotation, PartDegrees * i) + CenterPositionNGNG;
        
        UWidgetBlueprintLibrary::DrawLine(Context, PositionA, PositionB, Tint, bAntiAlias, Thickness);
    }

    return LayerId;
}

void UCircleCrosshairUserWidget::FillAnimationsMap()
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

UWidgetAnimation* UCircleCrosshairUserWidget::GetAnimationByName(FName AnimationName) const
{
    UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimationName);
    if (WidgetAnim)
    {
        return *WidgetAnim;
    }
    return nullptr;
}


bool UCircleCrosshairUserWidget::PlayAnimationByName(FName AnimationName,
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
