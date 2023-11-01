#include <Player.hpp>
#include <Enemy.hpp>

#include <Graphics/Window.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/SpriteSheet.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/Color.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Vertex.hpp>
#include <Graphics/Keyboard.hpp>
#include <Graphics/KeyboardState.hpp>
#include <Graphics/Input.hpp>
#include <Math/Transform2D.hpp>
#include <Graphics/TileMap.hpp>
#include <Math/Camera2D.hpp>
#include <Audio/Device.hpp>
#include <Audio/Sound.hpp>

#include <fmt/core.h>
#include <glm/vec2.hpp>

#include <iostream>

using namespace Graphics;
using namespace Audio;
using namespace Math;

Window window;
Image image; 
TileMap grassTiles;
Sprite background;
Camera2D camera;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Player player;
Enemy enemy;

void InitGame()
{
	player.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	camera.setSize(player.getPosition());
	camera.setPosition(player.getPosition());
}

int main()
{
	// Input to go to reload the current map.
	Input::mapButton("Reload", [](std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState) {
		bool b = false;

		for (auto& gamePadState : gamePadStates)
		{
			b = b || gamePadState.b == ButtonState::Pressed;
		}

		const bool r = keyboardState.isKeyPressed(KeyCode::R);
		const bool enter = keyboardState.isKeyPressed(KeyCode::Enter);

		return b || enter || r;
		});
	

	image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	window.create(L"Mist", SCREEN_WIDTH, SCREEN_HEIGHT);
	window.show();
	window.setFullscreen(true);
	
	
	player = Player{ { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 } };
	enemy = Enemy{ {SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2} };

	// Load tilemap.
	auto backgroundMap = ResourceManager::loadImage("assets/Map.png");
	background = Sprite{ backgroundMap };

	auto grass_sprites = ResourceManager::loadSpriteSheet("assets/PixelArt/TX Tileset Grass.png", 16, 16);
	grassTiles = TileMap{ grass_sprites, 30, 30 };

	Sound music;

	music.loadMusic("audio/Theme.wav");
	music.setVolume(0.25f);
	
	for(int i = 0; i < 30; ++i)
    {
		for (int j = 0; j < 30; ++j)
		{
			grassTiles(i, j) = (i *grass_sprites->getNumColumns()+ j) % grass_sprites->getNumSprites();
		}
	}

	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";

	InitGame();

	while (window)
	{
		// update the input state
		Input::update();

		player.update(static_cast<float>(timer.elapsedSeconds()));
		enemy.update(static_cast<float>(timer.elapsedSeconds()));

		// Check collisions
		static bool isColliding = false;
		if (player.collides(enemy))
		{
			if (!isColliding)
			{
				std::cout << "Ouch!" << std::endl;
				isColliding = true;
			}
		}
		else
		{
			isColliding = false;
		}

		camera.setPosition(player.getPosition());

		// Make sure that the camera's visible area does not leave the area of the background sprite.
		glm::vec2 cameraCorrection{ 0 };
		if (camera.getLeftEdge() < 0)
		{
			cameraCorrection.x = -camera.getLeftEdge();
		}
		else if (camera.getRightEdge() > static_cast<float>(background.getWidth()))
		{
			cameraCorrection.x = std::floor(static_cast<float>(background.getWidth()) - camera.getRightEdge());
		}

		if (camera.getTopEdge() < 0)
		{
			cameraCorrection.y = -camera.getTopEdge();
		}
		else if (camera.getBottomEdge() > static_cast<float>(background.getHeight()))
		{
			cameraCorrection.y = std::floor(static_cast<float>(background.getHeight()) - camera.getBottomEdge());
		}

		//Apply camera correction
		camera.translate(cameraCorrection);

		// Check collisions with player.
		//Screen space collision.
		//{
		//	auto aabb = player.getAABB();
		//	glm::vec2 correction{ 0 };
		//	if (aabb.min.x < 0)
		//	{
		//		correction.x = -aabb.min.x;
		//	}
		//	if (aabb.min.y < 0)
		//	{
		//		correction.y = -aabb.min.y;
		//	if (aabb.max.x >= image.getWidth())
		//	{
		//		correction.x = image.getWidth() - aabb.max.x;
		//	}
		//	if (aabb.max.y >= image.getHeight())
		//	{
		//		correction.y = image.getHeight() - aabb.max.y;
		//	}

			// Apply correction
		//	player.translate(correction);
		//}


		if (Input::getButton("Reload"))
		{
			InitGame();
		}

		// Render loop.

		image.clear(Color::White);
		
		music.play();

		music.setLooping(true);

		image.drawSprite(background, camera);

		player.draw(image, camera);

		enemy.draw(image, camera);

		enemy.setTarget(&player);

		image.drawText(Font::Default, fps, 10, 10, Color::Black);

		window.present(image);


		Event e;
		while (window.popEvent(e))
		{
			switch (e.type)
			{
			case Event::Close:
				window.destroy(); 
				break;
			case Event::KeyPressed:
			{
				switch (e.key.code)
				{
				case KeyCode::Escape:
					window.destroy();
					break;
				case KeyCode::V:
					window.toggleVSync();
					break;
				case KeyCode::F11:
					window.toggleFullscreen();
					break;
				}
			}
			break;
			}
		}

		timer.tick();
		++frameCount;

		totalTime += timer.elapsedSeconds();
		if (totalTime > 1.0)
		{
			fps = fmt::format("FPS: {:.3f}", static_cast<double>(frameCount) / totalTime);

			std::cout << fps << std::endl;

			frameCount = 0;
			totalTime = 0.0;
		}
	}
	std::cout << "Thanks for playing" << std::endl; 

	return 0;
}