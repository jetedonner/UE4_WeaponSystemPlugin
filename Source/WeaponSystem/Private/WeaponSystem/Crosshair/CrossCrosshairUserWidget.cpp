//
//  CrossCrosshairUserWidget.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 25.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/Crosshair/CrossCrosshairUserWidget.h"

void UCrossCrosshairUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    FVector2D MySize = FVector2D(InitSize, InitSize);
    SetDesiredSizeInViewport(MySize);
}

int32 UCrossCrosshairUserWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
    
    FVector2D CenterPosition = FVector2D((InitSize / 2), (InitSize / 2));
    
    DrawLine(Context, FVector2D(CenterPosition.X + InnerDistance, CenterPosition.Y), FVector2D(CenterPosition.X + InitSize / 2, CenterPosition.Y));
    DrawLine(Context, FVector2D(CenterPosition.X - InnerDistance, CenterPosition.Y), FVector2D(CenterPosition.X - InitSize / 2, CenterPosition.Y));
    DrawLine(Context, FVector2D(CenterPosition.X, CenterPosition.Y + InnerDistance), FVector2D(CenterPosition.X, CenterPosition.Y + InitSize / 2));
    DrawLine(Context, FVector2D(CenterPosition.X, CenterPosition.Y - InnerDistance), FVector2D(CenterPosition.X, CenterPosition.Y - InitSize / 2));
    
    DrawLine(Context, FVector2D(CenterPosition.X + 1, CenterPosition.Y + 1), FVector2D(CenterPosition.X - 1, CenterPosition.Y - 1));
    
    DrawLine(Context, FVector2D(CenterPosition.X - 1, CenterPosition.Y + 1), FVector2D(CenterPosition.X + 1, CenterPosition.Y - 1));

//    template<typename PointType>
//    UWidgetBlueprintLibrary::DrawPoint
//    (
//        const PointType & Position,
//     Tint,
//        3.0f
//    )
    
    return LayerId;
}

//void UCrossCrosshairUserWidget::DrawLine(FPaintContext Context, FVector2D PositionA, FVector2D PositionB) const
//{
//    UWidgetBlueprintLibrary::DrawLine(Context, PositionA, PositionB, Tint, bAntiAlias, Thickness);
//}
