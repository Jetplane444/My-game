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

#include <fmt/core.h>

#include <glm/vec2.hpp>

#include <iostream>

using namespace Graphics; 

Window window;
Image image; 
Sprite sprite; 
SpriteAnim idleAnim;
TileMap grassTiles;
SpriteAnim runAnim;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Math::Transform2D Player_transform;

float Player_x = SCREEN_WIDTH / 2;
float Player_y = SCREEN_HEIGHT / 2;
float Player_speed = 60.0f;

void InitGame()
{
	Player_transform.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
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
	
	auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Warrior/SpriteSheet/Warrior_SheetnoEffect.png", 64, 44, 0, 0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim{ idle_sprites, 12, {{0,1,2,3,4,5}} };
	runAnim = SpriteAnim{ idle_sprites, 12, {{6,7,8,9,10,11,12,13,}} };


	// Load tilemap.
	auto grass_sprites = ResourceManager::loadSpriteSheet("assets/PixelArt/TX Tileset Grass.png", 16, 16);
	grassTiles = TileMap{ grass_sprites, 30, 30 };
	
	for(int i = 0; i < 30; i++)
    {
		for (int j = 0; j < 30; j++)
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
		// update loop.
//		auto keyState = Keyboard::getState();

		// update the input state
		Input::update();

		Player_x += Input::getAxis("Horizontal") * Player_speed * timer.elapsedSeconds();
		Player_y -= Input::getAxis("Vertical") * Player_speed * timer.elapsedSeconds();

		//if (keyState.W)
		//{
		//	Player_y -= timer.elapsedSeconds();
		//}
		//if (keyState.S)
		//{
		//	Player_y += timer.elapsedSeconds();
		//}
		//if (keyState.A)
		//{
		//	Player_x -= timer.elapsedSeconds();
		//}
		//if (keyState.D)
		//{
		//	Player_x += timer.elapsedSeconds();
		//}



		

		// Render loop.

		image.clear(Color::White);

		grassTiles.draw(image); 

		image.drawText(Font::Default, fps, 10, 10, Color::Black);

		

		if (Input::getAxis("Horizontal") != 0 || Input::getAxis("Vertical") != 0) 
		{
			runAnim.update(timer.elapsedSeconds());
			image.drawSprite(runAnim, Player_x, Player_y);
		}
		else {
			idleAnim.update(timer.elapsedSeconds());
            image.drawSprite(idleAnim, Player_x, Player_y);
		}

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