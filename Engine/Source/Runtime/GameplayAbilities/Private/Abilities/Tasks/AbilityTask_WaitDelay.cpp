
#include "AbilitySystemPrivatePCH.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UAbilityTask_WaitDelay::UAbilityTask_WaitDelay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Time = 0.f;
	TimeStarted = 0.f;
}

UAbilityTask_WaitDelay* UAbilityTask_WaitDelay::WaitDelay(UObject* WorldContextObject, float Time)
{
	auto MyObj = NewTask<UAbilityTask_WaitDelay>(WorldContextObject);
	MyObj->Time = Time;
	return MyObj;
}

void UAbilityTask_WaitDelay::Activate()
{
	UWorld* World = GetWorld();
	TimeStarted = World->GetTimeSeconds();
	World->GetTimerManager().SetTimer(this, &UAbilityTask_WaitDelay::OnTimeFinish, Time, false);
}

void UAbilityTask_WaitDelay::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_WaitDelay::OnTimeFinish()
{
	OnFinish.Broadcast();
}

FString UAbilityTask_WaitDelay::GetDebugString() const
{
	float TimeLeft = GetWorld()->GetTimeSeconds() - TimeStarted;
	return FString::Printf(TEXT("WaitDelay. Time: %.2f. TimeLeft: %.2f"), Time, TimeLeft);
}