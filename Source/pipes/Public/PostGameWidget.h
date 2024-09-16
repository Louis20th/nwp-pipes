// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chaser.h"
#include "Blueprint/UserWidget.h"
#include "PostGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIPES_API UPostGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void onEnd(ChaseStatus const status, uint32 const score);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MenuText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ScoreText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* LeftButtonText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* QuitButtonText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* LeftButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;

	UFUNCTION()
	void onQuit();
	UFUNCTION()
	void onRestart();
};
