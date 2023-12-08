// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Widgets/ShopWidget.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "UMG/Public/Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/Button.h"
#include "Camera/CameraComponent.h"
#include "ZombieGame/Characters/Grandad/Grandad.h"


void UShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UShopWidget::OnExitClicked);
    }

    for (int i = 0; i <= 7; i++)
    {
        FetchItem(i);
        ShopItemWidgetInstance = CreateWidget<UShopItem>(GetWorld(), WidgetClass);

        ItemGrid->AddChildToUniformGrid(ShopItemWidgetInstance, i/4, i%4);

        // Store the ShopItem instance in the array
        ShopItems.Add(ShopItemWidgetInstance);
    }
}

void UShopWidget::OnExitClicked()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    
    PlayerController->SetInputMode(FInputModeGameOnly());
    Character->GetCharacterMovement()->MaxWalkSpeed = 600;
    PlayerController->bShowMouseCursor = false;
    SetVisibility(ESlateVisibility::Hidden);
}

void UShopWidget::FetchItem(int Index)
{
    AGrandad* Grandad = AGrandad::GetInstance();

    if (Grandad->Inventory.IsValidIndex(Index))
    {
        ItemID = Grandad->Inventory[Index];
    }

    else
    {
        ItemID = -1;
    }
    
}
