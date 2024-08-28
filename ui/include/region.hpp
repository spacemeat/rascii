#pragma once

#include <optional>
#include <vector>
#include <iostream>

using uint = unsigned int;

namespace rascii
{

struct Pos
{
	int x = 0;
	int y = 0;

	Pos operator +(Pos p) { return Pos { x + p.x, y + p.y }; }
	Pos operator -(Pos p) { return Pos { x - p.x, y - p.y }; }
	Pos & operator +=(Pos p) { x += p.x; y += p.y; return *this; }
	Pos & operator -=(Pos p) { x -= p.x; y -= p.y; return *this; }
};

inline bool operator ==(Pos a, Pos b) { return a.x == b.x && a.y == b.y; }

struct Size
{
	Size(int w = 0, int h = 0)
	: w {std::max(w, 0)}, h {std::max(h, 0)} { }

	int w = 0;
	int h = 0;
};

inline bool operator ==(Size a, Size b) { return a.w == b.w && a.h == b.h; }

enum class Overlap : uint
{
	NONE		= 0,
	LEFT		= 1 << 0,
	MID_HOR		= 1 << 1,
	RIGHT		= 1 << 2,
	UP			= 1 << 3,
	MID_VER		= 1 << 4,
	DOWN		= 1 << 5
};

inline constexpr uint operator |(auto a, auto b)
{
	return static_cast<uint>(a) | static_cast<uint>(b);
}

inline Overlap & operator |=(Overlap & lhs, Overlap rhs)
{
	lhs = static_cast<Overlap>(lhs | rhs);
	return lhs;
}

enum class Coverage : uint
{
	NONE =			0,
	UPPER_LEFT =	Overlap::LEFT | Overlap::UP,
	UPPER_MID =		Overlap::MID_HOR | Overlap::UP,
	UPPER_RIGHT =	Overlap::RIGHT | Overlap::UP,
	UPPER_CUT =		Overlap::LEFT | Overlap::RIGHT | Overlap::UP,
	MID_LEFT =		Overlap::LEFT | Overlap::MID_VER,
	MIDDLE =		Overlap::MID_HOR | Overlap::MID_VER,
	MID_RIGHT =		Overlap::RIGHT | Overlap::MID_VER,
	HOR_CUT =		Overlap::LEFT | Overlap::RIGHT | Overlap::MID_VER,
	LOWER_LEFT =	Overlap::LEFT | Overlap::DOWN,
	LOWER_MID =		Overlap::MID_HOR | Overlap::DOWN,
	LOWER_RIGHT =	Overlap::RIGHT | Overlap::DOWN,
	LOWER_CUT =		Overlap::LEFT | Overlap::RIGHT | Overlap::DOWN,
	LEFT_CUT =		Overlap::LEFT | Overlap::UP | Overlap::DOWN,
	VER_CUT =		Overlap::MID_HOR | Overlap::UP | Overlap::DOWN,
	RIGHT_CUT =		Overlap::RIGHT | Overlap::UP | Overlap::DOWN,
	FULL =			Overlap::LEFT | Overlap::RIGHT | Overlap::UP | Overlap::DOWN
};


struct Box
{
	Pos pos;
	Size size;

	operator bool() const { return size.w > 0 && size.h > 0; }

	bool operator < (Box const & rhs) const
    {
		return pos.y < rhs.pos.y ||
		       (pos.y == rhs.pos.y && pos.x < rhs.pos.x);
	}

	Box operator + (Pos p) { return Box { pos + p, size }; }
	Box operator - (Pos p) { return Box { pos - p, size }; }

	Box & operator += (Pos p) { pos += p; return *this; }
	Box & operator -= (Pos p) { pos -= p; return *this; }

	Box intersection_with(Box const & tool) const;
	std::vector<Box> cut_with(Box const & tool) const;
	std::vector<Box> union_with(Box const & tool) const;

private:
	Coverage compute_coverage(Box const & tool) const;
	Box intersection_with(Box const & tool, Coverage cover) const;
	std::vector<Box> cut_with(Box const & tool, Coverage cover) const;
	std::vector<Box> union_with(Box const & tool, Coverage cover) const;
};

std::ostream & operator <<(std::ostream & os, Pos const & value);
std::ostream & operator <<(std::ostream & os, Size const & value);
std::ostream & operator <<(std::ostream & os, Box const & value);


// Pre: lhs and rhs must not be overlapping. This is ensured by boolean operations on Box.
int merge_boxes_if_adjacent(Box lhs, Box rhs,
							std::array<std::optional<Box>, 3> & replacements);
bool is_boxes_sorted(std::vector<Box> const & boxes);
void sort_boxes(std::vector<Box> & boxes);
std::vector<Box> normalize_boxes(std::vector<Box> boxes);


// A region contains a number of boxes. Boxes -may- be adjacent or not, but -may not- overlap.
// The boxes must be in sorted order: The top-left corner is sorted by y, then by x.
class Region
{
	Region() = default;
	explicit Region(std::vector<Box> const & rhs);

	Region intersection_with(Region const & tool) const;
	Region cut_with(Region const & tool) const;
	Region union_with(Region const & tool) const;
	
private:
	std::vector<Box> m_boxes;
};

}

