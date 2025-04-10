/******************************************************************************
 * Copyright (c) 2018-2024 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

#include "VillagerArchetype.h"

#include <glm/gtx/euler_angles.hpp>
#include <glm/vec3.hpp>

#include "Common/RandomNumberManager.h"
#include "ECS/Components/LivingAction.h"
#include "ECS/Components/Mesh.h"
#include "ECS/Components/Mobile.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Villager.h"
#include "ECS/Components/WallHug.h"
#include "ECS/Registry.h"
#include "ECS/Systems/TownSystemInterface.h"
#include "InfoConstants.h"
#include "Locator.h"
#include "Resources/ResourceManager.h"

using namespace openblack;
using namespace openblack::ecs::archetypes;
using namespace openblack::ecs::components;
using namespace openblack::ecs::systems;

entt::entity VillagerArchetype::Create([[maybe_unused]] const glm::vec3& abodePosition, const glm::vec3& position,
                                       VillagerInfo type, uint32_t age)
{
	auto& registry = Locator::entitiesRegistry::value();
	const auto entity = registry.Create();

	const auto& info = Locator::infoConstants::value().villager.at(static_cast<size_t>(type));

	registry.Assign<Transform>(entity, position, glm::eulerAngleY(glm::radians(180.0f)), glm::vec3(1.0));
	registry.Assign<Mobile>(entity);
	const uint32_t health = 100;
	const uint32_t hunger = 100;

	const auto lifeStage = age < 18 ? Villager::LifeStage::Child : Villager::LifeStage::Adult;
	const auto sex = info.villagerNumber == VillagerNumber::Housewife ? Villager::Sex::FEMALE : Villager::Sex::MALE;
	const auto task = Villager::Task::IDLE;

	// TODO(bwrsandman): Might be better to make a FindClosestAbode
	const entt::entity town = Locator::townSystem::value().FindClosestTown(abodePosition);
	entt::entity abode = entt::null;
	if (town != entt::null)
	{
		abode = Locator::townSystem::value().FindAbodeWithSpace(town);
	}

	registry.Assign<Villager>(entity, health, static_cast<uint32_t>(age), hunger, lifeStage, sex, info.tribeType,
	                          info.villagerNumber, task, town, abode);
	registry.Assign<WallHug>(entity, glm::vec2(), glm::vec2(), GetSpeedStateSpeed(info.speedGroup.speedDefault));
	const auto resourceId = resources::HashIdentifier(info.highDetail);
	registry.Assign<Mesh>(entity, resourceId, static_cast<int8_t>(0), static_cast<int8_t>(0));
	auto turnsSinceStateChange = Locator::rng::value().NextValue<uint16_t>(1, 500);
	registry.Assign<LivingAction>(entity, VillagerStates::Created, turnsSinceStateChange);

	return entity;
}
