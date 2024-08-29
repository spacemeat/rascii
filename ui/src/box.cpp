#include <array>
#include <algorithm>
#include <cassert>
#include "region.hpp"

namespace rascii
{
std::ostream & operator <<(std::ostream & os, Pos const & value)
{
	os << "{" << value.x << ", " << value.y << "}";
	return os;
}

std::ostream & operator <<(std::ostream & os, Size const & value)
{
	os << "{" << value.w << ", " << value.h << "}";
	return os;
}

std::ostream & operator <<(std::ostream & os, Box const & value)
{
	os << "{" << value.pos << "; " << value.size << "}";
	return os;
}


int merge_boxes_if_adjacent(Box a, Box b,
							std::array<std::optional<Box>, 3> & replacements)
{
	int num_repl = 0;

	// fail if they are not in sorted order
	if (a.pos.y > b.pos.y ||
		(a.pos.y == b.pos.y && a.pos.x > b.pos.x))
		{ return 0; }

	// if a and b are vertically stacked and adjacent
	if (a.pos.x == b.pos.x &&
		a.size.w == b.size.w &&
		a.pos.y + a.size.h == b.pos.y)
	{
		replacements[num_repl++].emplace(a.pos, Size { a.size.w, a.size.h + b.size.h });
		return num_repl;
	}

	// if a's right edge is b's left edge (implies that a's top edge is above or level with b's 
	// top edge, since a and b must be sorted)
	if (a.pos.x + a.size.w == b.pos.x)
	{
		// if a's bottom edge is above b's top edge, they are not connected
		if (a.pos.y + a.size.h <= b.pos.y)
			{ return 0; }

		// if a's top edge is strictly above b's top edge
		if (a.pos.y < b.pos.y)
		{
			// if a's bottom edge is above b's bottom edge
			if (a.pos.y + a.size.h < b.pos.y + b.size.h)
			{
				replacements[num_repl++].emplace(
					a.pos, Size { a.size.w, b.pos.y - a.pos.y });
				replacements[num_repl++].emplace(Pos { a.pos.x, b.pos.y },
								  Size { a.size.w + b.size.w,
										 a.pos.y + a.size.h - b.pos.y});
				replacements[num_repl++].emplace(Pos { b.pos.x, a.pos.y + a.size.h },
								  Size { a.size.w, (b.pos.y + b.size.h) - 
													 (a.pos.y + a.size.h) });
			}
			// if a's bottom edge is level with b's bottom edge
			else if (a.pos.y + a.size.h == b.pos.y + b.size.h)
			{
				replacements[num_repl++].emplace(a.pos,
					Size { a.size.w, b.pos.y - a.pos.y });
				replacements[num_repl++].emplace(Pos { a.pos.x, b.pos.y },
					Size { a.size.w + b.size.w, b.size.h });
			}
			// if a's bottom edge is below b's bottom edge
			else
			{
				replacements[num_repl++].emplace(
					a.pos, Size { a.size.w, b.pos.y - a.pos.y });
				replacements[num_repl++].emplace(Pos { a.pos.x, b.pos.y },
								  Size { a.size.w + b.size.w, b.size.h });
				replacements[num_repl++].emplace(Pos { a.pos.x, b.pos.y + b.size.h },
								  Size { a.size.w, (a.pos.y + a.size.h) - 
													 (b.pos.y + b.size.h) });
			}
		}
		// if a's top edge is level with b's top edge
		else if (a.pos.y == b.pos.y)
		{
			// if a's bottom edge is above b's bottom edge
			if (a.pos.y + a.size.h < b.pos.y + b.size.h)
			{
				replacements[num_repl++].emplace(a.pos,
					Size { a.size.w + b.size.w, a.size.h });
				replacements[num_repl++].emplace(Pos { b.pos.x, a.pos.y + a.size.h },
				    Size { b.size.w, (b.pos.y + b.size.h) - 
									 (a.pos.y + a.size.h) });
			}
			// if a's bottom edge is level with b's bottom edge
			else if (a.pos.y + a.size.h == b.pos.y + b.size.h)
			{
				replacements[num_repl++].emplace(
					a.pos, Size { a.size.w + b.size.w, a.size.h });
			}
			// if a's bottom edge is below b's bottom edge
			else
			{
				replacements[num_repl++].emplace(a.pos,
					Size { a.size.w + b.size.w, b.size.h });
				replacements[num_repl++].emplace(Pos { a.pos.x, b.pos.y + b.size.h },
					Size { a.size.w, (a.pos.y + a.size.h) - 
									 (b.pos.y + b.size.h) });
			}
		}
	}
	// if a's left edge is b's right edge (implies a's top edge is strictly above b's top edge)
	else if (a.pos.x == b.pos.x + b.size.w)
	{
		// if b's bottom edge is above a's top edge, they are not connected
		if (a.pos.y + a.size.h <= b.pos.y)
			{ return 0; }

		// if a's tottom edge is above b's bottom edge
		if (b.pos.y + b.size.h < a.pos.y + a.size.h)
		{
			replacements[num_repl++].emplace(
				a.pos, Size { a.size.w, b.pos.y - a.pos.y });
			replacements[num_repl++].emplace(
				b.pos, Size { b.size.w + a.size.w, b.size.h });
			replacements[num_repl++].emplace(Pos { a.pos.x, b.pos.y + b.size.h },
							  Size { a.size.w, (a.pos.y + a.size.h) - 
												 (b.pos.y + b.size.h) });
		}
		// if a's bottom edge is level with b's bottom edge
		else if (b.pos.y + b.size.h == a.pos.y + a.size.h)
		{
			replacements[num_repl++].emplace(
				a.pos, Size { a.size.w, b.pos.y - a.pos.y });
			replacements[num_repl++].emplace(
				b.pos, Size { b.size.w + a.size.w, b.size.h });
		}
		// if a's bottom edge is above b's bottom edge
		else
		{
			replacements[num_repl++].emplace(a.pos,
				Size { a.size.w, b.pos.y - a.pos.y });
			replacements[num_repl++].emplace(b.pos,
				Size { b.size.w + a.size.w,
					   b.pos.y + b.size.h - (a.pos.y + a.size.h) });
			replacements[num_repl++].emplace(Pos { b.pos.x, a.pos.y + a.size.h },
				Size { b.size.w, (b.pos.y + b.size.h) - 
								 (a.pos.y + a.size.h) });
		}
	}

	return num_repl;
}


bool is_boxes_sorted(std::vector<Box> const & boxes)
{
	if (boxes.size() < 2)
		{ return true; }

	for (auto it = boxes.begin(), itn = boxes.begin() + 1; itn != boxes.end(); ++it, ++itn)
	{
		if (*itn < *it)
			{ return false; }
	}

	return true;
}

void sort_boxes(std::vector<Box> & boxes)
{
	std::sort(boxes.begin(), boxes.end());
}

std::vector<Box> normalize_boxes(std::vector<Box> boxes)
{
	std::vector<Box> new_boxes;
	std::array<std::optional<Box>, 3> repl;
	do
	{
		sort_boxes(boxes);
		new_boxes.clear();
		std::vector<Box>::const_iterator bit_old = boxes.begin();
		
		new_boxes.push_back(*bit_old++);
		std::vector<Box>::iterator bit_new = new_boxes.begin();

		while(bit_old != boxes.end())
		{
			size_t num_repl = 0;
			num_repl = merge_boxes_if_adjacent(*bit_new, *bit_old, repl);

			if (num_repl == 0)
			{
				new_boxes.push_back(*bit_old);
				bit_new = new_boxes.end() - 1;
			}
			else
			{
				*bit_new = *repl[0];
				for (size_t i = 1; i < num_repl; ++i)
				{
					new_boxes.push_back(*repl[i]);
				}
				bit_new = new_boxes.end() - 1;
			}
			++bit_old;
		}

		boxes = new_boxes;
		std::cout << "Boxes: \n";
		for (auto & box : boxes)
			{ std::cout << box << "\n"; }
	} while (is_boxes_sorted(new_boxes) == false);

	return new_boxes;
}


Coverage Box::compute_coverage(Box const & tool) const
{
	if (! tool || ! *this)
		{ return Coverage::NONE; }

	Overlap cover = static_cast<Overlap>(Coverage::NONE);

	if (tool.pos.x <= pos.x)
	{
		if (tool.pos.x + tool.size.w > pos.x)
			{ cover |= Overlap::LEFT; }
		if (tool.pos.x + tool.size.w >= pos.x + size.w)
			{ cover |= Overlap::RIGHT; }
	}
	else if (tool.pos.x < pos.x + size.w)
	{
		if (tool.pos.x + tool.size.w >= pos.x + size.w)
			{ cover |= Overlap::RIGHT; }
		else
			{ cover |= Overlap::MID_HOR; }
	}
	if (tool.pos.y <= pos.y)
	{
		if (tool.pos.y + tool.size.h > pos.y)
			{ cover |= Overlap::UP; }
		if (tool.pos.y + tool.size.h >= pos.y + size.h)
			{ cover |= Overlap::DOWN; }
	}
	else if (tool.pos.y < pos.y + size.h)
	{
		if (tool.pos.y + tool.size.h >= pos.y + size.h)
			{ cover |= Overlap::DOWN; }
		else
			{ cover |= Overlap::MID_VER; }
	}
	
	return static_cast<Coverage>(cover);
}

Box Box::intersection_with(Box const & tool, Coverage cover) const
{
	auto & self = *this;

	switch (cover)
	{
	case Coverage::NONE:
		return Box {};

	case Coverage::UPPER_LEFT:
		return Box { Pos { self.pos.x, self.pos.y }, 
			         Size { tool.size.w - (self.pos.x - tool.pos.x),
							tool.size.h - (self.pos.y - tool.pos.y) } };
    case Coverage::UPPER_MID:
		return Box { Pos { tool.pos.x, self.pos.y }, 
			         Size { tool.size.w,
					        tool.size.h - (self.pos.y - tool.pos.y) } };
    case Coverage::UPPER_RIGHT:
		return Box { Pos { tool.pos.x, self.pos.y }, 
			         Size { self.size.w - (tool.pos.x - self.pos.x),
							tool.size.h - (self.pos.y - tool.pos.y) } };
    case Coverage::UPPER_CUT:
		return Box { Pos { self.pos.x, self.pos.y }, 
			         Size { self.size.w,
							tool.pos.y + tool.size.h - self.pos.y } };
    case Coverage::MID_LEFT:
		return Box { Pos { self.pos.x, tool.pos.y }, 
			         Size { tool.size.w - (self.pos.x - tool.pos.x),
							tool.size.h } };
    case Coverage::MIDDLE:
		return tool;

    case Coverage::MID_RIGHT:
		return Box { Pos { tool.pos.x, tool.pos.y }, 
			         Size { self.size.w - (tool.pos.x - self.pos.x),
							tool.size.h } };
    case Coverage::HOR_CUT:
		return Box { Pos { self.pos.x, tool.pos.y }, 
			         Size { self.size.w,
					        tool.size.h } };
    case Coverage::LOWER_LEFT:
		return Box { Pos { self.pos.x, tool.pos.y }, 
			         Size { tool.size.w - (self.pos.x - tool.pos.x),
							self.size.h - (tool.pos.y - self.pos.y) } };
    case Coverage::LOWER_MID:
		return Box { Pos { tool.pos.x, tool.pos.y }, 
			         Size { tool.size.w,
							self.size.h - (tool.pos.y - self.pos.y) } };
    case Coverage::LOWER_RIGHT:
		return Box { Pos { tool.pos.x, tool.pos.y }, 
			         Size { self.size.w - (tool.pos.x - self.pos.x),
							self.size.h - (tool.pos.y - self.pos.y) } };
    case Coverage::LOWER_CUT:
		return Box { Pos { self.pos.x, tool.pos.y }, 
			         Size { self.size.w,
							self.pos.y + self.size.h - tool.pos.y } };
    case Coverage::LEFT_CUT:
		return Box { Pos { self.pos.x, self.pos.y }, 
			         Size { tool.pos.x + tool.size.w - self.pos.x,
					        self.size.h } };
    case Coverage::VER_CUT:
		return Box { Pos { tool.pos.x, self.pos.y }, 
			         Size { tool.size.w,
					        self.size.h } };
    case Coverage::RIGHT_CUT:
		return Box { Pos { tool.pos.x, self.pos.y }, 
			         Size { self.pos.x + self.size.w - tool.pos.x,
					        self.size.h } };
    case Coverage::FULL:
		return self;

	default:
		return Box {};
	}
}

Box Box::intersection_with(Box const & tool) const
{
	Coverage cover = compute_coverage(tool);
	return intersection_with(tool, cover);
}

std::vector<Box> Box::cut_with(Box const & tool, Coverage cover) const
{
	auto && intr = intersection_with(tool, cover);

	auto & self = *this;
	
	std::vector<Box> bvec;
	std::array<Box, 4> boxes;
	int num_boxes = 0;
	Box a, b, c;

	switch (cover)
	{
    case Coverage::NONE:
		boxes[num_boxes++] = self;
		break;

    case Coverage::UPPER_LEFT:
		boxes[num_boxes++] = Box { Pos { self.pos.x + intr.size.w, self.pos.y }, 
				  Size { self.size.w - intr.size.w,
						 intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y + intr.size.h },
				  Size { self.size.w,
		                 self.size.h - intr.size.h } };
		break;

    case Coverage::UPPER_MID:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y }, 
				  Size { intr.pos.x - self.pos.x,
						 intr.size.h } };
		boxes[num_boxes++] = Box { Pos { intr.pos.x + intr.size.w, self.pos.y }, 
				  Size { self.pos.x + self.size.w - (intr.pos.x + intr.size.w),
						 intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y + intr.size.h },
				  Size { self.size.w,
		                 self.size.h - intr.size.h } };
		break;

    case Coverage::UPPER_RIGHT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y }, 
				  Size { intr.pos.x - self.pos.x,
						 intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y + intr.size.h },
				  Size { self.size.w,
		                 self.size.h - intr.size.h } };
		break;

	case Coverage::UPPER_CUT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, tool.pos.y + tool.size.h },
				  Size { self.size.w,
						 self.size.h - intr.size.h } };
		break;

    case Coverage::MID_LEFT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
				  Size { self.size.w,
						 intr.pos.y - self.pos.y } };
		boxes[num_boxes++] = Box { Pos { self.pos.x + intr.size.w, intr.pos.y },
				  Size { self.size.w - intr.size.w,
					     intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y + intr.size.h },
				  Size { self.size.w,
						 self.pos.y + self.size.h - (intr.pos.y + intr.size.h) } };
		break;

    case Coverage::MIDDLE:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
				  Size { self.size.w,
						 intr.pos.y - self.pos.y } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y - self.pos.y },
				  Size { intr.pos.x - self.pos.x,
					     intr.size.h } };
		boxes[num_boxes++] = Box { Pos { intr.pos.x + intr.size.w, intr.pos.y - self.pos.y },
				  Size { self.pos.x + self.size.w - (intr.pos.x + intr.size.w),
					     intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y + intr.size.h },
				  Size { self.size.w,
						 self.pos.y + self.size.h - (intr.pos.y + intr.size.h) } };
		break;

    case Coverage::MID_RIGHT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
				  Size { self.size.w,
						 intr.pos.y - self.pos.y } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y },
				  Size { self.size.w - intr.size.w,
					     intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y + intr.size.h },
				  Size { self.size.w,
						 self.pos.y + self.size.h - (intr.pos.y + intr.size.h) } };
		break;

    case Coverage::HOR_CUT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
			  Size { self.size.w,
					 intr.pos.y - self.pos.y } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y + intr.size.h },
			  Size { self.size.w,
					 self.pos.y + self.size.h - (intr.pos.y + intr.size.h) } };
		break;

    case Coverage::LOWER_LEFT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
			  Size { self.size.w,
					 self.size.h - intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x + intr.size.h, self.pos.y },
			  Size { self.size.w - intr.size.w,
					 intr.size.h } };
		break;

    case Coverage::LOWER_MID:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
			  Size { self.size.w,
					 self.size.h - intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y }, 
			  Size { intr.pos.x - self.pos.x,
					 intr.size.h } };
		boxes[num_boxes++] = Box { Pos { intr.pos.x + intr.size.w, intr.pos.y }, 
			  Size { self.pos.x + self.size.w - (intr.pos.x + intr.size.w),
					 intr.size.h } };
		break;

    case Coverage::LOWER_RIGHT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
			  Size { self.size.w,
					 self.size.h - intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y },
			  Size { self.size.w - intr.size.w,
					 intr.size.h } };
		break;

	case Coverage::LOWER_CUT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y }, 
			  Size { self.size.w,
					 self.size.h - intr.size.h } };
		break;

	case Coverage::LEFT_CUT:
		boxes[num_boxes++] = Box { Pos { self.pos.x + intr.size.w, self.pos.y },
			  Size { self.size.w - intr.size.w,
					 self.size.h } };
		break;

    case Coverage::VER_CUT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
			  Size { intr.pos.x - self.pos.x,
					 self.size.h } };
		boxes[num_boxes++] = Box { Pos { intr.pos.x + intr.size.w, self.pos.y },
			  Size { self.pos.x + self.size.w - (intr.pos.x + intr.size.w),
					 self.size.h } };
		break;

	case Coverage::RIGHT_CUT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
			  Size { self.size.w - intr.size.w,
					 self.size.h } };
		break;

    case Coverage::FULL:
		break;

	default:
		boxes[num_boxes++] = self;
		break;
	}

	for (auto & b: boxes)
	   { if (b) { bvec.push_back(b); } }

	return bvec;
}

std::vector<Box> Box::cut_with(Box const & tool) const
{
	Coverage cover = compute_coverage(tool);
	return cut_with(tool, cover);
}

std::vector<Box> Box::union_with(Box const & tool, Coverage cover) const
{
	auto && intr = intersection_with(tool, cover);

	auto & self = *this;
	
	std::vector<Box> bvec;
	std::array<Box, 3> boxes;
	int num_boxes = 0;

	switch (cover)
	{
    case Coverage::NONE:
		if (self.pos.y < tool.pos.y)
	    {
			boxes[num_boxes++] = self;
			boxes[num_boxes++] = tool;
		}
	    else if (tool.pos.y < self.pos.y)
	    {
			boxes[num_boxes++] = tool;
			boxes[num_boxes++] = self;
		}
	    else if (self.pos.x <= tool.pos.x)
	    {
			boxes[num_boxes++] = self;
			boxes[num_boxes++] = tool;
		}
		else
		{
			boxes[num_boxes++] = tool;
			boxes[num_boxes++] = self;
		}
		break;

    case Coverage::UPPER_LEFT:
		boxes[num_boxes++] = Box { Pos { tool.pos.x, tool.pos.y },
				  Size { tool.size.w,
						 self.pos.y - tool.pos.y } };
		boxes[num_boxes++] = Box { Pos { tool.pos.x, self.pos.y }, 
				  Size { self.pos.x - tool.pos.x + self.size.w,
						 intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y + intr.size.h },
				  Size { self.size.w,
		                 self.size.h - intr.size.h } };
		break;

    case Coverage::UPPER_MID:
		boxes[num_boxes++] = Box { Pos { tool.pos.x, tool.pos.y }, 
					   Size { tool.size.w,
						      self.pos.y - tool.pos.y } };
	    boxes[num_boxes++] = self;
		break;

    case Coverage::UPPER_RIGHT:
		boxes[num_boxes++] = Box { Pos { tool.pos.x, tool.pos.y },
				  Size { tool.size.w,
						 self.pos.y - tool.pos.y } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y }, 
				  Size { tool.pos.x - self.pos.x + tool.size.w,
						 intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y + intr.size.h },
				  Size { self.size.w,
		                 self.size.h - intr.size.h } };
		break;

	case Coverage::UPPER_CUT:
		boxes[num_boxes++] = tool;
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y + intr.size.h },
		          Size { self.size.w,
		                 self.size.h - intr.size.h } };
		break;

    case Coverage::MID_LEFT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
				  Size { self.size.w,
						 intr.pos.y - self.pos.y } };
		boxes[num_boxes++] = Box { Pos { tool.pos.x, tool.pos.y },
				  Size { self.size.w + tool.size.w - intr.size.w,
					     intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y + intr.size.h },
				  Size { self.size.w,
						 self.pos.y + self.size.h - (intr.pos.y + intr.size.h) } };
		break;

    case Coverage::MIDDLE:
		boxes[num_boxes++] = tool;
		break;

    case Coverage::MID_RIGHT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
				  Size { self.size.w,
						 intr.pos.y - self.pos.y } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, tool.pos.y },
				  Size { self.size.w + tool.size.w - intr.size.w,
					     intr.size.h } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y + intr.size.h },
				  Size { self.size.w,
						 self.pos.y + self.size.h - (intr.pos.y + intr.size.h) } };
		break;

    case Coverage::HOR_CUT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
				  Size { self.size.w,
						 intr.pos.y - self.pos.y } };
	    boxes[num_boxes++] = tool;
		boxes[num_boxes++] = Box { Pos { self.pos.x, intr.pos.y + intr.size.h },
				  Size { self.size.w,
						 self.pos.y + self.size.h - (intr.pos.y + intr.size.h) } };
		break;

    case Coverage::LOWER_LEFT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
				  Size { self.size.w,
						 tool.pos.y - self.pos.y } };
		boxes[num_boxes++] = Box { Pos { tool.pos.x, tool.pos.y },
				  Size { self.pos.x - tool.pos.x + self.size.w,
						 intr.size.h } };
		boxes[num_boxes++] = Box { Pos { tool.pos.x, self.pos.y + self.size.h },
				  Size { tool.size.w,
						 self.size.h + tool.size.h - intr.size.h } };
		break;

    case Coverage::LOWER_MID:
		boxes[num_boxes++] = self;
	    boxes[num_boxes++] = Box { Pos { tool.pos.x, self.pos.y + self.size.h }, 
				  Size { tool.size.w,
						 self.size.h + tool.size.h - intr.size.h } };
		break;

    case Coverage::LOWER_RIGHT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
				  Size { self.size.w,
						 tool.pos.y - self.pos.y } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, tool.pos.y },
				  Size { tool.pos.x - self.pos.x + self.size.w,
						 intr.size.h } };
		boxes[num_boxes++] = Box { Pos { tool.pos.x, self.pos.y + self.size.h },
				  Size { tool.size.w,
						 self.size.h + tool.size.h - intr.size.h } };
		break;

	case Coverage::LOWER_CUT:
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
		          Size { self.size.w,
		                 self.size.h - intr.size.h } };
		boxes[num_boxes++] = tool;
		break;

	case Coverage::LEFT_CUT:
		boxes[num_boxes++] = Box { Pos { tool.pos.x, tool.pos.y },
				  Size { tool.size.w,
						 intr.pos.y - tool.pos.y } };
		boxes[num_boxes++] = Box { Pos { tool.pos.x, self.pos.y },
				  Size { self.size.w + self.size.w - intr.size.w,
				         intr.size.h } };
		boxes[num_boxes++] = Box { Pos { tool.pos.x, intr.pos.y + intr.size.w },
				  Size { self.size.w,
						 tool.pos.y + tool.size.h - (self.pos.y + self.size.h) } };
		break;

    case Coverage::VER_CUT:
		boxes[num_boxes++] = Box { Pos { tool.pos.x, tool.pos.y },
				  Size { tool.size.w,
						 intr.pos.y - tool.pos.y } };
		boxes[num_boxes++] = self;
		boxes[num_boxes++] = Box { Pos { tool.pos.x, intr.pos.y + intr.size.h },
				  Size { self.size.w,
						 tool.pos.y + tool.size.h - (self.pos.y + self.size.h) } };
		break;

	case Coverage::RIGHT_CUT:
		boxes[num_boxes++] = Box { Pos { tool.pos.x, tool.pos.y },
				  Size { tool.size.w,
						 intr.pos.y - tool.pos.y } };
		boxes[num_boxes++] = Box { Pos { self.pos.x, self.pos.y },
				  Size { self.size.w + self.size.w - intr.size.w,
				         intr.size.h } };
		boxes[num_boxes++] = Box { Pos { tool.pos.x, intr.pos.y + intr.size.w },
				  Size { self.size.w,
						 tool.pos.y + tool.size.h - (self.pos.y + self.size.h) } };
		break;

    case Coverage::FULL:
		boxes[num_boxes++] = tool;
	    break;

	default:
		if (self.pos.y < tool.pos.y)
	    {
			boxes[num_boxes++] = self;
			boxes[num_boxes++] = tool;
		}
	    else if (tool.pos.y < self.pos.y)
	    {
			boxes[num_boxes++] = tool;
			boxes[num_boxes++] = self;
		}
	    else if (self.pos.x <= tool.pos.x)
	    {
			boxes[num_boxes++] = self;
			boxes[num_boxes++] = tool;
		}
		else
		{
			boxes[num_boxes++] = tool;
			boxes[num_boxes++] = self;
		}
		break;
	}

	for (auto & b: boxes)
	   { if (b) { bvec.push_back(b); } }

	return bvec;
}

std::vector<Box> Box::union_with(Box const & tool) const
{
	Coverage cover = compute_coverage(tool);
	return union_with(tool, cover);
}
}
