#include <Graphics/Window.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>
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

#include <fmt/core.h>

#include <iostream>

using namespace Graphics; 

Window window;
Image image; 
Sprite sprite; 
SpriteAnim idleAnim; 

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


float Player_x = SCREEN_WIDTH / 2;
float Player_y = SCREEN_HEIGHT / 2;
float Player_speed = 60.0f;


int main()
{
	image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	window.create(L"PacMan", SCREEN_WIDTH, SCREEN_HEIGHT);
	window.show();
	window.setFullscreen(true);
	
	auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Idle.png", 137, 44);

	idleAnim = SpriteAnim{ idle_sprites, 6 };

	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";



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



		idleAnim.update(timer.elapsedSeconds());

		// Render loop.

		image.clear(Color::Black);

		image.drawText(Font::Default, fps, 10, 10, Color::White);

		image.drawSprite(idleAnim, Player_x, Player_y);

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