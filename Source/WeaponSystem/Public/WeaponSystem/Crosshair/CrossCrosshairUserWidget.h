//
//  CrosshairUserWidgetBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 05.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "SceneManagement.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Math/Color.h"
#include "Math/Vector2D.h"
#include "Widgets/Layout/Anchors.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSystem/Crosshair/CrosshairUserWidgetBase.h"
#include "CrossCrosshairUserWidget.generated.h"

/**
 * 
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrosshairAnimationDelegate, bool, AlternativeAnimation);

UCLASS()
class WEAPONSYSTEM_API UCrossCrosshairUserWidget : public UCrosshairUserWidgetBase
{
	GENERATED_BODY()
    
protected:

    virtual void NativeConstruct() override;
    
    virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;
    
public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
    float InitSize = 100.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
    FLinearColor Tint = FLinearColor::Red;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
    bool bAntiAlias = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
    float Thickness = 3.0f;
    
//    UWidgetAnimation* GetAnimationByName(FName AnimationName) const;
//
//    bool PlayAnimationByName(FName AnimationName,
//        float StartAtTime,
//        int32 NumLoopsToPlay,
//        EUMGSequencePlayMode::Type PlayMode,
//        float PlaybackSpeed);
//
//protected:
//    TMap<FName, UWidgetAnimation*> AnimationsMap;
//
//    void FillAnimationsMap();
    
};
