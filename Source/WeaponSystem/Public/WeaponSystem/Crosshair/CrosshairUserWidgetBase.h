//
//  CrosshairUserWidgetBase.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 05.07.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairUserWidgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrosshairAnimationDelegate, bool, AlternativeAnimation);

UCLASS()
class WEAPONSYSTEM_API UCrosshairUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
    
protected:
    const FName AimedAnimationName = FName("AimedAnimation");
    
    virtual void NativeConstruct() override;
    
public:
    
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
//    FLinearColor Tint = FLinearColor::Red;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
//    bool bAntiAlias = true;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cross Crosshair", Interp, meta = (BindWidget))
//    float Thickness = 3.0f;
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon System")
    void OnAnimateCrosshair(bool AlternativeAnimation = false);
    
    UFUNCTION(BlueprintCallable, Category = "Weapon System")
    void PlayAimedAtAnimation(bool AlternativeAnimation = false);
    
    void OnAnimateCrosshair_Implementation(bool AlternativeAnimation = false);
    
    UPROPERTY(BlueprintAssignable, Category="Weapon System")
    FCrosshairAnimationDelegate OnCrosshairAnimationDelegate;
    
    UWidgetAnimation* GetAnimationByName(FName AnimationName) const;

    bool PlayAnimationByName(FName AnimationName,
        float StartAtTime,
        int32 NumLoopsToPlay,
        EUMGSequencePlayMode::Type PlayMode,
        float PlaybackSpeed);

protected:
    TMap<FName, UWidgetAnimation*> AnimationsMap;

    void FillAnimationsMap();
    
};
