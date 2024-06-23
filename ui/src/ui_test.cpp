#include "ui.hpp"
#include "terminal.hpp"

using namespace rascii;


class MainMenu : public FloatingWindow
{
public:
	MainMenu(Ui * ui) : FloatingWindow{ui, {true, true}, {false, false}} { }
	~MainMenu() override { }

private:
	void on_init() override
	{
	}

	bool on_key(Key & key) override
	{
		return true;
	}

	void draw(Renderer & renderer, std::vector<Box> const & regions) override
	{
		//...
	}
};

class MainWindow : public TerminalFrameWindow
{
public:
	MainWindow(Ui * ui) : TerminalFrameWindow{ui} { }
	~MainWindow() override { }

private:
	void on_init() override
	{
		main_menu = add_child_window<MainMenu>();
	}


	bool on_key(Key & key) override
	{
		if (main_menu->is_visible())
		{
			return main_menu->on_key(key);
		}

		else if (key.key == SpecialKeys::ESC and key.flags == Special)
		{
			main_menu->show();
		}
		
		return TerminalFrameWindow::on_key(key);
	}


	void draw(Renderer & renderer, std::vector<Box> const & regions) override
	{
	}


	WindowHandle main_menu = nullptr;
};



int main()
{
	Terminal t;
	Ui ui<MainWindow>(t);
	ui.set_config_path("ui_test_config.hu");
	ui.load_config();
	ui.start();
};
