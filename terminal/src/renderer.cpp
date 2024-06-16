//#include <print>
#include <iostream>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#include <cstdlib>
#endif

#include "renderer.hpp"

namespace rascii
{

constexpr char const ansi_fg_4[] = "\x1b[38;3;";
constexpr char const ansi_bg_4[] = "\x1b[48;3;";
constexpr char const ansi_fg_8[] = "\x1b[38;5;";
constexpr char const ansi_bg_8[] = "\x1b[48;5;";
constexpr char const ansi_fg_24[] = "\x1b[38;2;";
constexpr char const ansi_bg_24[] = "\x1b[48;2;";

constexpr char const ansi_single_underline_on[] = "\x1b[4m";
constexpr char const ansi_double_underline_on[] = "\x1b[21m";
constexpr char const ansi_any_underline_off[] = "\x1b[24m";
constexpr char const ansi_overline_on[] = "\x1b[53m";
constexpr char const ansi_overline_off[] = "\x1b[55m";
constexpr char const ansi_italic_on[] = "\x1b[3m";
constexpr char const ansi_italic_off[] = "\x1b[23m";
constexpr char const ansi_strike_on[] = "\x1b[9m";
constexpr char const ansi_strike_off[] = "\x1b[29m";

void ctoa(color_b4 from, char * to)
{
	to[0] = '0' + from / 10;
	to[1] = '0' + from % 10;
	to[2] = 'm';
}

void ctoa(color_b8 from, char * to)
{
	to[0] = '0' + from / 100;
	to[1] = '0' + (from % 100) / 10;
	to[2] = '0' + ((from % 100) % 10);
	to[3] = 'm';
}

void ctoa(color_b24 from, char * to)
{
	rascii::ctoa(color_b8(from[0]), to);
	to[3] = ';';
	rascii::ctoa(color_b8(from[1]), to + 4);
	to[7] = ';';
	rascii::ctoa(color_b8(from[2]), to + 8);
}

std::tuple<bool, int> u32_to_u8(char32_t codepoint, char8_t * buf)
{
	static std::locale loc("");
	static std::codecvt<char32_t, char8_t, std::mbstate_t> const & cvt = 
		std::use_facet<std::codecvt<char32_t, char8_t, std::mbstate_t>>(loc);

	std::mbstate_t state;
	std::memset(& state, 0, sizeof(state));

	const char32_t * in;
	char8_t * out;
	std::codecvt_base::result res = cvt.out(state, & codepoint, 1 + & codepoint, in, buf, 
										    buf + 4, out);
	return { res == std::codecvt_base::result::ok || 
		     res == std::codecvt_base::result::noconv,
			 out - buf };
}


using Raster_b4  = Raster<ColorDepth::b4,  ansi_fg_4, ansi_bg_4,
						  sizeof(ansi_fg_4),  3,  color_b4>;
using Raster_b8  = Raster<ColorDepth::b8,  ansi_fg_8, ansi_bg_8,
					      sizeof(ansi_fg_8),  4,  color_b8>;
using Raster_b24 = Raster<ColorDepth::b24, ansi_fg_24, ansi_bg_24,
						  sizeof(ansi_fg_24), 12, color_b24>;

Renderer::Renderer(std::tuple<uint, uint> extents, ColorDepth depth)
{
	set_extents(extents);
	set_color_depth(depth);

	replace_raster();
}


Renderer::~Renderer()
{
	std::cout << "\x1b[?25h\n\x1b[0m";
}


std::tuple<uint, uint> Renderer::query_terminal_extents()
{
#if defined(_WIN32)

#elif defined(__linux__)
	struct winsize w;
	ioctl(fileno(stdout), TIOCGWINSZ, &w);
	uint width = (int) w.ws_col;
	uint height = (int) w.ws_row;
#endif

	return { width, height };
}

ColorDepth Renderer::query_best_color_depth()
{
#if defined(_WIN32)

#elif defined(__linux__)
	ColorDepth best_color_depth = ColorDepth::b4;

	char * evar = std::getenv("COLORTERM");
	if (evar != nullptr && strcmp(evar, "truecolor") == 0)
		{ best_color_depth = ColorDepth::b24; }
	else
	{
		evar = std::getenv("TERM");
		if (evar != nullptr && strcmp(evar, "xterm-256color") == 0)
			{ best_color_depth = ColorDepth::b8; }
	}
#endif

	return best_color_depth;
}

void Renderer::set_extents(std::tuple<uint, uint> extents)
{
	m_width = std::get<0>(extents);
	m_height = std::get<1>(extents);
	m_raster_is_dirty = true;
}

void Renderer::set_color_depth(ColorDepth color_depth)
{
	m_color_depth = color_depth;
	m_raster_is_dirty = true;
}

void Renderer::replace_raster()
{
	if (m_raster_is_dirty == false)
		{ return; }

	switch (m_color_depth)
	{
	case ColorDepth::b4:
		{
			std::unique_ptr<RasterBase> new_raster(new Raster_b4(m_width, m_height));
			// TODO: Conversion from previous raster
			m_raster = std::move(new_raster);
		}
		break;

	case ColorDepth::b8:
		{
			std::unique_ptr<RasterBase> new_raster(new Raster_b8(m_width, m_height));
			// TODO: Conversion from previous raster
			m_raster = std::move(new_raster);
		}
		break;

	case ColorDepth::b24:
		{
			std::unique_ptr<RasterBase> new_raster(new Raster_b24(m_width, m_height));
			// TODO: Conversion from previous raster
			m_raster = std::move(new_raster);
		}
		break;
	}

	m_raster_is_dirty = false;
}

RasterBase & Renderer::get_raster()
{
	return * m_raster.get();
}

void Renderer::clear_raster(color_b24 const * bg)
{
	auto && raster = get_raster();

	for (uint r = 0; r < raster.get_height(); ++r)
	{
		for (uint c = 0; c < raster.get_width(); ++c)
		{
			auto tx = u8' ';
			raster.set_cell(c, r, & tx, 1, nullptr, bg, 0);
		}
	}
}

void Renderer::print_str(uint c, uint r, std::string_view const str,
						 color_b24 * fg, color_b24 * bg, uint * fx)
{
	print_str(c, r, { reinterpret_cast<char8_t const *>(str.data()), str.size() }, fg, bg, fx);
}

void Renderer::print_str(uint c, uint r, std::u8string_view const str,
						 color_b24 * fg, color_b24 * bg, uint * fx)
{
	auto && raster = get_raster();
	size_t cursor = 0;
	while(r < raster.get_height())
	{
		while(c < raster.get_width())
		{
			int cus = 1;
			size_t cp_cursor = cursor;
			while(true)
			{
				if (cp_cursor >= str.size())
					{ return; }

				auto cu = str[cp_cursor++];
				if ((cu & 0x80) == 0)
					{ break; }
				else
				{
					cus += 1;
					if (cus > 4)
						{ return; }
				}
			}
			raster.set_cell(c, r, & str[cursor], cus, fg, bg, fx);
			cursor = cp_cursor;
			c += 1;
		}
		c = 0;
		r += 1;
	}
}


color_b24 Renderer::get_pixel(uint x, uint y)
{
	auto && raster = get_raster();
	return raster.get_pixel(x, y);
}


void Renderer::set_pixel(uint x, uint y, color_b24 color)
{
	auto && raster = get_raster();
	raster.set_pixel(x, y, color);
}


void Renderer::paint_raster()
{
	auto && raster = get_raster();

	auto str = raster.get_stream();
	std::cout << "\x1b[1;1H\x1b[?25l" << std::flush << str << std::flush;
}

} // using namespace rascii
