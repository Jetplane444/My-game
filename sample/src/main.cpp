#include <Graphics/Window.hpp>

Graphics::Window window;

int main()
{
	window.create(L"PacMan", 800, 600);

	window.show();

	while (window)
	{
		// Do game logic...


		Graphics::Event e;
		while (window.popEvent(e))
		{
			switch (e.type)
			{
			case Graphics::Event::Close:
				window.destroy();
				break;
			case Graphics::Event::KeyPressed:
			{
				switch (e.key.code)
				{
				case Graphics::KeyCode::Escape:
					window.destroy();
					break;
				}
			}
			break;
			}
		}

	}

	return 0;
}