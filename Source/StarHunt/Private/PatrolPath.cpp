// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//루트 컴포넌트 생성 및 설정
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	//스플라인 컴포넌트 추가 및 부착 
	SplineComp=CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	SplineComp->SetupAttachment(RootComp);

	//변수 초기 설정
	PatrolIndex = 0;
	Direction=1;

}

void APatrolPath::IncrementPatrolIndex()
{
	PatrolIndex+=Direction;
	if (PatrolIndex >= SplineComp->GetNumberOfSplinePoints()-1)
	{
		Direction = -1;
	}
	else if (PatrolIndex <= 0)
	{
		Direction = 1;
	}
}

FVector APatrolPath::GetSplinePointsAsWorldPosition() const
{
	return SplineComp->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

// Called when the game starts or when spawned
// void APatrolPath::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }

// Called every frame
// void APatrolPath::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

