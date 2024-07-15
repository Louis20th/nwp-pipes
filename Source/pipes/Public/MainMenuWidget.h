// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

namespace utils {
	using ClickEventCallback = std::function<void(void)>;
}

/**
*
*/
UCLASS()
class PIPES_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Sets onClicked callback for StartButton
	void setOnStartClickedCallback(utils::ClickEventCallback const startCallback);
	// Sets onClicked callback for QuitButton
	void setOnQuitClickedCallback(utils::ClickEventCallback const quitCallback);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();

private:
	utils::ClickEventCallback mStartCallback;
	utils::ClickEventCallback mQuitCallback;
};
