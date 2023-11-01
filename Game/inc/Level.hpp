#pragma once

#include "Player.hpp"

enum class ColliderType
{
	Default,
	Wall,
	Plant,
	Prop,
};

struct Collider
{

};

class Level final
{
public:
	Level() = default;
	//Level(ldtk)

	void update(float deltaTime);

	// Reset level.
	void reset();

	//choose character
	void setCharacter(size_t characterId);

	const Player& getPlayer() const noexcept
	{
		//return player;
	}

	void draw(Graphics::Image& image) const;

private:


};