#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.h"
#include "DoorButton.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class STARHUNT_API ADoorButton : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoorButton();

	// 버튼 메시
	UPROPERTY(VisibleAnywhere, Category="Button")
	UStaticMeshComponent* ButtonMesh;

	// 트리거 볼륨
	UPROPERTY(VisibleAnywhere, Category = "Button")
	UBoxComponent* TriggerVolume;

	// Door 참조
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Button")
	TArray<AActor*> DoorRefs;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected:
	virtual void BeginPlay() override;
};
