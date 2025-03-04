// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGun.h"
#include "ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class STARHUNT_API AShotGun : public ABaseGun
{
	GENERATED_BODY()

public:
    AShotGun();

protected:

    virtual void FireProgress() override;

    // 한 발당 발사할 펠렛 수
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ShotGun")
    int32 NumberOfPellets;
};
