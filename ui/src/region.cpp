#include <set>
#include "region.hpp"

bool const DEBUG_OUT = false;

namespace rascii
{

Region::Region(std::vector<Box> rhs)
: m_boxes(normalize_boxes(rhs))
{
	if (DEBUG_OUT)
	{
		std::cout << "new region:\n";
		for (auto & b0 : m_boxes)
		{
			std::cout << "b: " << b0 << "\n";
		}
	}
}


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


Region Region::xor_with(Region const & tool) const
{
	// Build a list of boxes, each the intersection of each of self and tool's boxes.
	// Then xor each box from self and tool against each other; if the resulting boxes
	// do not intersect any of the intersection boxes, they are part of the set.
	
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

	std::set<Box> xors;
	for (auto & b0 : m_boxes)
	{
		for (auto & b1 : tool.m_boxes)
		{
			for (auto & b2 : b0.xor_with(b1))
			{
				bool scrub = false;
				for (auto & b3 : inters)
				{
					if (b2.intersection_with(b3))
						{ scrub = true; break; }
				}

				if (! scrub)
					{ xors.insert(b2); }
			}
		}
	}

	return Region { std::vector<Box> { xors.begin(), xors.end() } };
}


Region Region::cut_with(Region const & tool) const
{
	// Build a list of boxes, each the cut of each of self and tool's boxes.
	// Then remove the boxes that intersect any of cut's boxes.
	std::vector<Box> inters;
	for (auto & b0 : m_boxes)
	{
		for (auto & b1 : tool.m_boxes)
		{
			auto local_inters = b0.cut_with(b1);
			for (auto & br: local_inters)
				{ inters.push_back(br); }
		}
	}

	std::vector<Box> rem;

	for (auto const & ae: inters)
	{
		bool incl = true;
		for (auto const & be: tool.m_boxes)
		{
			if (ae.intersection_with(be))
				{ incl = false; }
		}

		if (incl)
			{ rem.push_back(ae); }
	}

	return Region { rem };
}


Region Region::union_with(Region const & tool) const
{
	// Build a list of boxes, each the intersection of each of self and tool's boxes.
	// Then xor each box from self and tool against each other; if the resulting boxes
	// do not intersect any of the intersection boxes, they are part of the set. Then
	// add the intersection list to the set as well.
	
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

	std::set<Box> xors;
	for (auto & b0 : m_boxes)
	{
		for (auto & b1 : tool.m_boxes)
		{
			for (auto & b2 : b0.xor_with(b1))
			{
				bool scrub = false;
				for (auto & b3 : inters)
				{
					if (b2.intersection_with(b3))
						{ scrub = true; break; }
				}

				if (! scrub)
					{ xors.insert(b2); }
			}
		}
	}

	
	inters.insert(xors.begin(), xors.end());

	return Region { std::vector<Box> { inters.begin(), inters.end() } };
}


}
