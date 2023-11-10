#include <Player.hpp>
#include <Enemy.hpp>
#include <LDtkLoader/Project.hpp>

#include <Level.hpp>
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
Level level;

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

	ldtk::Project project;
	project.loadFromFile("assets/Map.ldtk");
	
	// get a world
	const auto& world = project.getWorld();
	// get a level
	const auto& level1 = world.getLevel("Level_0");

	level = Level(project, world, level1);
	

	Sound music;

	music.loadMusic("audio/Theme.wav");
	music.setVolume(0.25f);
	
	

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


		if (Input::getButton("Reload"))
		{
			InitGame();
		}

		// Render loop.

		image.clear(Color::White);
		
		music.play();

		music.setLooping(true);

		//level.draw(image, camera);

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