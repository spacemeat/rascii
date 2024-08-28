#include <array>
#include "fmt/base.h"
#include "i18n.hpp"

using namespace rascii;

int main()
{
	auto inter = I18n {"test-demo"};
	inter.set_locale("en-US");
	auto strs = std::array {"app-name", "new-game", "resume-game", "load-game", "quit-game"};

	fmt::print("\n");
	for (auto && str: strs)
	{
		fmt::print("{}: {}\n", str, inter.loc_c_c(str));
	}
}
