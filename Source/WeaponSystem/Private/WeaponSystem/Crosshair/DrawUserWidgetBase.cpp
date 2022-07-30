//
//  DrawUserWidgetBase.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 30.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/Crosshair/DrawUserWidgetBase.h"

void UDrawUserWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
}

void UDrawUserWidgetBase::DrawLine(FPaintContext Context, FVector2D PositionA, FVector2D PositionB) const
{
    UWidgetBlueprintLibrary::DrawLine(Context, PositionA, PositionB, Tint, bAntiAlias, Thickness);
}
