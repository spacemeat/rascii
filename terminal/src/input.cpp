#include <cstdio>
#if defined(_WIN32)
#elif defined(__linux__)
#include <sys/select.h>
#endif

#include "input.hpp"

namespace rascii
{
	char8_t const * get_special_key_name(SpecialKey key_code)
	{
	    char8_t const * names[] = { u8"UNKNOWN",
			u8"ESC", u8"HOME", u8"INSERT", u8"DELETE", u8"END", u8"PGUP", u8"PGDN", u8"F0",
			u8"F1", u8"F2", u8"F3", u8"F4", u8"F5", u8"F6", u8"F7", u8"F8", u8"F9", u8"F10",
			u8"F11", u8"F12", u8"F13", u8"F14", u8"F15", u8"F16", u8"F17", u8"F18", u8"F19",
			u8"F20", u8"UP", u8"DOWN", u8"RIGHT", u8"LEFT", u8"KEYPAD_5", u8"BACKSPACE",
		};

		return names[static_cast<int>(key_code)];
	}

	SpecialKey map_vt_code(int code)
	{
		switch (code)
		{
		case 1: return SpecialKey::HOME;
		case 2: return SpecialKey::INSERT;
		case 3: return SpecialKey::DELETE;
		case 4: return SpecialKey::END;
		case 5: return SpecialKey::PGUP;
		case 6: return SpecialKey::PGDN;
		case 7: return SpecialKey::HOME;
		case 8: return SpecialKey::END;
		case 10: return SpecialKey::F0;
		case 11: return SpecialKey::F1;
		case 12: return SpecialKey::F2;
		case 13: return SpecialKey::F3;
		case 14: return SpecialKey::F4;
		case 15: return SpecialKey::F5;
		case 17: return SpecialKey::F6;
		case 18: return SpecialKey::F7;
		case 19: return SpecialKey::F8;
		case 20: return SpecialKey::F9;
		case 21: return SpecialKey::F10;
		case 23: return SpecialKey::F11;
		case 24: return SpecialKey::F12;
		case 25: return SpecialKey::F13;
		case 26: return SpecialKey::F14;
		case 28: return SpecialKey::F15;
		case 29: return SpecialKey::F16;
		case 31: return SpecialKey::F17;
		case 32: return SpecialKey::F18;
		case 33: return SpecialKey::F19;
		case 34: return SpecialKey::F20;
		default: return SpecialKey::UNKNOWN;
		}
	}

	SpecialKey map_xterm_code(char8_t code)
	{
		switch(code)
		{
		case 'A': return SpecialKey::UP;
		case 'B': return SpecialKey::DOWN;
		case 'C': return SpecialKey::RIGHT;
		case 'D': return SpecialKey::LEFT;
		case 'F': return SpecialKey::END;
		case 'G': return SpecialKey::KEYPAD_5;
		case 'H': return SpecialKey::HOME;
		default: return SpecialKey::UNKNOWN;
		}
	}

	Input::Input()
	{
#if defined(_WIN32)
#elif defined(__linux__)
		tcgetattr(STDIN_FILENO, & m_old_io);
		io_t new_io = m_old_io;
		new_io.c_iflag &= ~(IXON | INLCR | ICRNL);
		new_io.c_lflag &= ~(ICANON | ECHO | IEXTEN | ISIG);
		new_io.c_cc[VMIN] = 1;
		new_io.c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSANOW, & new_io);

		auto flags = fcntl(STDIN_FILENO, F_GETFL, 0);
		flags |= O_NONBLOCK;
		fcntl(STDIN_FILENO, F_SETFL, flags);
#endif
		reset();
	}

	Input::~Input()
	{
		auto flags = fcntl(STDIN_FILENO, F_GETFL, 0);
		flags &= ~O_NONBLOCK;
		fcntl(STDIN_FILENO, F_SETFL, flags);

		tcsetattr(STDIN_FILENO, TCSANOW, & m_old_io);
	}
	
	void Input::reset()
	{
		seq = 0;
		sequence.fill(0);
		m_last_time = std::chrono::high_resolution_clock::now();
	}

	Key Input::reset(Key const & key)
	{
		seq = 0;
		sequence.fill(0);
		return key;
	}

	void print_seq(KeySequence const & sequence)
	{
		for (auto c: sequence)
		{
			if (static_cast<int>(c) < 32)
				{ std::cout << "\\" << std::hex << std::setfill('0') << std::setw(2) 
						    << static_cast<int>(c); }
			else
				{ std::cout << static_cast<char>(c); }
		}
	   
		std::cout << " :";

		for (auto c: sequence)
		{
			std::cout << ' ' << std::hex << std::setfill('0') << std::setw(2)
				      << static_cast<int>(c);
		}
	}

	Key Input::get_key()
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::microseconds>(now - m_last_time)
			.count();

		if (delta > 10 * 1000L)
		{
			if (seq == 1 && sequence[0] == 27)
			{
				//std::cout << "esc |<- timeout\n";
				return reset(Key { static_cast<char8_t>(SpecialKey::ESC), Special, sequence });
			}
			if (seq == 2 && sequence[0] == 27 && sequence[1] == u8'[')
			{
				//std::cout << "esc[ |<- timeout\n";
				return reset(Key { u8'[', Alt, sequence });
			}
		}

		auto && [is, is_eof, ch] = getchar_nonblock();
		if (! is)
			{ return Key::None(); }

		m_last_time = now;

		sequence[seq++] = static_cast<char8_t>(ch);

		//std::cout << "dur: " << std::dec << delta << "; seq: " << seq 
		//		  << "; seq: ";
		//print_seq(sequence);
		//std::cout << '\n';

		if (seq >= sequence.size())
		{
			//std::cout << "BAD BITCH\n";
			return reset(Key::Invalid(sequence));
		}
		if (seq == 1 && sequence[0] == 0)
		{
			//std::cout << "ctrl+`\n";
			return reset(Key { u8'`', Ctrl, sequence });
		}
		if (seq == 1 && sequence[0] >= 1 && sequence[0] <= 26)
		{
			//std::cout << "ctrl+letter\n";
			return reset(Key { static_cast<char8_t>(sequence[0] + u8'A' - 1), Ctrl, sequence });
		}
		if (seq == 1 && sequence[0] >= 28 && sequence[0] <= 31)
		{
			//std::cout << "above esc\n";
			return reset(Key { sequence[0], Special, sequence });
		}
		if (seq == 1 && sequence[0] >= 32 && sequence[0] <= 126)
		{
			//std::cout << "printable\n";
			return reset(Key { sequence[0], 0, sequence });
		}
		if (seq == 1 && sequence[0] == 127)
		{
			//std::cout << "delete\n";
			return reset(Key { static_cast<char8_t>(SpecialKey::BACKSPACE), Special, sequence });
		}
		if (seq == 2 && sequence[0] == 27 && sequence[1] >= 1 && sequence[1] <= 26)
		{
			//std::cout << "esc+key (ctrl+alt+letter)\n";
			return reset(Key { static_cast<char8_t>(sequence[0] + u8'A' - 1), Ctrl | Alt, sequence });
		}
		if (seq == 2 && sequence[0] == 27 && sequence[1] != u8'[')
		{
			//std::cout << "esc+key (alt+key), key != '['\n";
			return reset(Key { sequence[1], Alt, sequence });
		}
		if (seq > 2 && sequence[0] == 27 && sequence[1] == u8'[')
		{
			if (sequence[seq - 1] >= u8'@' && sequence[seq - 1] <= u8'~')
			{
				//std::cout << "esc[ ansi sequence\n";
				return reset(parse_ansi_sequence());
			}
		}

		return Key::None();
	}

	Key Input::parse_ansi_sequence()
	{
		auto is_letter = [](char8_t ch){ return ch >= u8'A' && ch <= u8'Z'; };

		int nums[] = { 0, 0 };
		int num_idx = 0;

		int cur = 2;
		read_num(nums[num_idx], cur);
		if (nums[num_idx] > 0)
			{ num_idx += 1; }
		if (sequence[cur] == u8';')
		{
			cur += 1;
			read_num(nums[num_idx], cur);
		}
		if (sequence[cur] == u8'~')
		{
			char8_t keycode = static_cast<char8_t>(map_vt_code(nums[0]));
			int modifiers = nums[1];
			if (modifiers > 0)
				{ modifiers -= 1; }
			return Key { keycode, modifiers | Special, sequence };
		}
		else if (is_letter(sequence[cur]))
		{
			char8_t keycode = static_cast<char8_t>(map_xterm_code(sequence[cur]));
			int modifiers = nums[0];
			if (modifiers > 0)
				{ modifiers -= 1; }
			return Key { keycode, modifiers | Special, sequence };
		}
		else
		{
			return Key::Invalid(sequence);
		}
	}

	void Input::read_num(int & val, int & cur)
	{
		auto is_digit = [](char8_t ch){ return ch >= u8'0' && ch <= u8'9'; };
		val = 0;

		while (is_digit(sequence[cur]))
		{
			val *= 10;
			val += sequence[cur++] - u8'0';
		}
	}

	bool Input::keyboard_has_input()
	{
#if defined(_WIN32)

		return false;

#elif defined(__linux__)

		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		fd_set read_fds;
		FD_ZERO(& read_fds);
		FD_SET(STDIN_FILENO, & read_fds);

		select(STDIN_FILENO + 1, & read_fds, NULL, NULL, & tv);
		return FD_ISSET(STDIN_FILENO, & read_fds);
		
#endif
	}

	std::tuple<bool, bool, char> Input::getchar_nonblock()
	{
		auto ch = std::getchar();
		if (ch == EOF)
			{ return {false, true, static_cast<char>(ch)}; }
		return {true, false, ch};
	}
}
