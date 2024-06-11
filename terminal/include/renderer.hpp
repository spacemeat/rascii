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

extern char8_t const b4_ansi[];//  = u8"\x1b[38;3;00m\x1b[38;3;00m";
constexpr uint fg_offset_4 = 7;
constexpr uint bg_offset_4 = 17;
extern char8_t const b8_ansi[];//  = u8"\x1b[38;5;000m\x1b[38;5;000m";
constexpr uint fg_offset_8 = 7;
constexpr uint bg_offset_8 = 18;
extern char8_t const b24_ansi[];// = u8"\x1b[38;2;000;000;000m\x1b[48;2;000;000;000m";
constexpr uint fg_offset_24 = 7;
constexpr uint bg_offset_24 = 26;

extern void ctoa(char8_t * str, color_b4 c);
extern void ctoa(char8_t * str, color_b8 c);
extern void ctoa(char8_t * str, color_b24 c);

extern void convert_color(color_b4 const & color_in, color_b4 & color_out);
extern void convert_color(color_b4 const & color_in, color_b8 & color_out);
extern void convert_color(color_b4 const & color_in, color_b24 & color_out);
extern void convert_color(color_b8 const & color_in, color_b4 & color_out);
extern void convert_color(color_b8 const & color_in, color_b8 & color_out);
extern void convert_color(color_b8 const & color_in, color_b24 & color_out);
extern void convert_color(color_b24 const & color_in, color_b4 & color_out);
extern void convert_color(color_b24 const & color_in, color_b8 & color_out);
extern void convert_color(color_b24 const & color_in, color_b24 & color_out);

struct RasterBase
{
public:
	RasterBase(uint width_cells, uint height_cells)
	: m_width(width_cells), m_height(height_cells) { }
	virtual ~RasterBase() { }

	uint get_width() const { return m_width; }
	uint get_height() const { return m_height; }

	virtual void set_pixel(uint x, uint y, color_b24 color);
	virtual void set_cell_colors(int cell, color_b24 const * fg, color_b24 const * bg);
	virtual void set_cell(uint c, uint r, char32_t const * ch, color_b24 const * fg,
					      color_b24 const * bg);
	virtual void set_cell(uint c, uint r, char8_t const * ch, int len, color_b24 const * fg,
					      color_b24 const * bg);
	virtual std::string const & get_stream();
	virtual std::string_view get_stream2();

protected:
	uint m_width = 0;
	uint m_height = 0;
	uint sizeof_cell = 0;
	uint sizeof_frame = 0;
	std::vector<char8_t> frame;
	std::string stream;
	std::u8string stream2;
};


template <ColorDepth color_depth, char8_t const * const color_string, size_t color_string_size,
		  typename color_t, uint fg_offset, uint bg_offset>
struct Raster : public RasterBase
{
public:
	Raster(uint width_cells, uint height_cells)
	: RasterBase(width_cells, height_cells)
	{
		// color, char8_t[4], size-of-utf8-cp
		sizeof_cell = color_string_size + sizeof(char32_t) + 1;
		sizeof_frame = width_cells * height_cells * (sizeof_cell);
		frame.reserve(sizeof_frame);
		stream.reserve(sizeof_frame);
		stream2.reserve(sizeof_frame + 10);
		initialize_frame();
	}
	
	~Raster() { }

	void initialize_frame()
	{
		for (uint i = 0; i < m_width * m_height; ++i)
		{
			for (uint j = 0; j < color_string_size; ++j)
				{ frame.push_back(color_string[j]); }
			frame.push_back(u8' ');
			frame.push_back(u8'\0');
			frame.push_back(u8'\0');
			frame.push_back(u8'\0');
			frame.push_back(u8'\x01');
		}
		assert(frame.size() == frame.capacity());
	}

	void set_pixel(uint x, uint y, color_b24 color)
	{
		//char8_t fgbgblock[] = u8"\x25\x80";
		char8_t fgbgblock[] = u8"▀";
		int r = y / 2;
		if (y % 2 == 0)
			{ set_cell(x, r, fgbgblock, 3, & color, nullptr); }
		else
			{ set_cell(x, r, fgbgblock, 3, nullptr, & color); }
	}

	void set_cell_colors(char8_t * cell_addr, color_b24 const * fg, color_b24 const * bg)
	{
		if (fg)
		{
			color_t color_conv;
			convert_color(* fg, color_conv);
			insert_color_fg(cell_addr, color_conv);
		}
		if (bg)
		{
			color_t color_conv;
			convert_color(* bg, color_conv);
			insert_color_bg(cell_addr, color_conv);
		}
	}

	void set_cell(uint c, uint r, char32_t const * ch, color_b24 const * fg,
			      color_b24 const * bg)
	{
		if (c >= m_width || r >= m_height)
			{ return; }

		auto cell_addr = frame.data() + (r * m_width + c) * sizeof_cell;
		set_cell_colors(cell_addr, fg, bg);
		if (ch)
			{ insert_char(cell_addr, * ch); }
	}

	void set_cell(uint c, uint r, char8_t const * ch, int len, color_b24 const * fg,
			      color_b24 const * bg)
	{
		if (c >= m_width || r >= m_height)
			{ return; }

		auto cell_addr = frame.data() + (r * m_width + c) * sizeof_cell;
		set_cell_colors(cell_addr, fg, bg);
		if (ch)
			{ insert_char(cell_addr, ch, len); }
	}

	std::string const & get_stream()
	{
		stream.clear();
		convert(stream, frame);
		stream.append("\x1b[0m");
		return stream;
	}

	std::string_view get_stream2()
	{
		return convert2(frame);
	}

private:
	void insert_color_fg(char8_t * cell_addr, color_t color)
	{
		ctoa(cell_addr + fg_offset, color);
	}

	void insert_color_bg(char8_t * cell_addr, color_t color)
	{
		ctoa(cell_addr + bg_offset, color);
	}

	void insert_char(char8_t * cell_addr, char32_t ch)
	{
		char8_t * str = cell_addr + color_string_size;
		auto [res, len] = u32_to_u8(ch, str);
		if (res)
		{
			for (int i = 0; i < 4 - len; ++i)
				{ * str++ = u8'\0'; }
			str[4] = static_cast<char8_t>(len);
		}
	}

	void insert_char(char8_t * cell_addr, char8_t const * ch, int len)
	{
		char8_t * str = cell_addr + color_string_size;
		for (int i = 0; i < len; ++i)
			{ str[i] = ch[i]; }
		for (int i = len; i < 4; ++i)
			{ str[i] = u8'\0'; }
		str[4] = static_cast<char8_t>(len);
	}

	std::u8string stream2;

	std::string_view convert2(std::vector<char8_t> const & in)
	{
		stream2.clear();

		int cell_size = (color_string_size + sizeof(char32_t) + 1);
		int num_cells = in.size() / cell_size;
		char8_t const * inc = in.data();

		for (int i = 0; i < num_cells; ++i)
		{
			int char_size = color_string_size + inc[cell_size - 1];
			for (int a = 0; a < char_size; ++a)
			{
				char8_t const * ch8 = inc + a;
				stream2.push_back(*ch8);
			}
			inc += cell_size;
		}
		stream2.append(u8"\x1b[0m");

		return { reinterpret_cast<char const *>(stream2.data()), stream2.size() };
	}

	int convert(std::string & out, std::vector<char8_t> const & in)
	{
		out.clear();

		int len_inword = (color_string_size + sizeof(char32_t) + 1);
		int len = in.size() / len_inword;
		char8_t const * inc = in.data();
		for (int i = 0; i < len; ++i)
		{
			int amt = color_string_size + inc[len_inword - 1];
			for (int a = 0; a < amt; ++a)
			{
				char8_t const * ch8 = inc + a;
				char const * ch = reinterpret_cast<char const *>(ch8);
				out.push_back(*ch);
			}
			inc += len_inword;
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
	void print_str(uint x, uint y, std::string_view const str, color_b24 * fg, color_b24 * bg);
	void print_str(uint x, uint y, std::u8string_view const str, color_b24 * fg, color_b24 * bg);
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
