#pragma once

#include "Player.hpp"

#include "Box.hpp"
#include "Effect.hpp"
#include "Pickup.hpp"
#include "Player.hpp"

#include <Audio/Sound.hpp>
#include <Math/AABB.hpp>

#include <Graphics/Image.hpp>
#include <Graphics/TileMap.hpp>

#include <LDTKLoader/Level.hpp>
#include <LDTKLoader/World.hpp>

#include <Random>

enum class ColliderType
{
	Default,
	Wall,
	Plant,
	Prop,
};

struct Collider
{
	ColliderType type = ColliderType::Default;
	Math::AABB  aabb;
	bool        isOneWay = false;
};

class Level final
{
public:
	Level() = default;
	Level(const ldtk::Project& project, const ldtk::World& world, const ldtk::Level& level);

	void update(float deltaTime);

	// Reset level.
	void reset();

	const Player& getPlayer() const noexcept
	{
		return player;
	}

	void draw(Graphics::Image& image, const Math::Camera2D& camera) const;

private:
	void updateCollisions(float deltaTime);
	void updateEffects(float deltaTime);
	void updateBoxes(float deltaTime);

	const ldtk::World* world = nullptr;
	const ldtk::Level* level = nullptr;

	std::string levelName;

	// Level colliders.
	std::vector<Collider> colliders;

	// Currently playing effects.
	std::vector<Effect> effects;

	// Boxes
	std::vector<std::shared_ptr<Box>> boxes;

	// Level tile map.
	Graphics::TileMap tileMap;

	Graphics::TileMap treeMap;

	Player    player;
	glm::vec2 playerStart{ 0 };

	Audio::Sound                    bgMusic;
	Audio::Sound                    pickupSound;
	std::vector<Audio::Sound>       woodBreakSounds;
	std::minstd_rand                rng;
	std::uniform_int_distribution<> dist;
};