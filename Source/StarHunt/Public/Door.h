#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class STARHUNT_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:
	ADoor();

protected:
	virtual void BeginPlay() override;

	// 문 메시에 해당하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	UStaticMeshComponent* DoorMesh;

public:	

    // 문을 아예 숨기고 충돌 비활성화
    UFUNCTION(BlueprintCallable, Category = "Door")
    void HideDoor();

    // 다시 보이게?
    UFUNCTION(BlueprintCallable, Category = "Door")
    void ShowDoor();
};
