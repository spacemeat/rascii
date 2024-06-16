#pragma once

#include <array>
#include <cassert>
#include <cstring>
#include <locale>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

namespace rascii
{

enum class ColorDepth
{
	b4,
	b8,
	b24,
};


struct color_b4
{
	using value_t = uint8_t;
	value_t val;

	color_b4() : val(0) { }
	explicit color_b4(value_t value) : val(value) { }
	operator value_t() const { return val; }
};

struct color_b8
{
	using value_t = uint8_t;
	value_t val;

	color_b8() : val(0) { }
	explicit color_b8(value_t value) : val(value) { }
	operator value_t() const { return val; }
};

using color_b24 = std::array<uint8_t, 3>;

extern std::tuple<bool, int> u32_to_u8(char32_t codepoint, char8_t * buf);

extern char const ansi_fg_4[];// = "\x1b[38;3;";
extern char const ansi_bg_4[];// = "\x1b[48;3;";
extern char const ansi_fg_8[];// = "\x1b[38;5;";
extern char const ansi_bg_8[];// = "\x1b[48;5;";
extern char const ansi_fg_24[];// = "\x1b[38;2;";
extern char const ansi_bg_24[];// = "\x1b[48;2;";

extern char const ansi_single_underline_on[];// = "\x1b[4m";
extern char const ansi_double_underline_on[];// = "\x1b[21m";
extern char const ansi_any_underline_off[];// = "\x1b[24m";
extern char const ansi_overline_on[];// = "\x1b[53m";
extern char const ansi_overline_off[];// = "\x1b[55m";
extern char const ansi_italic_on[];// = "\x1b[3m";
extern char const ansi_italic_off[];// = "\x1b[23m";
extern char const ansi_strike_on[];// = "\x1b[9m";
extern char const ansi_strike_off[];// = "\x1b[29m";

extern void ctoa(char8_t * str, color_b4 c);
extern void ctoa(char8_t * str, color_b8 c);
extern void ctoa(char8_t * str, color_b24 c);

extern void ctoa(color_b4 from, char * to);
extern void ctoa(color_b8 from, char * to);
extern void ctoa(color_b24 from, char * to);

extern void convert_color(color_b4 const & color_in, color_b4 & color_out);
extern void convert_color(color_b4 const & color_in, color_b8 & color_out);
extern void convert_color(color_b4 const & color_in, color_b24 & color_out);
extern void convert_color(color_b8 const & color_in, color_b4 & color_out);
extern void convert_color(color_b8 const & color_in, color_b8 & color_out);
extern void convert_color(color_b8 const & color_in, color_b24 & color_out);
extern void convert_color(color_b24 const & color_in, color_b4 & color_out);
extern void convert_color(color_b24 const & color_in, color_b8 & color_out);
extern void convert_color(color_b24 const & color_in, color_b24 & color_out);


// fx flags
const uint8_t UNDERLINE = (1 << 0);
const uint8_t DOUBLE_UNDERLINE = (1 << 1);
const uint8_t OVERLINE = (1 << 2);
const uint8_t ITALIC = (1 << 3);
const uint8_t STRIKE = (1 << 4);

// char_size
// Two bits for char size; the rest for deltas
const uint8_t NEW_FG_COLOR = (1 << 2);		// differs from previous pixel
const uint8_t NEW_BG_COLOR = (1 << 3);		// differs from previous pixel
const uint8_t NEW_UNDERLINE = (1 << 4);
const uint8_t NEW_OVERLINE = (1 << 5);
const uint8_t NEW_ITALIC = (1 << 6);
const uint8_t NEW_STRIKE = (1 << 7);


struct FramePixel
{
	FramePixel(): fr(0), fg(0), fb(0), fx(0), br(0), bg(0), bb(0), sz_flags(0), ch{0, 0, 0, 0} {}
	uint8_t fr;
	uint8_t fg;
	uint8_t fb;
	uint8_t fx;
	uint8_t br;
	uint8_t bg;
	uint8_t bb;
	uint8_t sz_flags;
	char8_t ch[4]; // UTF-8 codepoint
};

struct RasterBase
{
public:
	RasterBase(uint width_cells, uint height_cells)
	: m_width(width_cells), m_height(height_cells) { }
	virtual ~RasterBase() { }

	uint get_width() const { return m_width; }
	uint get_height() const { return m_height; }

	virtual color_b24 get_pixel(uint x, uint y) = 0;
	virtual void set_pixel(uint x, uint y, color_b24 color) = 0;
	virtual void set_cell(uint c, uint r, char32_t const * ch, color_b24 const * fg,
			      color_b24 const * bg, uint * fx) = 0;
	virtual void set_cell(uint c, uint r, char8_t const * ch, int len, color_b24 const * fg,
			      color_b24 const * bg, uint * fx) = 0;
	virtual void set_cell_fg(uint c, uint r, color_b24 fg) = 0;
	virtual void set_cell_fg(uint idx, color_b24 fg) = 0;
	virtual void set_cell_bg(uint c, uint r, color_b24 fg) = 0;
	virtual void set_cell_bg(uint idx, color_b24 bg) = 0;
	virtual void set_cell_fx(uint c, uint r, uint fx) = 0;
	virtual void set_cell_fx(uint idx, uint fx) = 0;
	virtual void set_cell_ch(uint c, uint r, char32_t ch) = 0;
	virtual void set_cell_ch(uint idx, char32_t ch) = 0;
	virtual void set_cell_ch(uint c, uint r, char8_t const * ch, uint len) = 0;
	virtual void set_cell_ch(uint idx, char8_t const * ch, uint len) = 0;
	virtual std::string const & get_stream() = 0;

protected:
	uint m_width = 0;
	uint m_height = 0;
	uint sizeof_cell = 0;
	uint sizeof_frame = 0;
	std::vector<FramePixel> frame;
	std::string stream;
};


template <ColorDepth color_depth,
		  char const * const ansi_fg,
		  char const * const ansi_bg,
		  size_t color_prefix_size,
		  size_t color_string_size,
		  typename color_t>
struct Raster : public RasterBase
{
public:
	Raster(uint width_cells, uint height_cells)
	: RasterBase(width_cells, height_cells)
	{
		sizeof_cell = sizeof(FramePixel);
		sizeof_frame = width_cells * height_cells * (sizeof_cell);
		frame.reserve(sizeof_frame);
		stream.reserve(color_string_size + sizeof(char32_t) + 1);
		initialize_frame();
	}

	
	~Raster() { }


	void initialize_frame()
	{
		for (uint i = 0; i < m_width * m_height; ++i)
		{
			frame.push_back( FramePixel {} );
		}
	}


	bool is_pixel(uint x, uint y)
	{
		uint r = y / 2;

		if (x >= m_width || r >= m_height)
			{ return false; }

		auto cell_idx = r * m_width + x;
		FramePixel & cell = frame[cell_idx];
		return memcmp(cell.ch, u8"▀", 3) == 0 &&
					  cell.ch[4] == 0 &&
					  cell.fx == 0;
	}


	color_b24 get_pixel(uint x, uint y)
	{
		uint r = y / 2;

		if (x >= m_width || r >= m_height)
			{ return { 0, 0, 0 }; }

		auto cell_idx = r * m_width + x;
		FramePixel & cell = frame[cell_idx];

		if (y % 2 == 0)
			{ return { cell.fr, cell.fg, cell.fb }; }
		else
			{ return { cell.br, cell.bg, cell.bb }; }
	}


	void set_pixel(uint x, uint y, color_b24 color)
	{
		char8_t fgbgblock[] = u8"▀";
		int r = y / 2;
		if (y % 2 == 0)
			{ set_cell(x, r, fgbgblock, 3, & color, nullptr, 0); }
		else
			{ set_cell(x, r, fgbgblock, 3, nullptr, & color, 0); }
	}


	void set_cell(uint c, uint r, char32_t const * ch, color_b24 const * fg,
			      color_b24 const * bg, uint * fx)
	{
		if (c >= m_width || r >= m_height)
			{ return; }

		auto cell_idx = r * m_width + c;
		if (ch) { set_cell_ch(cell_idx, * ch); }
		if (fg) { set_cell_fg(cell_idx, * fg); }
		if (bg) { set_cell_bg(cell_idx, * bg); }
		if (fx) { set_cell_fx(cell_idx, * fx); }
	}


	void set_cell(uint c, uint r, char8_t const * ch, int len, color_b24 const * fg,
			      color_b24 const * bg, uint * fx)
	{
		if (c >= m_width || r >= m_height)
			{ return; }

		auto cell_idx = r * m_width + c;
		if (ch) { set_cell_ch(cell_idx, ch, len); }
		if (fg) { set_cell_fg(cell_idx, * fg); }
		if (bg) { set_cell_bg(cell_idx, * bg); }
		if (fx) { set_cell_fx(cell_idx, * fx); }
	}


	void set_cell_fg(uint c, uint r, color_b24 fg)
	{
		if (c >= m_width || r >= m_height)
			{ return; }

		auto cell_idx = r * m_width + c;
		set_cell_fg(cell_idx, fg);
	}


	void set_cell_fg(uint idx, color_b24 fg)
	{
		FramePixel & cell = frame[idx];
		if (idx > 0)
		{
			FramePixel & cellp = frame[idx - 1];
			if (cellp.fr != fg[0] || cellp.fg != fg[1] || cellp.fb != fg[2])
			{
				cell.sz_flags |= NEW_FG_COLOR;
			}
			else
			{
				cell.sz_flags &= ~NEW_FG_COLOR;
			}
		}
		else
			{ cell.sz_flags |= NEW_FG_COLOR; }

		cell.fr = fg[0];
		cell.fg = fg[1];
		cell.fb = fg[2];
		if (idx < m_width * m_height - 1)
		{
			FramePixel & cellp = frame[idx + 1];
			if (cellp.fr != fg[0] || cellp.fg != fg[1] || cellp.fb != fg[2])
			{
				cellp.sz_flags |= NEW_FG_COLOR;
			}
			else
			{
				cellp.sz_flags &= ~NEW_FG_COLOR;
			}
		}
	}

	
	void set_cell_bg(uint c, uint r, color_b24 bg)
	{
		if (c >= m_width || r >= m_height)
			{ return; }

		auto cell_idx = r * m_width + c;
		set_cell_bg(cell_idx, bg);
	}


	void set_cell_bg(uint idx, color_b24 bg)
	{
		FramePixel & cell = frame[idx];
		if (idx > 0)
		{
			FramePixel & cellp = frame[idx - 1];
			if (cellp.fr != bg[0] || cellp.bg != bg[1] || cellp.bg != bg[2])
			{
				cell.sz_flags |= NEW_BG_COLOR;
			}
			else
			{
				cell.sz_flags &= ~NEW_BG_COLOR;
			}
		}
		else
			{ cell.sz_flags |= NEW_BG_COLOR; }

		cell.br = bg[0];
		cell.bg = bg[1];
		cell.bb = bg[2];

		if (idx < m_width * m_height - 1)
		{
			FramePixel & cellp = frame[idx + 1];
			if (cellp.fr != bg[0] || cellp.bg != bg[1] || cellp.bg != bg[2])
			{
				cellp.sz_flags |= NEW_BG_COLOR;
			}
			else
			{
				cellp.sz_flags &= ~NEW_BG_COLOR;
			}
		}
	}


	void set_cell_fx(uint c, uint r, uint fx)
	{
		if (c >= m_width || r >= m_height)
			{ return; }

		auto cell_idx = r * m_width + c;
		set_cell_fx(cell_idx, fx);
	}


	void set_cell_fx(uint idx, uint fx)
	{
		FramePixel & cell = frame[idx];

		if (idx > 0)
		{
			FramePixel & cellp = frame[idx - 1];
			auto diff = cellp.fx ^ fx;
			if (diff)
			{
				if (diff & (UNDERLINE | DOUBLE_UNDERLINE))
					{ cell.sz_flags |= NEW_UNDERLINE; }
				else
					{ cell.sz_flags &= ~NEW_UNDERLINE; }
				if (diff & (OVERLINE))
					{ cell.sz_flags |= NEW_OVERLINE; }
				else
					{ cell.sz_flags &= ~NEW_OVERLINE; }
				if (diff & (ITALIC))
					{ cell.sz_flags |= NEW_ITALIC; }
				else
					{ cell.sz_flags &= ~NEW_ITALIC; }
				if (diff & (STRIKE))
					{ cell.sz_flags |= NEW_STRIKE; }
				else
					{ cell.sz_flags &= ~NEW_STRIKE; }
			}
		}
		else
			{ cell.sz_flags |= (NEW_UNDERLINE | NEW_OVERLINE | NEW_ITALIC | NEW_STRIKE); }

		cell.fx = fx;

		if (idx < m_width * m_height - 1)
		{
			FramePixel & cellp = frame[idx + 1];
			auto diff = cellp.fx ^ fx;
			if (diff)
			{
				if (diff & (UNDERLINE | DOUBLE_UNDERLINE))
					{ cellp.sz_flags |= NEW_UNDERLINE; }
				else
					{ cellp.sz_flags &= ~NEW_UNDERLINE; }
				if (diff & (OVERLINE))
					{ cellp.sz_flags |= NEW_OVERLINE; }
				else
					{ cellp.sz_flags &= ~NEW_OVERLINE; }
				if (diff & (ITALIC))
					{ cellp.sz_flags |= NEW_ITALIC; }
				else
					{ cellp.sz_flags &= ~NEW_ITALIC; }
				if (diff & (STRIKE))
					{ cellp.sz_flags |= NEW_STRIKE; }
				else
					{ cellp.sz_flags &= ~NEW_STRIKE; }
			}
		}
	}

	
	void set_cell_ch(uint c, uint r, char32_t ch)
	{
		if (c >= m_width || r >= m_height)
			{ return; }

		auto cell_idx = r * m_width + c;
		set_cell_ch(cell_idx, ch);
	}


	void set_cell_ch(uint idx, char32_t ch)
	{
		FramePixel & cell = frame[idx];
		char8_t * str = cell.ch;
		auto [res, len] = u32_to_u8(ch, str);
		if (res)
		{
			for (int i = 0; i < 4 - len; ++i)
				{ * str++ = u8'\0'; }
			cell.sz_flags = (cell.sz_flags & 0b11111100) + (len - 1);
		}
	}


	void set_cell_ch(uint c, uint r, char8_t const * ch, uint len)
	{
		if (c >= m_width || r >= m_height)
			{ return; }

		auto cell_idx = r * m_width + c;
		set_cell_ch(cell_idx, ch, len);
	}

	
	void set_cell_ch(uint idx, char8_t const * ch, uint len)
	{
		assert(len > 0 && len < 5);
		FramePixel & cell = frame[idx];
		char8_t * str = cell.ch;
		for (uint i = 0; i < len; ++i)
			{ str[i] = ch[i]; }
		for (uint i = len; i < 4; ++i)
			{ str[i] = u8'\0'; }

		cell.sz_flags = (cell.sz_flags & 0b11111100) + (len - 1);
	}


	std::string const & get_stream()
	{
		stream.clear();
		convert(stream, frame);
		stream.append("\x1b[0m");
		return stream;
	}

private:
	int convert(std::string & out, std::vector<FramePixel> const & in)
	{
		out.clear();

		for (uint i = 0; i < m_width * m_height; ++i)
		{
			auto & cell = in[i];
			char ansi_color[color_string_size];

			if (cell.sz_flags & NEW_FG_COLOR)
			{
				color_b24 cell_color {cell.fr, cell.fg, cell.fb};
				color_t color_conv;
				convert_color(cell_color, color_conv);
				ctoa(color_conv, ansi_color);
				out.append(ansi_fg, color_prefix_size);
				out.append(ansi_color, color_string_size);
			}
			if (cell.sz_flags & NEW_BG_COLOR)
			{
				color_b24 cell_color {cell.br, cell.bg, cell.bb};
				color_t color_conv;
				convert_color(cell_color, color_conv);
				ctoa(color_conv, ansi_color);
				out.append(ansi_bg, color_prefix_size);
				out.append(ansi_color, color_string_size);
			}
            if (cell.sz_flags & NEW_UNDERLINE)
			{
				if (cell.fx & DOUBLE_UNDERLINE)
					{ out.append(ansi_double_underline_on, strlen(ansi_double_underline_on)); }
			    else if (cell.fx & UNDERLINE)
					{ out.append(ansi_single_underline_on, strlen(ansi_single_underline_on)); }
				else
					{ out.append(ansi_any_underline_off, strlen(ansi_any_underline_off)); }
			}
            if (cell.sz_flags & NEW_OVERLINE)
			{
			    if (cell.fx & OVERLINE)
					{ out.append(ansi_overline_on, strlen(ansi_overline_on)); }
				else
					{ out.append(ansi_overline_off, strlen(ansi_overline_off)); }
			}
            if (cell.sz_flags & NEW_ITALIC)
			{
			    if (cell.fx & ITALIC)
					{ out.append(ansi_italic_on, strlen(ansi_italic_on)); }
				else
					{ out.append(ansi_italic_off, strlen(ansi_italic_off)); }
			}
            if (cell.sz_flags & NEW_STRIKE)
			{
			    if (cell.fx & STRIKE)
					{ out.append(ansi_strike_on, strlen(ansi_italic_on)); }
				else
					{ out.append(ansi_strike_off, strlen(ansi_italic_off)); }
			}
			for (uint a = 0; a < (cell.sz_flags & 0b00000011) + 1U; ++a)
			{
				char8_t const * ch8 = & cell.ch[a];
				char const * ch = reinterpret_cast<char const *>(ch8);
				out.push_back(*ch);
			}
		}

		return out.size();
	}
};


class Renderer
{
public:
	Renderer(std::tuple<uint, uint> extents, ColorDepth depth);
	~Renderer();
	static std::tuple<uint, uint> query_terminal_extents();
	static ColorDepth query_best_color_depth();

	void set_extents(std::tuple<uint, uint> extents);
	void set_color_depth(ColorDepth depth);
	void replace_raster();

	RasterBase & get_raster();

	void clear_raster(color_b24 const * fg);
	void print_str(uint x, uint y, std::string_view const str,
				   color_b24 * fg, color_b24 * bg, uint * fx);
	void print_str(uint x, uint y, std::u8string_view const str,
				   color_b24 * fg, color_b24 * bg, uint * fx);
	color_b24 get_pixel(uint x, uint y);
	void set_pixel(uint x, uint y, color_b24 color);
	void paint_raster();

private:
	uint m_width = 0;
	uint m_height = 0;
	ColorDepth m_color_depth;
	bool m_raster_is_dirty;
	std::unique_ptr<RasterBase> m_raster;
};

} // namesapce rascii
