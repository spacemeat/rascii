#include "terminal.hpp"


using namespace rascii;


class Sesh
{
public:
	Sesh()
	{
		term.enter_loop(
			[this](Key & key){ on_key(key); },
			[this](){ return on_loop(); });
	}

	void on_key(Key const & key)
	{
		color_b24 fg = { 255, 255, 255 };
		term.get_renderer().print_str(3, 4, key.get_sequence_view(), & fg, nullptr, nullptr);

		if (key.flags == Special && key.key == static_cast<char8_t>(SpecialKey::ESC))
			{ esc_count += 1; }
		else
			{ esc_count = 0; }
	}

	bool on_loop()
	{
		return esc_count < 2;
	}

private:

	Terminal term;
	int esc_count = 0;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv)
{
	Sesh s;
	return 0;
}
