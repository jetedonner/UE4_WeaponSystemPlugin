//
//  CrossCrosshairUserWidgetBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 05.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "WeaponSystem/Crosshair/DrawUserWidgetBase.h"
#include "CrossCrosshairUserWidget.generated.h"

UCLASS()
class WEAPONSYSTEM_API UCrossCrosshairUserWidget : public UDrawUserWidgetBase
{
	GENERATED_BODY()
    
protected:

    virtual void NativeConstruct() override;
    
    virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;
    
public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
    float InitSize = 100.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
    float InnerDistance = 10.0f;
    
};
