// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountdownTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIPES_API UCountdownTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void startCountdown(int const time);
	void setParent(AGameModeBase*);
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CountDownText;

private:
	void TimerTick();

	FTimerHandle mCountdownTimerHandle;
	int mRemainingTime;
};
