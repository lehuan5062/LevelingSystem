// Copyright 2023 Lê Vương Gia Huân

#pragma once

// engine
#include "Components/ActorComponent.h"

// CurrencySystem
#include "CurrencyComponent.h"

//
#include "LevelingComponent.generated.h"

UCLASS()
class LEVELINGSYSTEM_API ULevelingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULevelingComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY() const TObjectPtr<UCurrencyComponent> LevelComponent;

	UPROPERTY() const TObjectPtr<UCurrencyComponent> ExperienceComponent;

	// calculate level from experience
	// bLoop: use loop over log2 method
	// loop: slower, multi-threaded, more accurate
	// log2: faster, THREAD SAFE, might be inaccurate
	// we'll use the log2 method until actual issue occurs
	// multithreaded
	void CalculateLevel(const bool& bLoop) noexcept;

	// add experience and calculate level
	// bLoop: use loop over log2 method
	// loop: slower, multi-threaded, more accurate
	// log2: faster, THREAD SAFE, might be inaccurate
	// we'll use the log2 method until actual issue occurs
	// multithreaded
	void AddExperience(const unsigned int& experience, const bool& bLoop) noexcept;

	// subtract experience and calculate level
	// THREAD_SAFE
	void SubtractExperience(const unsigned int& experience) noexcept;

	// calculate base experience from level
	// THREAD SAFE
	int CalculateExperience(const unsigned int& level) const noexcept;

private:
	// get stored leveling system setting(s)
	const TWeakObjectPtr<const class ULevelingSetting> setting;
};