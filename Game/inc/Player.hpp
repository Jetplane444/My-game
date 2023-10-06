#pragma once

#include "Entity.hpp"

#include <Graphics/SpriteAnim.hpp>

namespace Math
{
	class Camera2D;
}
namespace Graphics
{
	class Image;
}

class Player : public Entity
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
	Player();

	explicit Player(const glm::vec2& pos);

	void doMovement(float deltaTime);
	void doIdle(float deltaTime);
	void doRunning(float deltaTime);
	void doAttack(float deltaTime);

	virtual void update(float deltaTime) override;
	virtual void draw(Graphics::Image& image, const Math::Camera2D& camera) override;
	
	
private:
	void setState(State newState);

	glm::vec2 velocity{ 0 };
	float speed{ 60.0f };

	State state = State::None;
	Graphics::SpriteAnim IdleAnim;
	Graphics::SpriteAnim RunAnim;
	Graphics::SpriteAnim AttackAnim;
};
