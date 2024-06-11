#include "terminal.hpp"


namespace rascii
{

	Terminal::Terminal()
	{
		auto depth = Renderer::query_best_color_depth();
		auto extents = Renderer::query_terminal_extents();
		
		m_renderer.emplace(extents, depth);
		m_input.emplace();
	}


	Terminal::~Terminal()
	{
		m_input.reset();
		m_renderer.reset();
	}

}
