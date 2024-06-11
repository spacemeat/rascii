#pragma once

#include <array>
#include <chrono>
#include <iostream>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#elif defined(__linux__)
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

namespace rascii
{

#if defined(_WIN32)
#elif defined(__linux__)
	using io_t = struct termios;
#endif

	constexpr uint Shift = 1 << 0;
	constexpr uint Alt = 1 << 1;
	constexpr uint Ctrl = 1 << 2;
	constexpr uint Meta = 1 << 3;
	constexpr uint Special = 1 << 7;

	enum class SpecialKey : char
	{
		UNKNOWN,
		ESC, HOME, INSERT, DELETE, END, PGUP, PGDN, F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10,
		F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, UP, DOWN, RIGHT, LEFT, KEYPAD_5,
		BACKSPACE,
		NUM_SPECIAL_KEYS
	};

	extern char8_t const * get_special_key_name(SpecialKey key_code);
	using KeySequence = std::array<char8_t, 15>;

	struct Key
	{
		static Key None() { return Key { 0, 0, {}}; }
		static Key Invalid(KeySequence sequence) { return Key { 0, 0, sequence }; }

		Key(char8_t key, uint flags, KeySequence sequence)
		: key(key), flags(flags), sequence(sequence) { }

		char8_t key = 0;
		uint flags = 0;
		KeySequence sequence = { 0 };

		std::u8string_view const get_sequence_view() const
			{ return { sequence.data(), sequence.size() }; }

		bool is_key() { return ! (key == 0 && flags == 0 && sequence[0] == 0); }
		bool is_valid() { return key != 0 || flags != 0 || sequence[0] == 0; }
	};

	class Input
	{
	public:
		Input();
		~Input();
		
		void reset();
		Key reset(Key const & key);
		Key get_key();

	private:
		Key parse_ansi_sequence();
		void read_num(int & val, int & cur);

		bool keyboard_has_input();
		std::tuple<bool, bool, char> getchar_nonblock();

		KeySequence sequence = { 0 };
		std::chrono::time_point<std::chrono::high_resolution_clock> m_last_time;
		uint seq = 0;
		io_t m_old_io;
	};
}
