#pragma once

#include "Character.hpp"

#include <Math/AABB.hpp>

class Box : public Character
{
public:
	enum class State
	{
		Idle,
		Hit,
		Break
	};

	Box() = default;
	Box(int hitPoints);
	Box(const Box& copy);
	Box(Box&& other) noexcept;
	~Box() override = default;

	Box& operator=(const Box& copy);
	Box& operator=(Box&& other) noexcept;

	void setPosition(const glm::vec2& position);

	const Math::AABB& getAABB() const noexcept
	{
		return aabb;
	}

	/// <summary>
	/// The player hit the box. Reduce the hit points by 1 and return the remaining hitpoints.
	/// </summary>
	/// <param name="deltaTime">The frame time (in seconds).</param>
	void hit()
	{
		setState(State::Hit);
	}

	int getHitPoints() const noexcept
	{
		return hitPoints;
	}


	void update(float deltaTime) override;

	void draw(Graphics::Image& image) const;

	void setState(State newState);

	State getState() const noexcept
	{
		return state;
	}

private:
	void startState(State newState);
	void endState(State oldState);

	void updateIdle(float deltaTime);
	void updateHit(float deltaTime);
	void updateBreak(float deltaTime);

	Math::AABB        aabb;
	Math::Transform2D transform;

	State state = State::Idle;

	// How many hits it takes to destroy the box
	int hitPoints = 0;
};