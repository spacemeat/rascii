#include <set>
#include "region.hpp"

namespace rascii
{

Region::Region(std::vector<Box> const & rhs)
: m_boxes(rhs) { }


Region Region::intersection_with(Region const & tool) const
{
	// Build a list of boxes, each the intersection of each of self and tool's boxes.
	// Then simplify.
	
	std::set<Box> inters;
	for (auto & b0 : m_boxes)
	{
		for (auto & b1 : tool.m_boxes)
		{
			auto br = b0.intersection_with(b1);
			if (br)
				{ inters.insert(br); }
		}
	}

	return Region { std::vector<Box> { inters.begin(), inters.end() } };
}


Region Region::cut_with(Region const & tool) const
{
	// Build a list of boxes, each the intersection of each of self and tool's boxes.
	// Then simplify.
	
	std::set<Box> inters;
	for (auto & b0 : m_boxes)
	{
		for (auto & b1 : tool.m_boxes)
		{
			auto local_inters = b0.cut_with(b1);
			for (auto & br: local_inters)
			{
				if (br)
					{ inters.insert(br); }
			}
		}
	}

	return Region { std::vector<Box> { inters.begin(), inters.end() } };
}


Region Region::union_with(Region const & tool) const
{
	// Build a list of boxes, each the intersection of each of self and tool's boxes.
	// Then simplify.
	
	std::set<Box> inters;
	for (auto & b0 : m_boxes)
	{
		for (auto & b1 : tool.m_boxes)
		{
			auto local_inters = b0.union_with(b1);
			for (auto & br: local_inters)
			{
				if (br)
					{ inters.insert(br); }
			}
		}
	}

	return Region { std::vector<Box> { inters.begin(), inters.end() } };
}


}
