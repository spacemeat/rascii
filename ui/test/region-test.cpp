#include <iostream>
#include "doctest.h"
#include "region.hpp"


using namespace rascii;

TEST_CASE("Region intersect")
{
	Region ra { std::vector<Box> { Box { Pos { -6, -8 }, Size { 5, 4 } },
								   Box { Pos { -6, -4 }, Size { 13, 5 } },
								   Box { Pos { 2, 1 }, Size { 5, 5 } } } };
	Region rb { std::vector<Box> { Box { Pos { -3, -6 }, Size { 8, 2 } },
								   Box { Pos { -8, -4 }, Size { 13, 1 } },
								   Box { Pos { -8, -3 }, Size { 17, 2 } },
								   Box { Pos { -3, -1 }, Size { 12, 3 } },
								   Box { Pos { -3, 2 }, Size { 8, 2 } } } };

	SUBCASE("intersect ra & rb")
	{
		auto r2 = ra.intersection_with(rb);
		CHECK(r2.m_boxes.size() == 6);
		CHECK(r2.m_boxes[0], Box { Pos { -3, -6 }, Size { 2, 2 } } );
		CHECK(r2.m_boxes[1], Box { Pos { -6, -4 }, Size { 11, 1 } } );
		CHECK(r2.m_boxes[2], Box { Pos { -6, -3 }, Size { 13, 2 } } );
		CHECK(r2.m_boxes[3], Box { Pos { -3, -1 }, Size { 10, 2 } } );
		CHECK(r2.m_boxes[4], Box { Pos { 2, 1 }, Size { 5, 1 } } );
		CHECK(r2.m_boxes[5], Box { Pos { 2, 2 }, Size { 3, 2 } } );
	}

	SUBCASE("intersect rb & ra")
	{
		auto r2 = rb.intersection_with(ra);
		CHECK(r2.m_boxes.size() == 6);
		CHECK(r2.m_boxes[0], Box { Pos { -3, -6 }, Size { 2, 2 } } );
		CHECK(r2.m_boxes[1], Box { Pos { -6, -4 }, Size { 11, 1 } } );
		CHECK(r2.m_boxes[2], Box { Pos { -6, -3 }, Size { 13, 2 } } );
		CHECK(r2.m_boxes[3], Box { Pos { -3, -1 }, Size { 10, 2 } } );
		CHECK(r2.m_boxes[4], Box { Pos { 2, 1 }, Size { 5, 1 } } );
		CHECK(r2.m_boxes[5], Box { Pos { 2, 2 }, Size { 3, 2 } } );
	}

	SUBCASE("intersect ra & ra")
	{
		auto r2 = ra.intersection_with(ra);
		CHECK(r2 == ra);
	}

	SUBCASE("intersect rb & rb")
	{
		auto r2 = rb.intersection_with(rb);
		CHECK(r2 == rb);
	}
}

TEST_CASE("Region cut")
{
	Region ra { std::vector<Box> { Box { Pos { -6, -8 }, Size { 5, 4 } },
								   Box { Pos { -6, -4 }, Size { 13, 5 } },
								   Box { Pos { 2, 1 }, Size { 5, 5 } } } };
	Region rb { std::vector<Box> { Box { Pos { -3, -6 }, Size { 8, 2 } },
								   Box { Pos { -8, -4 }, Size { 13, 1 } },
								   Box { Pos { -8, -3 }, Size { 17, 2 } },
								   Box { Pos { -3, -1 }, Size { 12, 3 } },
								   Box { Pos { -3, 2 }, Size { 8, 2 } } } };

	SUBCASE("cut ra & rb")
	{
		auto r2 = ra.cut_with(rb);
		CHECK(r2.m_boxes.size() == 6);
		CHECK(r2.m_boxes[0], Box { Pos { -6, -8 }, Size { 5, 2 } } );
		CHECK(r2.m_boxes[1], Box { Pos { -6, -6 }, Size { 3, 2 } } );
		CHECK(r2.m_boxes[2], Box { Pos { 5, -4 }, Size { 2, 1 } } );
		CHECK(r2.m_boxes[3], Box { Pos { -6, -1 }, Size { 3, 2 } } );
		CHECK(r2.m_boxes[4], Box { Pos { 5, 2 }, Size { 2, 2 } } );
		CHECK(r2.m_boxes[5], Box { Pos { 2, 4 }, Size { 5, 2 } } );
	}

	SUBCASE("cut rb & ra")
	{
		auto r2 = rb.cut_with(ra);
		CHECK(r2.m_boxes.size() == 4);
		CHECK(r2.m_boxes[0], Box { Pos { -1, -6 }, Size { 6, 2 } } );
		CHECK(r2.m_boxes[1], Box { Pos { -8, -4 }, Size { 2, 3 } } );
		CHECK(r2.m_boxes[2], Box { Pos { 7, -3 }, Size { 2, 5 } } );
		CHECK(r2.m_boxes[3], Box { Pos { -3, 1 }, Size { 5, 3 } } );
	}

	SUBCASE("cut ra & ra")
	{
		auto r2 = ra.cut_with(ra);
		CHECK(r2.m_boxes.size() == 0);
	}

	SUBCASE("cut rb & rb")
	{
		auto r2 = rb.cut_with(rb);
		CHECK(r2.m_boxes.size() == 0);
	}
}

TEST_CASE("Region union")
{
	Region ra { std::vector<Box> { Box { Pos { -6, -8 }, Size { 5, 4 } },
								   Box { Pos { -6, -4 }, Size { 13, 5 } },
								   Box { Pos { 2, 1 }, Size { 5, 5 } } } };
	Region rb { std::vector<Box> { Box { Pos { -3, -6 }, Size { 8, 2 } },
								   Box { Pos { -8, -4 }, Size { 13, 1 } },
								   Box { Pos { -8, -3 }, Size { 17, 2 } },
								   Box { Pos { -3, -1 }, Size { 12, 3 } },
								   Box { Pos { -3, 2 }, Size { 8, 2 } } } };

	SUBCASE("union ra & rb")
	{
		auto r2 = ra.union_with(rb);
		CHECK(r2.m_boxes.size() == 8);
		CHECK(r2.m_boxes[0], Box { Pos { -6, -8 }, Size { 5, 2 } } );
		CHECK(r2.m_boxes[1], Box { Pos { -6, -6 }, Size { 11, 2 } } );
		CHECK(r2.m_boxes[2], Box { Pos { -8, -4 }, Size { 15, 1 } } );
		CHECK(r2.m_boxes[3], Box { Pos { -8, -3 }, Size { 17, 2 } } );
		CHECK(r2.m_boxes[4], Box { Pos { -6, -1 }, Size { 15, 2 } } );
		CHECK(r2.m_boxes[5], Box { Pos { -3, 1 }, Size { 12, 1 } } );
		CHECK(r2.m_boxes[6], Box { Pos { -3, 2 }, Size { 10, 2 } } );
		CHECK(r2.m_boxes[7], Box { Pos { 2, 4 }, Size { 5, 2 } } );
	}

	SUBCASE("union rb & ra")
	{
		auto r2 = rb.union_with(ra);
		CHECK(r2.m_boxes.size() == 8);
		CHECK(r2.m_boxes[0], Box { Pos { -6, -8 }, Size { 5, 2 } } );
		CHECK(r2.m_boxes[1], Box { Pos { -6, -6 }, Size { 11, 2 } } );
		CHECK(r2.m_boxes[2], Box { Pos { -8, -4 }, Size { 15, 1 } } );
		CHECK(r2.m_boxes[3], Box { Pos { -8, -3 }, Size { 17, 2 } } );
		CHECK(r2.m_boxes[4], Box { Pos { -6, -1 }, Size { 15, 2 } } );
		CHECK(r2.m_boxes[5], Box { Pos { -3, 1 }, Size { 12, 1 } } );
		CHECK(r2.m_boxes[6], Box { Pos { -3, 2 }, Size { 10, 2 } } );
		CHECK(r2.m_boxes[7], Box { Pos { 2, 4 }, Size { 5, 2 } } );
	}

	SUBCASE("union ra & ra")
	{
		auto r2 = ra.union_with(ra);
		CHECK(r2 == ra);
	}

	SUBCASE("union rb & rb")
	{
		auto r2 = rb.union_with(rb);
		CHECK(r2 == rb);
	}
}
