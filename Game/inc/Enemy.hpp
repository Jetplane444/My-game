#pragma once

#include "Entity.hpp"

#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>
#include <glm/vec2.hpp>

namespace Math
{
	class Camera2D;
}

namespace Graphics
{
	class Image;
}


class Enemy : public Entity
{
public:
	enum class State
	{
		None,
		Idle,
		Running,
		Attack,
		Dead
	};

	// Default constructor.
	Enemy();

	explicit Enemy(const glm::vec2& pos);

	void doMovement(float deltaTime);
	void doIdle(float deltaTime);
	void doRunning(float deltaTime);
	void doAttack(float deltaTime);
	void doDie(float deltaTime);

	void update(float deltaTime) override;
	void draw(Graphics::Image& image, const Math::Camera2D& camera) override;
	void setTarget(Entity* target);

private:
	void setState(State newState);

	Entity* target = nullptr;

	State state = State::None;
	
	glm::vec2 velocity{ 0 };
	float speed{ 60.0f };

	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim runAnim;
	Graphics::SpriteAnim attackAnim;
	Graphics::SpriteAnim deathAnim;
};

