// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMouseController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameBoardActor.h"

void AInGameMouseController::BeginPlay()
{
    Super::BeginPlay();

    bEnableClickEvents = true;
    bShowMouseCursor = true;

}

void AInGameMouseController::OnLeftMouseClick(const FInputActionValue& Value)
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

    if (HitResult.GetActor())
    {
        // Try to cast the hit actor to your custom tile map actor
        AGameBoardActor* TileMapActor = Cast<AGameBoardActor>(HitResult.GetActor());
        if (TileMapActor)
        {
            UE_LOG(LogTemp, Error, TEXT("---------> OnLeftMouseClick()"));
            // Handle the click on the tile map actor
            //TileMapActor->HandleTileMapClick(HitResult.Location);
        }
    }

    /*UE_LOG(LogTemp, Error, TEXT("---------> OnLeftMouseClick()"));
    FHitResult HitResult;
    GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, HitResult);

    if (HitResult.GetComponent())
    {
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Mouse Click+++ Component: %s"), *HitResult.GetComponent()->GetName()));
    }

    if (HitResult.GetActor())
    {
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Mouse Click+++ Actor: %s"), *HitResult.GetActor()->GetName()));
    }*/
}
