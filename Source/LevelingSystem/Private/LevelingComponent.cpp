// Copyright 2023 Lê Vương Gia Huân

// LevelingSystem
#include "LevelingComponent.h"
#include "LevelingSetting.h"

// default value
ULevelingComponent::ULevelingComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, LevelComponent{ CreateDefaultSubobject<UCurrencyComponent>("LevelRatingComponent") }
	, ExperienceComponent{ CreateDefaultSubobject<UCurrencyComponent>("ExperiencePointComponent") }
	, setting{ GetMutableDefault<ULevelingSetting>() }
{
	// get experience from stored setting
	AddExperience(setting->StartingExperience, true);
}

// calculate level from experience
// bLoop: use loop over log2 method
// loop: slower, multi-threaded, more accurate
// log2: faster, THREAD SAFE, might be inaccurate
// we'll use the log2 method until actual issue occur
// multithreaded
void ULevelingComponent::CalculateLevel(const bool& bLoop = false) noexcept
{
	if (setting->MaxLevel == 0 || LevelComponent->get() < setting->MaxLevel)
	{
		const auto&& experience{ ExperienceComponent->get() };

		// for x = experience, n = level, x2 = BaseNextLevel
		// x[n] = 2x[n-1] + x[2] = x[n-1] + 2^(n-2) * x[2]
		if (experience >= setting->BaseNextLevel)
		{
			if (bLoop)
			{
				// (x[n] - x[2]) / (2 * x[2]) + 1 = 2^(n-2)
				// try to count (n-2) using loop
				std::atomic_int&& LookUpValue{ (experience - setting->BaseNextLevel) / (2 * setting->BaseNextLevel) + 1 };
				std::atomic_int&& level{ 2 };
				auto&& LevelUp{ UE::Tasks::Launch(UE_SOURCE_LOCATION, [&level] { return level.load(); }) };
				for (
					// calculate condition (x[n] - x[2]) / (2 * x[2]) + 1 > 1
					auto&& condition{ UE::Tasks::Launch(UE_SOURCE_LOCATION, [&LookUpValue] { return LookUpValue > 1; }) };

					// increase level count condition
					condition.GetResult();

					// increase level count
					LevelUp.Launch(UE_SOURCE_LOCATION, [&level] { return ++level; })
					)
				{
					condition.Launch(UE_SOURCE_LOCATION, [&LookUpValue] { LookUpValue = LookUpValue / 2; return LookUpValue > 1; });
					LevelUp.BusyWait();
				}

				LevelComponent->set(LevelUp.GetResult());
			}
			// n = log[2]((x[n] - x[2]) / (2 * x[2])) + 1) + 2
			// shouldn't use LookUp (above) for this, result's precision is lower
			else LevelComponent->set(FMath::Log2(((experience - setting->BaseNextLevel) / (2. * setting->BaseNextLevel)) + 1.) + 2);
		}
		else LevelComponent->set(1);
	}
}

// add experience and calculate level
// bLoop: use loop over log2 method
// loop: slower, multi-threaded, more accurate
// log2: faster, THREAD SAFE, might be inaccurate
// we'll use the log2 method until actual issue occur
// multithreaded
void ULevelingComponent::AddExperience(const unsigned int& experience, const bool& bLoop = false) noexcept
{
	ExperienceComponent->add(experience); CalculateLevel(bLoop);
}

// subtract experience and calculate level
// THREAD_SAFE
void ULevelingComponent::SubtractExperience(const unsigned int& experience) noexcept
{
	checkSlow(ExperienceComponent->ShouldTake(experience))
		ExperienceComponent->take(experience); CalculateLevel();
}

// calculate base experience from level > 0
// THREAD SAFE
int ULevelingComponent::CalculateExperience(const unsigned int& level) const noexcept
{
	// for x = experience, n = level >= 2, x2 = BaseNextLevel
	// x[n] = 2x[n-1] + x[2] = x[n-1] + 2^(n-2) * x[2]
	// x[n] = (2^(n-1) - 1) * x[2]
	return FMath::RoundHalfFromZero(FGenericPlatformMath::Pow(2., level - 1.) - 1) * setting->BaseNextLevel;
}