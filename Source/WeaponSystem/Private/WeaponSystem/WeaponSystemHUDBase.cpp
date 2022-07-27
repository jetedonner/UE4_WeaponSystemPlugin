//
//  WeaponSystemHUDBase.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//


#include "WeaponSystem/WeaponSystemHUDBase.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AWeaponSystemHUDBase::AWeaponSystemHUDBase()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> TestBP(TEXT("WidgetBlueprint'/WeaponSystem/HUD/WeaponSystemHUDWidgetBase'"));
    TSubclassOf<UUserWidget> MyWidgetClass = TestBP.Class;
    
    if (MyWidgetClass == nullptr)
    {
//        UE_LOG(LogTemp, Warning, TEXT("MyWidgetClass NOT LOADED ..."));
    }
    else
    {
//        UE_LOG(LogTemp, Warning, TEXT("MyWidgetClass IS LOADED ..."));
        AllUIWidgets.Add(MyWidgetClass);
//        UE_LOG(LogTemp, Warning, TEXT("MyWidgetClass ADDED ..."));
    }
}

void AWeaponSystemHUDBase::BeginPlay()
{
    Super::BeginPlay();

    // If any widgets need to be added
    if (AllUIWidgets.Num() > 0)
    {
        // Iterate through all widgets
        for (TSubclassOf<UUserWidget> Widget : AllUIWidgets)
        {
            // Create an instance of the widget and add to viewport
            UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);
            CreatedWidget->AddToViewport();

            // Store instanced widget in array
            CreatedWidgets.Add(CreatedWidget);
        }
    }
}

void AWeaponSystemHUDBase::DrawHUD()
{
    Super::DrawHUD();
}
