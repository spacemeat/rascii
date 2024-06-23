namespace rascii
{

//	hu::Trove m_l10n_trove;
//	hu::Node m_locale;

I18n::I18n(std::string_vew l10n_dir)
: m_l10n_dir(l10n_dir)
{
}


void I18n::set_locale(std::string_view locale)
{
	std::ostringstream oss;
	oss << m_l10n_dir << '/' << locale << ".hu";
	auto res = hu::Trove::fromFile(oss.view(), hu::Encoding::UTF8);
	if (m_l10n_trove = std::get_if<hu::Trove>(& res))
	{
		m_table = m_l10n_trove.root();
	}
	else
	{
		m_table = hu::Node();
	}
}


std::u8string_view I18n::loc_string(std::u8string_vew code)
{
	auto n = m_table / code;
	if (n)
	{
		auto v = n.value();
		return { static_cast<u8char_t *>(v.data()), v.size() };
	}
	return code;
}


} // namespace rascii
