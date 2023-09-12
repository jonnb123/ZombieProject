// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Widgets/ShopWidget.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "UMG/Public/Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"

void UShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);

    for (int i = 0; i <= 7; i++)
    {
        ShopItemWidgetInstance = CreateWidget<UShopItem>(GetWorld(), WidgetClass);

        ItemGrid->AddChildToUniformGrid(ShopItemWidgetInstance, i/4, i%4);
    }
}
