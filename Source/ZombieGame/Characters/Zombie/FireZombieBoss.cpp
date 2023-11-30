// Fill out your copyright notice in the Description page of Project Settings.


#include "FireZombieBoss.h"

AFireZombieBoss::AFireZombieBoss()
{
	// don't allow dismemberment for bosses
	MaxHealth = 200;
	HeadHealth = 200;
	LegHealth = 200;
	RightArmHealth = 200;
	LeftArmHealth = 200;

	Health = MaxHealth;
}
