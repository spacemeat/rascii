#include <array>
#include <iostream>
#include "doctest.h"
#include "region.hpp"


using namespace rascii;


TEST_CASE("two box merge")
{
	SUBCASE("merge top-bottom")
	{
		Box a { Pos { -3, -2 }, Size { 6, 2 } };
		Box b { Pos { -3, 0 }, Size { 6, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 1);
		CHECK(*repl[0] == Box { Pos { -3, -2 }, Size { 6, 4 } });
	}
	SUBCASE("merge top-bottom reverse")
	{
		Box a { Pos { -3, 0 }, Size { 6, 2 } };
		Box b { Pos { -3, -2 }, Size { 6, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge non-adjacent nw-se")
	{
		Box a { Pos { -3, -2 }, Size { 3, 2 } };
		Box b { Pos { 0, 0 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge non-adjacent nw-se reverse")
	{
		Box a { Pos { 0, 0 }, Size { 3, 2 } };
		Box b { Pos { -3, -2 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge non-adjacent ne-sw")
	{
		Box a { Pos { 0, -2 }, Size { 3, 2 } };
		Box b { Pos { -3, 0 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge non-adjacent ne-sw reverse")
	{
		Box a { Pos { -3, 0 }, Size { 3, 2 } };
		Box b { Pos { 0, -2 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge uu")
	{
		Box a { Pos { -3, -2 }, Size { 3, 2 } };
		Box b { Pos { 0, -1 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 3);
		CHECK(*repl[0] == Box { Pos { -3, -2 }, Size { 3, 1 } });
		CHECK(*repl[1] == Box { Pos { -3, -1 }, Size { 6, 1 } });
		CHECK(*repl[2] == Box { Pos { 0, 0 }, Size { 3, 1 } });
	}
	SUBCASE("merge uu reverse")
	{
		Box a { Pos { 0, -1 }, Size { 3, 2 } };
		Box b { Pos { -3, -2 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge ue")
	{
		Box a { Pos { -3, -2 }, Size { 3, 4 } };
		Box b { Pos { 0, 0 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 2);
		CHECK(*repl[0] == Box { Pos { -3, -2 }, Size { 3, 2 } });
		CHECK(*repl[1] == Box { Pos { -3, 0 }, Size { 6, 2 } });
	}
	SUBCASE("merge ue reverse")
	{
		Box a { Pos { 0, 0 }, Size { 3, 2 } };
		Box b { Pos { -3, -2 }, Size { 3, 4 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge ud")
	{
		Box a { Pos { -3, -2 }, Size { 3, 4 } };
		Box b { Pos { 0, -1 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 3);
		CHECK(*repl[0] == Box { Pos { -3, -2 }, Size { 3, 1 } });
		CHECK(*repl[1] == Box { Pos { -3, -1 }, Size { 6, 2 } });
		CHECK(*repl[2] == Box { Pos { -3, 1 }, Size { 3, 1 } });
	}
	SUBCASE("merge ud reverse")
	{
		Box a { Pos { 0, -1 }, Size { 3, 2 } };
		Box b { Pos { -3, -2 }, Size { 3, 4 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge eu")
	{
		Box a { Pos { -3, -2 }, Size { 3, 2 } };
		Box b { Pos { 0, -2 }, Size { 3, 4 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 2);
		CHECK(*repl[0] == Box { Pos { -3, -2 }, Size { 6, 2 } });
		CHECK(*repl[1] == Box { Pos { 0, 0 }, Size { 3, 2 } });
	}
	SUBCASE("merge eu reverse")
	{
		Box a { Pos { 0, -2 }, Size { 3, 4 } };
		Box b { Pos { -3, -2 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge ee")
	{
		Box a { Pos { -3, -2 }, Size { 3, 4 } };
		Box b { Pos { 0, -2 }, Size { 3, 4 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 1);
		CHECK(*repl[0] == Box { Pos { -3, -2 }, Size { 6, 4 } });
	}
	SUBCASE("merge ee reverse")
	{
		Box a { Pos { 0, -2 }, Size { 3, 4 } };
		Box b { Pos { -3, -2 }, Size { 3, 4 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge ed")
	{
		Box a { Pos { -3, -2 }, Size { 3, 4 } };
		Box b { Pos { 0, -2 }, Size { 3, 2 } };
	
		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 2);
		CHECK(*repl[0] == Box { Pos { -3, -2 }, Size { 6, 2 } });
		CHECK(*repl[1] == Box { Pos { -3, 0 }, Size { 3, 2 } });
	}
	SUBCASE("merge ed reverse")
	{
		Box a { Pos { 0, -2 }, Size { 3, 2 } };
		Box b { Pos { -3, -2 }, Size { 3, 4 } };
	
		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge du")
	{
		Box a { Pos { -3, -1 }, Size { 3, 2 } };
		Box b { Pos { 0, -2 }, Size { 3, 4 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge du reverse")
	{
		Box a { Pos { 0, -2 }, Size { 3, 4 } };
		Box b { Pos { -3, -1 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 3);
		CHECK(*repl[0] == Box { Pos { 0, -2 }, Size { 3, 1 } });
		CHECK(*repl[1] == Box { Pos { -3, -1 }, Size { 6, 2 } });
		CHECK(*repl[2] == Box { Pos { 0, 1 }, Size { 3, 1 } });
	}
	SUBCASE("merge de")
	{
		Box a { Pos { -3, 0 }, Size { 3, 2 } };
		Box b { Pos { 0, -2 }, Size { 3, 4 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge de reverse")
	{
		Box a { Pos { 0, -2 }, Size { 3, 4 } };
		Box b { Pos { -3, 0 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 2);
		CHECK(*repl[0] == Box { Pos { 0, -2 }, Size { 3, 2 } });
		CHECK(*repl[1] == Box { Pos { -3, 0 }, Size { 6, 2 } });
	}
	SUBCASE("merge dd")
	{
		Box a { Pos { -3, -1 }, Size { 3, 2 } };
		Box b { Pos { 0, -2 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 0);
	}
	SUBCASE("merge dd reverse")
	{
		Box a { Pos { 0, -2 }, Size { 3, 2 } };
		Box b { Pos { -3, -1 }, Size { 3, 2 } };

		std::array<std::optional<Box>, 3> repl;
		auto nb = merge_boxes_if_adjacent(a, b, repl);
		CHECK(nb == 3);
		CHECK(*repl[0] == Box { Pos { 0, -2 }, Size { 3, 1 } });
		CHECK(*repl[1] == Box { Pos { -3, -1 }, Size { 6, 1 } });
		CHECK(*repl[2] == Box { Pos { -3, 0 }, Size { 3, 1 } });
	}
}


TEST_CASE("is_boxes_sorted")
{
	Box b1 { Pos { -3, -2 }, Size { 3, 2 } };
	Box b2 { Pos { 0, -2 }, Size { 3, 4 } };
	Box b3 { Pos { 3, -2 }, Size { 3, 2 } };
	Box b4 { Pos { -3, 0 }, Size { 3, 2 } };
	Box b5 { Pos { 3, 0 }, Size { 3, 2 } };
	std::vector<Box> sorted { b1, b2, b3, b4, b5 };

	SUBCASE("sort 12345")
	{
		std::vector<Box> v { b1, b2, b3, b4, b5 };
		[[maybe_unused]] auto nb = is_boxes_sorted(v);
		CHECK(nb == true);
	}
	SUBCASE("sort 21345")
	{
		std::vector<Box> v { b2, b1, b3, b4, b5 };
		[[maybe_unused]] auto nb = is_boxes_sorted(v);
		CHECK(nb == false);
	}
	SUBCASE("sort 21435")
	{
		std::vector<Box> v { b2, b1, b4, b3, b5 };
		[[maybe_unused]] auto nb = is_boxes_sorted(v);
		CHECK(nb == false);
	}
	SUBCASE("sort 54321")
	{
		std::vector<Box> v { b5, b4, b3, b2, b1 };
		[[maybe_unused]] auto nb = is_boxes_sorted(v);
		CHECK(nb == false);
	}
	SUBCASE("sort 12354")
	{
		std::vector<Box> v { b1, b2, b3, b5, b4 };
		[[maybe_unused]] auto nb = is_boxes_sorted(v);
		CHECK(nb == false);
	}
}

TEST_CASE("sort vector of boxes")
{
	Box b1 { Pos { -3, -2 }, Size { 3, 2 } };
	Box b2 { Pos { 0, -2 }, Size { 3, 4 } };
	Box b3 { Pos { 3, -2 }, Size { 3, 2 } };
	Box b4 { Pos { -3, 0 }, Size { 3, 2 } };
	Box b5 { Pos { 3, 0 }, Size { 3, 2 } };
	std::vector<Box> sorted { b1, b2, b3, b4, b5 };

	SUBCASE("sort 12345")
	{
		std::vector<Box> v { b1, b2, b3, b4, b5 };
		sort_boxes(v);
		CHECK(v == sorted);
	}
	SUBCASE("sort 21345")
	{
		std::vector<Box> v { b2, b1, b3, b4, b5 };
		sort_boxes(v);
		CHECK(v == sorted);
	}
	SUBCASE("sort 21435")
	{
		std::vector<Box> v { b2, b1, b4, b3, b5 };
		sort_boxes(v);
		CHECK(v == sorted);
	}
	SUBCASE("sort 54321")
	{
		std::vector<Box> v { b5, b4, b3, b2, b1 };
		sort_boxes(v);
		CHECK(v == sorted);
	}
}


TEST_CASE("normalize_vector")
{
	SUBCASE("normalize top-bottom")
	{
		std::vector<Box> v {
			Box { Pos { -3, -2 }, Size { 6, 2 } },
			Box { Pos { -3, 0 }, Size { 6, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 1);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 6, 4 } });
	}
	SUBCASE("normalize top-bottom reverse")
	{
		std::vector<Box> v {
			Box { Pos { -3, 0 }, Size { 6, 2 } },
			Box { Pos { -3, -2 }, Size { 6, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 1);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 6, 4 } });
	}
	SUBCASE("normalize non-adjacent nw-se")
	{
		std::vector<Box> v {
			Box { Pos { -3, -2 }, Size { 3, 2 } },
			Box { Pos { 0, 0 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == v[0]);
		CHECK(nb[1] == v[1]);
	}
	SUBCASE("normalize non-adjacent nw-se reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, 0 }, Size { 3, 2 } },
			Box { Pos { -3, -2 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb[0] == v[1]);
		CHECK(nb[1] == v[0]);
	}
	SUBCASE("normalize non-adjacent ne-sw")
	{
		std::vector<Box> v {
			Box { Pos { 0, -2 }, Size { 3, 2 } },
			Box { Pos { -3, 0 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == v[0]);
		CHECK(nb[1] == v[1]);
	}
	SUBCASE("normalize non-adjacent ne-sw reverse")
	{
		std::vector<Box> v {
			Box { Pos { -3, 0 }, Size { 3, 2 } },
			Box { Pos { 0, -2 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == v[1]);
		CHECK(nb[1] == v[0]);
	}
	SUBCASE("normalize uu")
	{
		std::vector<Box> v {
			Box { Pos { -3, -2 }, Size { 3, 2 } },
			Box { Pos { 0, -1 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 3);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 3, 1 } });
		CHECK(nb[1] == Box { Pos { -3, -1 }, Size { 6, 1 } });
		CHECK(nb[2] == Box { Pos { 0, 0 }, Size { 3, 1 } });
	}
	SUBCASE("normalize uu reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, -1 }, Size { 3, 2 } },
			Box { Pos { -3, -2 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 3);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 3, 1 } });
		CHECK(nb[1] == Box { Pos { -3, -1 }, Size { 6, 1 } });
		CHECK(nb[2] == Box { Pos { 0, 0 }, Size { 3, 1 } });
	}
	SUBCASE("normalize ue")
	{
		std::vector<Box> v {
			Box { Pos { -3, -2 }, Size { 3, 4 } },
			Box { Pos { 0, 0 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 3, 2 } });
		CHECK(nb[1] == Box { Pos { -3, 0 }, Size { 6, 2 } });
	}
	SUBCASE("normalize ue reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, 0 }, Size { 3, 2 } },
			Box { Pos { -3, -2 }, Size { 3, 4 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 3, 2 } });
		CHECK(nb[1] == Box { Pos { -3, 0 }, Size { 6, 2 } });
	}
	SUBCASE("normalize ud")
	{
		std::vector<Box> v {
			Box { Pos { -3, -2 }, Size { 3, 4 } },
			Box { Pos { 0, -1 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 3);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 3, 1 } });
		CHECK(nb[1] == Box { Pos { -3, -1 }, Size { 6, 2 } });
		CHECK(nb[2] == Box { Pos { -3, 1 }, Size { 3, 1 } });
	}
	SUBCASE("normalize ud reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, -1 }, Size { 3, 2 } },
			Box { Pos { -3, -2 }, Size { 3, 4 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 3);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 3, 1 } });
		CHECK(nb[1] == Box { Pos { -3, -1 }, Size { 6, 2 } });
		CHECK(nb[2] == Box { Pos { -3, 1 }, Size { 3, 1 } });
	}
	SUBCASE("normalize eu")
	{
		std::vector<Box> v {
			Box { Pos { -3, -2 }, Size { 3, 2 } },
			Box { Pos { 0, -2 }, Size { 3, 4 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 6, 2 } });
		CHECK(nb[1] == Box { Pos { 0, 0 }, Size { 3, 2 } });
	}
	SUBCASE("normalize eu reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, -2 }, Size { 3, 4 } },
			Box { Pos { -3, -2 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 6, 2 } });
		CHECK(nb[1] == Box { Pos { 0, 0 }, Size { 3, 2 } });
	}
	SUBCASE("normalize ee")
	{
		std::vector<Box> v {
			Box { Pos { -3, -2 }, Size { 3, 4 } },
			Box { Pos { 0, -2 }, Size { 3, 4 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 1);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 6, 4 } });
	}
	SUBCASE("normalize ee reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, -2 }, Size { 3, 4 } },
			Box { Pos { -3, -2 }, Size { 3, 4 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 1);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 6, 4 } });
	}
	SUBCASE("normalize ed")
	{
		std::vector<Box> v {
			Box { Pos { -3, -2 }, Size { 3, 4 } },
			Box { Pos { 0, -2 }, Size { 3, 2 } }
		};
	
		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 6, 2 } });
		CHECK(nb[1] == Box { Pos { -3, 0 }, Size { 3, 2 } });
	}
	SUBCASE("normalize ed reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, -2 }, Size { 3, 2 } },
			Box { Pos { -3, -2 }, Size { 3, 4 } }
		};
	
		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 6, 2 } });
		CHECK(nb[1] == Box { Pos { -3, 0 }, Size { 3, 2 } });
	}
	SUBCASE("normalize du")
	{
		std::vector<Box> v {
			Box { Pos { -3, -1 }, Size { 3, 2 } },
			Box { Pos { 0, -2 }, Size { 3, 4 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 3);
		CHECK(nb[0] == Box { Pos { 0, -2 }, Size { 3, 1 } });
		CHECK(nb[1] == Box { Pos { -3, -1 }, Size { 6, 2 } });
		CHECK(nb[2] == Box { Pos { 0, 1 }, Size { 3, 1 } });
	}
	SUBCASE("normalize du reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, -2 }, Size { 3, 4 } },
			Box { Pos { -3, -1 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 3);
		CHECK(nb[0] == Box { Pos { 0, -2 }, Size { 3, 1 } });
		CHECK(nb[1] == Box { Pos { -3, -1 }, Size { 6, 2 } });
		CHECK(nb[2] == Box { Pos { 0, 1 }, Size { 3, 1 } });
	}
	SUBCASE("normalize de")
	{
		std::vector<Box> v {
			Box { Pos { -3, 0 }, Size { 3, 2 } },
			Box { Pos { 0, -2 }, Size { 3, 4 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == Box { Pos { 0, -2 }, Size { 3, 2 } });
		CHECK(nb[1] == Box { Pos { -3, 0 }, Size { 6, 2 } });
	}
	SUBCASE("normalize de reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, -2 }, Size { 3, 4 } },
			Box { Pos { -3, 0 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 2);
		CHECK(nb[0] == Box { Pos { 0, -2 }, Size { 3, 2 } });
		CHECK(nb[1] == Box { Pos { -3, 0 }, Size { 6, 2 } });
	}
	SUBCASE("normalize dd")
	{
		std::vector<Box> v {
			Box { Pos { -3, -1 }, Size { 3, 2 } },
			Box { Pos { 0, -2 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 3);
		CHECK(nb[0] == Box { Pos { 0, -2 }, Size { 3, 1 } });
		CHECK(nb[1] == Box { Pos { -3, -1 }, Size { 6, 1 } });
		CHECK(nb[2] == Box { Pos { -3, 0 }, Size { 3, 1 } });
	}
	SUBCASE("normalize dd reverse")
	{
		std::vector<Box> v {
			Box { Pos { 0, -2 }, Size { 3, 2 } },
			Box { Pos { -3, -1 }, Size { 3, 2 } }
		};

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 3);
		CHECK(nb[0] == Box { Pos { 0, -2 }, Size { 3, 1 } });
		CHECK(nb[1] == Box { Pos { -3, -1 }, Size { 6, 1 } });
		CHECK(nb[2] == Box { Pos { -3, 0 }, Size { 3, 1 } });
	}
	SUBCASE("normalize rect grid")
	{
		Box b1 { Pos { -3, -2 }, Size { 3, 2 } };
		Box b2 { Pos { 0, -2 }, Size { 3, 4 } };
		Box b3 { Pos { 3, -2 }, Size { 3, 2 } };
		Box b4 { Pos { -3, 0 }, Size { 3, 2 } };
		Box b5 { Pos { 3, 0 }, Size { 3, 2 } };
		std::vector<Box> v { b1, b2, b3, b4, b5 };

		auto nb = normalize_boxes(v);
		CHECK(nb.size() == 1);
		CHECK(nb[0] == Box { Pos { -3, -2 }, Size { 9, 4 } });
	}
}

