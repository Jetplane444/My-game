#pragma once

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


class Enemy
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

	explicit Enemy(const glm::vec2& pos, const Graphics::SpriteAnim& sprite);

	void update(float deltaTime);

	void draw(Graphics::Image& image, const Math::Camera2D& camera);

	void setPosition(const glm::vec2& pos);
	const glm::vec2& getPosition() const;

	void translate(const glm::vec2& t);

	const Math::AABB getAABB() const;


private:
	void setState(State newState);

	State state = State::None;
	Math::Transform2D transform;
	glm::vec2 velocity{ 0 };
	float speed{ 60.0f };
	Graphics::SpriteAnim sprite;
	Graphics::SpriteAnim IdleAnim;
	Math::AABB aabb;

};

