#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SpotLightComponent.h>
#include "RangedSpotLight.generated.h"

UCLASS()
class STARHUNT_API ARangedSpotLight : public AActor
{
	GENERATED_BODY()

public:

	ARangedSpotLight();

protected:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* SpotLightComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracking")
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightControl")
	bool bIsLightActive;

};
