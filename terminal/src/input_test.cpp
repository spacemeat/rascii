#include <iomanip>
#include <iostream>
#include "input.hpp"

using namespace rascii;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	Input inp;
	int esc_count = 0;

	inp.reset();
	while (esc_count < 2)
	{
		Key key = inp.get_key();
		if (key.is_key() == false)
			{ continue; }

		if (key.is_valid() == false)
		{
			std::cout << "invalid sequence";
		}
		
		for (auto c: key.get_sequence_view())
		{
			if (static_cast<int>(c) < 32 || static_cast<int>(c) == 127)
				{ std::cout << std::hex << std::setfill('0') << std::setw(2); }
			else
				{ std::cout << static_cast<char>(c); }
		}
	   
		std::cout << " :";

		for (auto c: key.get_sequence_view())
		{
			std::cout << ' ' << std::hex << std::setfill('0') << std::setw(2)
				      << static_cast<int>(c);
		}

		std::cout << " -";

		if (key.flags & Shift) { std::cout << " shift"; }
		if (key.flags & Alt) { std::cout << " alt"; }
		if (key.flags & Ctrl) { std::cout << " control"; }
		if (key.flags & Meta) { std::cout << " meta"; }
		if (key.flags & Special) { std::cout << " special"; }

		if (key.flags & Special)
		{
			std::cout << " - ";
			for (char8_t const * ch = get_special_key_name(static_cast<SpecialKey>(key.key));
				 * ch != static_cast<char8_t>(0); ++ch)
				{ std::cout << static_cast<char>(*ch); }
		}

		std::cout << "\n";

		if (key.flags == Special &&
			key.key == static_cast<char8_t>(SpecialKey::ESC))
			{ esc_count += 1; }
		else
			{ esc_count = 0; }
	}

	return 0;
}
