// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "PatrolPath.generated.h"

UCLASS()
class STARHUNT_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	
	//Patrol 경로 관련 변수 선언
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Path")
	int32 PatrolIndex;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Path")
	int32 Direction;
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// 스플라인 컴포넌트
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Path")
	USplineComponent* SplineComp;

	//루트 컴포넌트
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	USceneComponent* RootComp;

	UFUNCTION(BlueprintCallable, Category="Path")
	void IncrementPatrolIndex();
	UFUNCTION(BlueprintCallable, Category="Path")
	FVector GetSplinePointsAsWorldPosition() const;
};
