// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIPES_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

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
};
