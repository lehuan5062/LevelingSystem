// Copyright 2023 Lê Vương Gia Huân

// LevelingSystem
#include "LevelingSetting.h"

// set category in project settings
ULevelingSetting::ULevelingSetting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), StartingExperience{ 0 }, BaseNextLevel{ 1 }, MaxLevel{}
{
	CategoryName = "Plugins";
}