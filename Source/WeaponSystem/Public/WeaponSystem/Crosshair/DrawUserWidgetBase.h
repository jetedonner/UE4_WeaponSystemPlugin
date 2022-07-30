//
//  DrawUserWidgetBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 30.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DrawUserWidgetBase.generated.h"

UCLASS()
class WEAPONSYSTEM_API UDrawUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
    
protected:

    virtual void NativeConstruct() override;
    
public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
    FLinearColor Tint = FLinearColor::Red;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
    bool bAntiAlias = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
    float Thickness = 3.0f;
    
protected:
    
    void DrawLine(FPaintContext Context, FVector2D PositionA, FVector2D PositionB) const;
};
