//
//  ScoreManagerComponent.h
//  UE4 WeaponSystem Plugin
//
//  Created by Kim David Hauser on 26.02.22.
//  Copyright © 1991 - 2022 DaVe Inc. kimhauser.ch, All rights reserved.
//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utils/GlobalDefinitions.h"
#include "ScoreManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONSYSTEM_API UScoreManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScoreManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Score System")
    float Score = 0.0f;
    
    UFUNCTION(BlueprintCallable, Category="Score System")
    void AddScore(float Value, float& NewScore);
    
    UFUNCTION(BlueprintCallable, Category="Score System")
    void ResetScore();
    
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
