/******************************************************************************
 * Copyright (c) 2018-2024 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *******************************************************************************/

#pragma once

#include <cstdint>

#include <chrono>
#include <memory>
#include <optional>

#include <glm/vec3.hpp>

namespace openblack
{

class Camera;

class CameraModel
{
public:
	enum class Model : uint8_t
	{
		Old,
	};

	struct CameraInterpolationUpdateInfo
	{
		glm::vec3 origin;
		glm::vec3 focus;
		std::chrono::duration<float> duration;
	};

	static std::unique_ptr<CameraModel> CreateModel(Model model);

	virtual ~CameraModel();

	virtual std::optional<CameraInterpolationUpdateInfo> Update(std::chrono::microseconds dt, const Camera& camera) = 0;
	virtual void HandleActions(std::chrono::microseconds dt) = 0;
	[[nodiscard]] virtual glm::vec3 GetTargetOrigin() const = 0;
	[[nodiscard]] virtual glm::vec3 GetTargetFocus() const = 0;
	[[nodiscard]] virtual std::chrono::seconds GetIdleTime() const = 0;
};

} // namespace openblack