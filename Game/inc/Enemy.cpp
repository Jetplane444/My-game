#include "Enemy.hpp"

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Math/Camera2D.hpp>

using namespace Graphics;
using namespace Math;

Enemy::Enemy() = default;

Enemy::Enemy(const glm::vec2& pos, const Graphics::SpriteAnim& _sprite)
	: transform{pos }
	, aabb{ {8, 16, 0}, {24, 38, 0} }
{
	auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer?Idle.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
		IdleAnim = SpriteAnim{ idle_sprites, 6 };
}

void Enemy::update(float deltaTime)
{
	auto initialPos = transform.getPosition(); 
	auto newPos = initialPos;

	newPos.x += Input::getAxis("Horisontal") * speed * deltaTime;
	newPos.y -= Input::getAxis("Vertical") * speed * deltaTime;

	velocity = (newPos - initialPos) / deltaTime;

	transform.setPosition(newPos);

	if (glm::length(velocity) > 0)
	{
		setState(State::Running);
	}
	else
	{
		setState(State::Idle);
	}

	sprite.update(deltaTime);
}


void Enemy::draw(Graphics::Image& image, const Math::Camera2D& camera)
{
	switch(state)
	{
		case State::Idle:
			image.drawSprite(sprite, camera * transform);
			break;
		case State::Running:
			// TODO: draw running animation.
			break;
	}
#if _DEBUG
	image.drawAABB(getAABB(), Color::Yellow, {}, FillMode::WireFrame);
	auto pos = camera * transform;
	image.drawText(Font::Default, "State...", pos[2][0], pos[1][0], Color::Black);
#endif
}

void Enemy::setPosition(const glm::vec2& pos)
{
	transform.setPosition(pos);
}

const glm::vec2& Enemy::getPosition() const
{
	return transform.getPosition();
}

void Enemy::translate(const glm::vec2& t)
{
	transform.translate(t);
}

const Math::AABB Enemy::getAABB() const
{
	return transform * aabb;
}

void Enemy::setState(State newState)
{
	 if (newState != state)
	 {
		 switch (newState)
		 {
		 case State::Idle:
			 break;
		 case State::Running:
			 break;
		 }
		 state = newState;
	 }
}


