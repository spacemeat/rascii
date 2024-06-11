#pragma once

#include <functional>
#include <optional>
#include "renderer.hpp"
#include "input.hpp"


namespace rascii
{
	//using Handler_key_pressed = std::function<void(Key const &)>;
	//using Handler_on_loop = std::function<bool()>;

	class Terminal
	{
	public:
		Terminal();
		~Terminal();

		template <typename On_key_pressed, typename On_loop>
		void enter_loop(On_key_pressed && on_key_pressed, On_loop && on_loop)
		{
			bool running = true;
			while (running)
			{
				Key key = m_input->get_key();
				if (key.is_key())
				{
					on_key_pressed(key);
				}

				running = on_loop();
			}
		}

		Renderer & get_renderer() { return * m_renderer; }
		Input & get_input() { return * m_input; }

	private:
		std::optional<Renderer> m_renderer;
		std::optional<Input> m_input;
	};
}
