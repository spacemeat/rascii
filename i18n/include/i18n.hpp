#include "humon/humon.hpp"

namespace rascii
{

class I18n
{
public:
	I18n(std::string_view l10n_dir);
	void set_locale(std::string_view locale);
	std::string_view loc_c_c(std::string_view code);
	std::u8string_view loc_u8_u8(std::u8string_view code);

private:
	std::string m_l10n_dir;
	hu::Trove m_l10n_trove;
	hu::Node m_table;
};

} // namespace rascii
