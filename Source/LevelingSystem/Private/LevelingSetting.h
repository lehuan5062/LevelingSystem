// Copyright 2023 Lê Vương Gia Huân

#pragma once

// DeveloperSettings
#include "Engine/DeveloperSettings.h"

//
#include "LevelingSetting.generated.h"

UCLASS(config = LevelingSystem, DefaultConfig)
class LEVELINGSYSTEM_API ULevelingSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// set category in project settings
	ULevelingSetting(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(category = LevelingSystem, config, EditAnywhere, meta = (ConfigRestartRequired = true)) int StartingExperience;

	// base experience point to reach next level
	// actual experience point to reach next level rating will be doubled each time player reach next level rating
	UPROPERTY(category = LevelingSystem, config, EditAnywhere) int BaseNextLevel;

	// 0 means no limit
	UPROPERTY(category = LevelingSystem, config, EditAnywhere) int MaxLevel;
};