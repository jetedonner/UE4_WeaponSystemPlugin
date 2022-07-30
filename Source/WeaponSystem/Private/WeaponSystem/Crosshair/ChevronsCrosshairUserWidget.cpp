//
//  ChevronsCrosshairUserWidget.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 25.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "WeaponSystem/Crosshair/ChevronsCrosshairUserWidget.h"

void UChevronsCrosshairUserWidget::NativeConstruct()
{
//    FillAnimationsMap();
    
    Super::NativeConstruct();
    
    FVector2D MySize = FVector2D(InitSize, InitSize);
    SetDesiredSizeInViewport(MySize);
}

int32 UChevronsCrosshairUserWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
    
    FVector2D CenterPosition = FVector2D((InitSize / 2), (InitSize / 2));
    
    // Right to Down
    DrawLine(Context, FVector2D(CenterPosition.X + InnerDistance, CenterPosition.Y), FVector2D(CenterPosition.X + InnerDistance + ChevronLength, CenterPosition.Y + ChevronLength));
    // Right to Up
    DrawLine(Context, FVector2D(CenterPosition.X + InnerDistance, CenterPosition.Y), FVector2D(CenterPosition.X + InnerDistance + ChevronLength, CenterPosition.Y - ChevronLength));
    
    // Left to Down
    DrawLine(Context, FVector2D(CenterPosition.X - InnerDistance, CenterPosition.Y), FVector2D(CenterPosition.X - InnerDistance - ChevronLength, CenterPosition.Y + ChevronLength));
    // Left to Up
    DrawLine(Context, FVector2D(CenterPosition.X - InnerDistance, CenterPosition.Y), FVector2D(CenterPosition.X - InnerDistance - ChevronLength, CenterPosition.Y - ChevronLength));
    
    // Up to Right
    DrawLine(Context, FVector2D(CenterPosition.X, CenterPosition.Y - InnerDistance), FVector2D(CenterPosition.X + ChevronLength, CenterPosition.Y - InnerDistance - ChevronLength));
    // Up to Left
    DrawLine(Context, FVector2D(CenterPosition.X, CenterPosition.Y - InnerDistance), FVector2D(CenterPosition.X - ChevronLength, CenterPosition.Y - InnerDistance - ChevronLength));
    
    // Down to Right
    DrawLine(Context, FVector2D(CenterPosition.X, CenterPosition.Y + InnerDistance), FVector2D(CenterPosition.X + ChevronLength, CenterPosition.Y + InnerDistance + ChevronLength));
    // Down to Left
    DrawLine(Context, FVector2D(CenterPosition.X, CenterPosition.Y + InnerDistance), FVector2D(CenterPosition.X - ChevronLength, CenterPosition.Y + InnerDistance + ChevronLength));
        
    // Draw X in the middle
    DrawLine(Context, FVector2D(CenterPosition.X + 1, CenterPosition.Y + 1), FVector2D(CenterPosition.X - 1, CenterPosition.Y - 1));
    DrawLine(Context, FVector2D(CenterPosition.X - 1, CenterPosition.Y + 1), FVector2D(CenterPosition.X + 1, CenterPosition.Y - 1));
   
    return LayerId;
}
