#include "humon.hpp"

namespace rascii
{

class I18n
{
public:
	I18n(std::string_vew l10n_dir);
	void set_locale(std::string_view locale);
	std::u8string_view loc_string(std::u8string_vew code);

private:
	std::string m_l10n_dir;
	hu::Trove m_l10n_trove;
	hu::Node m_table;
}
};

} // namespace rascii
