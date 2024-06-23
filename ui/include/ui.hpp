#pragma once

#include <concepts>
#include <limits>

namespace rascii
{
const uint INFINITE = std::numeric_limits<uint>::max();

struct Pos
{
	uint x = 0;
	uint y = 0;
};

struct Size
{
	uint w = 0;
	uint h = 0;
};

struct Box
{
	Pos pos;
	Size size;
};

struct Color_rgba
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

constexpr uint L = 1 << 0;
constexpr uint U = 1 << 1;
constexpr uint R = 1 << 2;
constexpr uint D = 1 << 3;
constexpr enum class BoxConnection { LEFT, UP, RIGHT, DOWN };

constexpr enum class BoxCellType {
	NONE, THIN, THICK, DOUBLE,
	ROUND,																// only corners
	//SEG_2, SEG_2_THICK, SEG_3, SEG_3_THICK, SEG_4, SEG_4_THICK,		// only LR and UD -- not supported yet
};

char8_t * get_box_cell(std::array<BoxCellType, 4> const & type);

struct Border
{
	BoxCellType m_cell_type;
	Color_rgba m_fg;
	Color_rgba m_bg;
};


using WindowHandle = Window *;

/*
class WindowHandle
{
public:
	WindowHandle()
	: m_window { nullptr } { }
	explicit WindowHandle(Window * window)
	: m_window { window } { }
	WindowHandle(WindowHandle const &) = delete;
	WindowHandle(WindowHandle && rhs)
	: m_window(std::move(rhs.m_window)
		{ rhs.m_window = nullptr; }
	~WindowHandle() { }
	WindowHandle & operator =(WindowHandle const &) = delete;
	WindowHandle & operator =(WindowHandle && rhs)
		{ std::swap(m_window, rhs.m_window); }

private:
	std::unique_ptr<Window> m_window;
};
*/

/*
virtual struct WindowConfig
{

	std::array<BoxCellType, 6> m_borders;	// L, U, R, D, H, V
	virtual void on_init(Window * window) { }
	virtual bool on_key(Window * window, Key & key) { return false; }
	virtual void draw(Window * window, std::vector<Box> const & regions) { }
};
*/

class Ui;

class Window
{
public:
	Window(Ui * ui,
		   WindowHandle parent_windoe,
		   std::array<bool, 2> resize_from_content = {true, true},
		   std::array<bool, 2> can_scroll = {true, true},
		   int relative_depth = 50,
		   int scroll_start_priority = 0)
	:	m_ui{ui},
		m_parent_window{parent_window},
		m_relative_depth{relative_depth},
		m_absolute_box{},
		m_resize_from_content{resize_from_content},
		m_can_scroll{can_scroll},
		m_min_size_when_scrolling{0, 0},
		m_scroll_start_priority{scroll_start_priority}
	{
	}

	virtual ~Window() { }
	
	void invalidate_region(Box rect) { /* TODO: this */ }

	template <typename WindowType, typename Args...>
	requires std::derived_from<WindowType, Window>
	WindowHandle add_child_window(Args ...args)
	{
		auto win = ui->manage_window(new WindowType(m_ui, ...args));
		add_child_window(win);
		return win;
	}

	bool add_child_window(WindowHandle window, int idx = -1)
	{
		if (m_child_windows.size() >= m_max_num_children)
			{ return false; }

		if (idx == -1)
			{ m_child_windows.push_back(win); }
		else
			{ m_child_windows.insert(std::next(m_child_windows.begin(), idx), window); }
		layout(relative_box);
		return true;
	}

	void remove_child_window(WindowHandle window)
	{
		m_child_windows.erase(window);
		layout(relative_box);
	}

	bool remove_child_window(int idx)
	{
		if (idx >= m_child_windows.size())
			{ return false; }

		auto it = std::next(m_child_windows.begin(), idx);
		m_child_windows.erase(it);
		layout(relative_box);
		return true;
	}

	void remove_all_child_windows()
	{
		m_child_windows.clear();
		layout(relative_box);
	}

	void destroy_self()
	{
		remove_all_child_windows();
		//...
	}

	void hide()
	{
		m_is_visible = false;
	}

	void show()
	{
		m_is_visible = true;
		layout();
	}

protected:
	virtual void on_init() = 0;
	virtual bool on_key(Key & key) = 0;
	virtual void draw(std::vector<Box> const & regions) = 0;

	virtual void layout(Box relative_box) = 0;

	virtual Size get_real_minimum_size() = 0;
	virtual Size get_ideal_minimum_size() = 0;

	Ui * m_ui;
	WindowHandle m_parent_window;
	std::vector<WindowHandle> m_child_windows;
	std::vector<WindowHandle> m_floating_windows;
	
	int m_relative_depth; // relative to parent window's depth
	Box m_relative_box;	// relative to parent window's relative_box
	Box m_absolute_box;
	bool m_resize_from_content[2];	// width, height
	bool m_can_scroll[2]; // width, height
	Size m_min_size_when_scrolling;
	uint m_scroll_start_priority;

	bool m_is_visible = false;
};


class FloatingWindow : public Window
{
public:
	FloatingWindow(Ui * ui, WindowHandle parent_window,
				   std::array<bool, 2> resize_from_content = {false, false},
				   std::array<bool, 2> can_scroll = {true, true},
			       int relative_depth = 50)
	:	Window{ui, parent_window, resize_from_content, can_scroll, relative_depth, 0}
	{
	}

protected:
	void layout(Box relative_box) override
	{
		// get child window's real minimum size, ideal minimum size
	}


private:

};


class ContentWindow : public Window
{
public:
	ContentWindow(Ui * ui, WindowHandle parent_windoe,
				   std::array<bool, 2> resize_from_content = {true, true},
				   std::array<bool, 2> can_scroll = {true, true},
			       int relative_depth = 50,
				   int scroll_start_priority = 0)
	:	Window{ui, parent_window, resize_from_content, can_scroll, relative_depth, 0}
	{
	}

protected:
	void layout(Box relative_box) override
	{
		
	}

	Size get_real_minimum_size() override
	{
		return min_size_when_scrolling;
	}

	Size get_ideal_minimum_size() override
	{
		return content_box;
	}

private:
	Box m_content_box;
};



class BorderWindow : public Window
{
public:
	BorderWindow(Ui * ui, WindowHandle parent_window, 
				   std::array<bool, 2> resize_from_content = {false, false},
				   int relative_depth = 200)
	:	Window{ui, parent_window, resize_from_content, can_scroll, relative_depth, 0}
	{
		Size sz = { 0, 0 };
		if (m_borders[L].m_cell_type != BoxCellType::NONE) { sz[0] += 1; }
		if (m_borders[U].m_cell_type != BoxCellType::NONE) { sz[1] += 1; }
		if (m_borders[R].m_cell_type != BoxCellType::NONE) { sz[0] += 1; }
		if (m_borders[D].m_cell_type != BoxCellType::NONE) { sz[1] += 1; }
		m_min_size_when_scrolling = sz;
	}

protected:
	virtual void on_init() { }


	virtual bool on_key(Key & key) { }


	virtual void draw(Renderer & renderer, std::vector<Box> const & regions)
	{
		// TODO: clip against regions
		auto x = m_absolute_box.pos.x;
		auto w = m_absolute_box.size.x;
		auto y = m_absolute_box.pos.y;
		auto h = m_absolute_box.size.y;

		auto bl = m_borders[L].m_cell_type;
		auto bu = m_borders[U].m_cell_type;
		auto br = m_borders[R].m_cell_type;
		auto bd = m_borders[D].m_cell_type;
		auto bn = BoxCellType::NONE;

		Color_b24 fg = m_border_color.fg;
		Color_b24 bg = m_border_color_bg;

		u8string_view glyph = renderer.get_box_cell({bn, bn, br, bd});
		renderer.print_str(x, y, glyph, & fg, & bg);

		glyph = renderer.get_box_cell({bl, bn, bn, bd});
		renderer.print_str(x + w - 1, y, glyph, & fg, & bg);

		glyph = renderer.get_box_cell({bl, bu, bn, bn});
		renderer.print_str(x + w - 1, y + h - 1, glyph, & fg, & bg);

		glyph = renderer.get_box_cell({bn, bu, br, bn});
		renderer.print_str(x, y + h - 1, glyph, & fg, & bg);

		glyph = renderer.get_box_cell({bl, bn, br, bn});
		for (uint x = x + 1; x < x + w - 1; ++x)
		{
			renderer.print_str(x, y, glyph, & fg, & bg);
			renderer.print_str(x, y + h - 1, glyph, & fg, & bg);
		}
		glyph = renderer.get_box_cell({bn, bu, bn, bd});
		for (uint y = y + 1; y < y + h - 1; ++y)
		{
			renderer.print_str(x, y, glyph, & fg, & bg);
			renderer.print_str(x + w - 1, y, glyph, & fg, & bg);
		}
	}


	void layout(uint dimension, uint relative_pos, uint extent) override
	{
		auto child = nullptr;
		if (m_child_windows.size() > 0)
			{ child = m_child_windows[0]; }

		uint be0 = (m_borders[dimension    ].m_cell_type == BoxCellType::NONE ? 0 : 1);
		uint be1 = (m_borders[dimension + 2].m_cell_type == BoxCellType::NONE ? 0 : 1);
		uint be01 = be0 + be1;
		if (m_resize_from_content[dimension])
		{
			if (child)
			{
				child->layout(dimension, relative_pos + be0, INFINITE);
				m_absolute_box.size[dimension] = child->m_absolute_box.size[dimension] + be01;
			}
			else
			{
				m_absolute_box.size[dimension] = be01;
			}
		}
		else
		{
			m_absolute_box.size[dimension] = be01;

			if (child)
			{
				uint inner_extent = extent - be01;
				child->layout(dimension, relative_pos + be0, inner_extent);
			}
			else
			{
			}
		}
	}

	Size get_real_minimum_size() override
	{
		Size sz = { 0, 0 };
		if (m_borders[L].m_cell_type != BoxCellType::NONE) { sz[0] += 1; }
		if (m_borders[U].m_cell_type != BoxCellType::NONE) { sz[1] += 1; }
		if (m_borders[R].m_cell_type != BoxCellType::NONE) { sz[0] += 1; }
		if (m_borders[D].m_cell_type != BoxCellType::NONE) { sz[1] += 1; }

		if (m_child_windows.size() > 0)
		{
			sz += m_child_windows[0].get_real_minimum_size();
		}

		for (auto && f : m_floating_windows)
		{
			sz = min(sz, f->get_real_minimum_size());
		}

		return sz;
	}

	Size get_ideal_minimum_size() override
	{
		Size sz = { 0, 0 };
		if (m_borders[L].m_cell_type != BoxCellType::NONE) { sz[0] += 1; }
		if (m_borders[U].m_cell_type != BoxCellType::NONE) { sz[1] += 1; }
		if (m_borders[R].m_cell_type != BoxCellType::NONE) { sz[0] += 1; }
		if (m_borders[D].m_cell_type != BoxCellType::NONE) { sz[1] += 1; }

		if (m_child_windows.size() > 0)
		{
			sz += m_child_windows[0].get_ideal_minimum_size();
		}

		for (auto && f : m_floating_windows)
		{
			sz = min(sz, f->get_ideal_minimum_size());
		}

		return sz;
	}

private:
	std::array<Border, 4> m_borders;	// L, U, R, D
	Color_b24 m_border_color_fg;
	Color_b24 m_border_color_bg;
	WindowHandle m_title_window;
};


class StackWindow : public Window
{
public:
	StackWindow();

private:
	bool m_is_vertical_stack;
	BoxCellType m_separators;
};


class TerminalFrameWindow : public Window
{
public:
	TerminalFrameWindow(Ui * ui)
	:	Window{ui, nullptr, {false, false}, {false, false}, 0, 0}
	{
	}

protected:
	virtual void on_init() { }


	virtual bool on_key(Key & key)
	{
		if ((key == 'd' || key == 'D') && key.flags == CTRL)
			{ return false; }

		return true;
	}


	virtual void draw(Renderer & renderer, std::vector<Box> const & regions)
	{
	}
};


class Ui
{
public:
	template <typename MainWindowType, typename ...Args>
	requires std::derived_from<MainWindowType, TerminalFrameWindow>
	Ui(Terminal & terminal, Args... args)
	: m_terminal(& terminal)
	{
		manage_window(new MainWindowType(this, args));
	}

	~Ui();

	void start()
	{
		m_terminal->enter_loop(
			[this](Key & key){ on_key(key); },
			[this](){ return on_loop(); });
	}

	void set_config_path(std::string_view path);
	void load_config();

	WindowHandle get_terminal_frame_window() const { return m_windows[0].get(); }
	WindowHandle manage_window(Window * window);

private:

	void on_key(Key & key)
	{
		m_windows[0]->on_key(key);
	}

	bool on_loop()
	{
		return m_windows[0]->on_loop();
	}

	std::string m_config_path;
	hu::Trove m_config_trove;
	hu::Node m_strings;
	hu::Node m_styles;
	Terminal * terminal;

	std::vector<std::unique_ptr<Window>> m_windows;
};


} // namespace rascii
