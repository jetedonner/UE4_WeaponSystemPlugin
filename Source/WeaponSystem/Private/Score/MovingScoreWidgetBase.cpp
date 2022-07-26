//
//  MovingScoreWidgetBase.cpp
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 31.03.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#include "Score/MovingScoreWidgetBase.h"

void UMovingScoreWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMovingScoreWidgetBase::PlayMoveAndFadeAnim()
{
    PlayAnimation(MoveUpAndFadeOutAnim, 0.0f, 1.0f, EUMGSequencePlayMode::Forward, 1.0f, false);
}
