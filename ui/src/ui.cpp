#include "ui.hpp"

using std::literals::string_view;

namespace rascii
{


Ui::~Ui()
{
	m_windows[0].destroy_self();
}


void Ui::set_config_path(std::string_view path)
{
	m_config_path = path;
}


void Ui::load_config()
{
	auto res = hu::Trove::fromFile(m_config_path, hu::Encoding::UTF8);
	if (m_config_trove = std::get_if<hu::Trove>(& res))
	{
		m_strings = m_config_trove.root() / "strings";
		m_styles = m_config_trove.root() / "styles";
	}
	else
	{
		m_strings = hu::Node();
		m_styles = hu::Node();
	}
}


WindowHandle Ui::manage_window(Window * window)
{
	m_windows.push_back(window);

	return window;
}

}
