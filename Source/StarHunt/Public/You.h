#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "You.generated.h"

UCLASS()
class STARHUNT_API AYou : public ACharacter
{
    GENERATED_BODY()

public:
    // 기본 생성자
    AYou();

protected:
    // 게임 시작 시 호출
    virtual void BeginPlay() override;

public:
    // 매 틱마다 실행
    virtual void Tick(float DeltaTime) override;

    /** 적의 체력 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
    float Health;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
    float MaxHealth;

    /** 적이 데미지를 받았을 때 */
    void TakeDamage(float DamageAmount);

    /** 적이 사망했을 때 */
    void Die();

};
