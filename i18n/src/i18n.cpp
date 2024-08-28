#include "i18n.hpp"

namespace rascii
{

I18n::I18n(std::string_view l10n_dir)
: m_l10n_dir(l10n_dir)
{
}


void I18n::set_locale(std::string_view locale)
{
	std::ostringstream oss;
	oss << m_l10n_dir << '/' << locale << ".hu";
	auto res = hu::Trove::fromFile(oss.view(), hu::Encoding::utf8);
	if (auto trove = std::get_if<hu::Trove>(& res))
	{
		m_l10n_trove = std::move(* trove);
		m_table = m_l10n_trove.root();
	}
	else
	{
		std::cout << "No trove at " << oss.view() << "\n";
		m_table = hu::Node();
	}
}


std::string_view I18n::loc_c_c(std::string_view code)
{
	auto n = m_table / code;
	if (n)
	{
		auto v = n.value();
		return { v.str().data(), v.str().size() };
	}
	return code;
}


std::u8string_view I18n::loc_u8_u8(std::u8string_view code)
{
	auto n = m_table / std::string_view { reinterpret_cast<char const *>(code.data()), code.size() };
	if (n)
	{
		auto v = n.value();
		return { reinterpret_cast<char8_t const *>(v.str().data()), v.str().size() };
	}
	return code;
}


} // namespace rascii
