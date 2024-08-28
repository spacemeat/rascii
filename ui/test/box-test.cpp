#include <iostream>
#include "doctest.h"
#include "region.hpp"


using namespace rascii;

TEST_CASE("Box intersect")
{
	Box bx { Pos { -5, -5 }, Size { 11, 11 } };
	Box bz {};

	Box n0 { Pos { 2, 2 }, Size { 0, 0 } };
	Box n1 { Pos { 2, 2 }, Size { 1, 0 } };
	Box n2 { Pos { 2, 2 }, Size { 0, 1 } };
	Box ul { Pos { -8, -8 }, Size { 5, 5 } };
	Box um { Pos { -3, -8 }, Size { 7, 5 } };
	Box ur { Pos {  4, -8 }, Size { 5, 5 } };
	Box uc { Pos { -8, -8 }, Size { 17, 5 } };
	Box ml { Pos { -8, -3 }, Size { 5, 7 } };
	Box mm { Pos { -3, -3 }, Size { 7, 7 } };
	Box mr { Pos {  4, -3 }, Size { 5, 7 } };
	Box hc { Pos { -8, -3 }, Size { 17, 7 } };
	Box ll { Pos { -8,  4 }, Size { 5, 5 } };
	Box lm { Pos { -3,  4 }, Size { 7, 5 } };
	Box lr { Pos {  4,  4 }, Size { 5, 5 } };
	Box loc { Pos { -8, 4 }, Size { 17, 5 } };
	Box lc { Pos { -8, -8 }, Size { 5, 17 } };
	Box vc { Pos { -3, -8 }, Size { 7, 17 } };
	Box rc { Pos { 4, -8 }, Size { 5, 17 } };
	Box fu { Pos { -8, -8 }, Size { 17, 17 } };

	SUBCASE("intersect bx & n0")
	{
		auto b2 = bx.intersection_with(n0);
		CHECK(b2 == bz);
	}

	SUBCASE("intersect n0 & bx")
	{
		auto b2 = n0.intersection_with(bx);
		CHECK(b2 == bz);
	}

	SUBCASE("intersect bx & n1")
	{
		auto b2 = bx.intersection_with(n1);
		CHECK(b2 == bz);
	}

	SUBCASE("intersect n1 & bx")
	{
		auto b2 = n1.intersection_with(bx);
		CHECK(b2 == bz);
	}

	SUBCASE("intersect bx & n2")
	{
		auto b2 = bx.intersection_with(n2);
		CHECK(b2 == bz);
	}

	SUBCASE("intersect n2 & bx")
	{
		auto b2 = n2.intersection_with(bx);
		CHECK(b2 == bz);
	}

	SUBCASE("intersect bx & ul")
	{
		auto b2 = bx.intersection_with(ul);
		CHECK(b2 == Box { Pos { -5, -5 }, Size { 2, 2 } });
	}

	SUBCASE("intersect ul & bx")
	{
		auto b2 = ul.intersection_with(bx);
		CHECK(b2 == Box { Pos { -5, -5 }, Size { 2, 2 } });
	}

	SUBCASE("intersect bx & um")
	{
		auto b2 = bx.intersection_with(um);
		CHECK(b2 == Box { Pos { -3, -5 }, Size { 7, 2 } });
	}

	SUBCASE("intersect um & bx")
	{
		auto b2 = um.intersection_with(bx);
		CHECK(b2 == Box { Pos { -3, -5 }, Size { 7, 2 } });
	}

	SUBCASE("intersect bx & ur")
	{
		auto b2 = bx.intersection_with(ur);
		CHECK(b2 == Box { Pos { 4, -5 }, Size { 2, 2 } });
	}

	SUBCASE("intersect ur & bx")
	{
		auto b2 = ur.intersection_with(bx);
		CHECK(b2 == Box { Pos { 4, -5 }, Size { 2, 2 } });
	}

	SUBCASE("intersect bx & uc")
	{
		auto b2 = bx.intersection_with(uc);
		CHECK(b2 == Box { Pos { -5, -5 }, Size { 11, 2 } });
	}
	
	SUBCASE("intersect uc & bx")
	{
		auto b2 = uc.intersection_with(bx);
		CHECK(b2 == Box { Pos { -5, -5 }, Size { 11, 2 } });
	}

	SUBCASE("intersect bx & ml")
	{
		auto b2 = bx.intersection_with(ml);
		CHECK(b2 == Box { Pos { -5, -3 }, Size { 2, 7 } });
	}

	SUBCASE("intersect ml & bx")
	{
		auto b2 = ml.intersection_with(bx);
		CHECK(b2 == Box { Pos { -5, -3 }, Size { 2, 7 } });
	}

	SUBCASE("intersect bx & mm")
	{
		auto b2 = bx.intersection_with(mm);
		CHECK(b2 == Box { Pos { -3, -3 }, Size { 7, 7 } });
	}

	SUBCASE("intersect mm & bx")
	{
		auto b2 = mm.intersection_with(bx);
		CHECK(b2 == Box { Pos { -3, -3 }, Size { 7, 7 } });
	}

	SUBCASE("intersect bx & mr")
	{
		auto b2 = bx.intersection_with(mr);
		CHECK(b2 == Box { Pos { 4, -3 }, Size { 2, 7 } });
	}

	SUBCASE("intersect mr & bx")
	{
		auto b2 = mr.intersection_with(bx);
		CHECK(b2 == Box { Pos { 4, -3 }, Size { 2, 7 } });
	}

	SUBCASE("intersect bx & hc")
	{
		auto b2 = bx.intersection_with(hc);
		CHECK(b2 == Box { Pos { -5, -3 }, Size { 11, 7 } });
	}

	SUBCASE("intersect hc & bx")
	{
		auto b2 = hc.intersection_with(bx);
		CHECK(b2 == Box { Pos { -5, -3 }, Size { 11, 7 } });
	}

	SUBCASE("intersect bx & ll")
	{
		auto b2 = bx.intersection_with(ll);
		CHECK(b2 == Box { Pos { -5, 4 }, Size { 2, 2 } });
	}

	SUBCASE("intersect ll & bx")
	{
		auto b2 = ll.intersection_with(bx);
		CHECK(b2 == Box { Pos { -5, 4 }, Size { 2, 2 } });
	}

	SUBCASE("intersect bx & lm")
	{
		auto b2 = bx.intersection_with(lm);
		CHECK(b2 == Box { Pos { -3, 4 }, Size { 7, 2 } });
	}

	SUBCASE("intersect lm & bx")
	{
		auto b2 = lm.intersection_with(bx);
		CHECK(b2 == Box { Pos { -3, 4 }, Size { 7, 2 } });
	}

	SUBCASE("intersect bx & lr")
	{
		auto b2 = bx.intersection_with(lr);
		CHECK(b2 == Box { Pos { 4, 4 }, Size { 2, 2 } });
	}

	SUBCASE("intersect lr & bx")
	{
		auto b2 = lr.intersection_with(bx);
		CHECK(b2 == Box { Pos { 4, 4 }, Size { 2, 2 } });
	}

	SUBCASE("intersect bx & loc")
	{
		auto b2 = bx.intersection_with(loc);
		CHECK(b2 == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}
	
	SUBCASE("intersect loc & bx")
	{
		auto b2 = loc.intersection_with(bx);
		CHECK(b2 == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("intersect bx & lc")
	{
		auto b2 = bx.intersection_with(lc);
		CHECK(b2 == Box { Pos { -5, -5 }, Size { 2, 11 } });
	}
	
	SUBCASE("intersect lc & bx")
	{
		auto b2 = lc.intersection_with(bx);
		CHECK(b2 == Box { Pos { -5, -5 }, Size { 2, 11 } });
	}

	SUBCASE("intersect bx & vc")
	{
		auto b2 = bx.intersection_with(vc);
		CHECK(b2 == Box { Pos { -3, -5 }, Size { 7, 11 } });
	}

	SUBCASE("intersect vc & bx")
	{
		auto b2 = vc.intersection_with(bx);
		CHECK(b2 == Box { Pos { -3, -5 }, Size { 7, 11 } });
	}

	SUBCASE("intersect bx & rc")
	{
		auto b2 = bx.intersection_with(rc);
		CHECK(b2 == Box { Pos { 4, -5 }, Size { 2, 11 } });
	}

	SUBCASE("intersect rc & bx")
	{
		auto b2 = rc.intersection_with(bx);
		CHECK(b2 == Box { Pos { 4, -5 }, Size { 2, 11 } });
	}

	SUBCASE("intersect bx & fu")
	{
		auto b2 = bx.intersection_with(fu);
		CHECK(b2 == Box { Pos { -5, -5 }, Size { 11, 11 } });
	}

	SUBCASE("intersect fu & bx")
	{
		auto b2 = fu.intersection_with(bx);
		CHECK(b2 == Box { Pos { -5, -5 }, Size { 11, 11 } });
	}

	// non-overlapping boxen
	SUBCASE("intersect mm & ul") { CHECK(mm.intersection_with(ul) == bz); }
	SUBCASE("intersect ul & mm") { CHECK(ul.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & um") { CHECK(mm.intersection_with(um) == bz); }
	SUBCASE("intersect um & mm") { CHECK(um.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & ur") { CHECK(mm.intersection_with(ur) == bz); }
	SUBCASE("intersect ur & mm") { CHECK(ur.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & ml") { CHECK(mm.intersection_with(ml) == bz); }
	SUBCASE("intersect ml & mm") { CHECK(ml.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & mm") { CHECK(mm.intersection_with(mm) == mm); }
	SUBCASE("intersect mm & mr") { CHECK(mm.intersection_with(mr) == bz); }
	SUBCASE("intersect mr & mm") { CHECK(mr.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & hc") { CHECK(mm.intersection_with(hc) == mm); }
	SUBCASE("intersect hc & mm") { CHECK(hc.intersection_with(mm) == mm); }
	SUBCASE("intersect mm & ll") { CHECK(mm.intersection_with(ll) == bz); }
	SUBCASE("intersect ll & mm") { CHECK(ll.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & lc") { CHECK(mm.intersection_with(lc) == bz); }
	SUBCASE("intersect lc & mm") { CHECK(lc.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & lr") { CHECK(mm.intersection_with(lr) == bz); }
	SUBCASE("intersect lr & mm") { CHECK(lr.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & loc") { CHECK(mm.intersection_with(loc) == bz); }
	SUBCASE("intersect loc & mm") { CHECK(loc.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & lc") { CHECK(mm.intersection_with(lc) == bz); }
	SUBCASE("intersect lc & mm") { CHECK(lc.intersection_with(mm) == bz); }
	SUBCASE("intersect mm & vc") { CHECK(mm.intersection_with(vc) == mm); }
	SUBCASE("intersect vc & mm") { CHECK(vc.intersection_with(mm) == mm); }
	SUBCASE("intersect mm & rc") { CHECK(mm.intersection_with(rc) == bz); }
	SUBCASE("intersect rc & mm") { CHECK(rc.intersection_with(mm) == bz); }
}


TEST_CASE("Box cut")
{
	Box bx { Pos { -5, -5 }, Size { 11, 11 } };

	Box n0 { Pos { 2, 2 }, Size { 0, 0 } };
	Box n1 { Pos { 2, 2 }, Size { 1, 0 } };
	Box n2 { Pos { 2, 2 }, Size { 0, 1 } };
	Box ul { Pos { -8, -8 }, Size { 5, 5 } };
	Box um { Pos { -3, -8 }, Size { 7, 5 } };
	Box ur { Pos {  4, -8 }, Size { 5, 5 } };
	Box uc { Pos { -8, -8 }, Size { 17, 5 } };
	Box ml { Pos { -8, -3 }, Size { 5, 7 } };
	Box mm { Pos { -3, -3 }, Size { 7, 7 } };
	Box mr { Pos {  4, -3 }, Size { 5, 7 } };
	Box hc { Pos { -8, -3 }, Size { 17, 7 } };
	Box ll { Pos { -8,  4 }, Size { 5, 5 } };
	Box lm { Pos { -3,  4 }, Size { 7, 5 } };
	Box lr { Pos {  4,  4 }, Size { 5, 5 } };
	Box loc { Pos { -8, 4 }, Size { 17, 5 } };
	Box lc { Pos { -8, -8 }, Size { 5, 17 } };
	Box vc { Pos { -3, -8 }, Size { 7, 17 } };
	Box rc { Pos { 4, -8 }, Size { 5, 17 } };
	Box fu { Pos { -8, -8 }, Size { 17, 17 } };

	SUBCASE("cut bx & n0")
	{
		auto b2 = bx.cut_with(n0);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == bx);
	}

	SUBCASE("cut n0 & bx")
	{
		auto b2 = n0.cut_with(bx);
		CHECK(b2.size() == 0);
	}

	SUBCASE("cut bx & n1")
	{
		auto b2 = bx.cut_with(n1);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == bx);
	}

	SUBCASE("cut n1 & bx")
	{
		auto b2 = n1.cut_with(bx);
		CHECK(b2.size() == 0);
	}

	SUBCASE("cut bx & n2")
	{
		auto b2 = bx.cut_with(n2);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == bx);
	}

	SUBCASE("cut n2 & bx")
	{
		auto b2 = n2.cut_with(bx);
		CHECK(b2.size() == 0);
	}

	SUBCASE("cut bx & ul")
	{
		auto b2 = bx.cut_with(ul);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -3, -5 }, Size { 9, 2 } });
		CHECK(b2[1] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}

	SUBCASE("cut ul & bx")
	{
		auto b2 = ul.cut_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -8, -5 }, Size { 3, 2 } });
	}

	SUBCASE("cut bx & um")
	{
		auto b2 = bx.cut_with(um);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 2, 2 } });
		CHECK(b2[1] == Box { Pos { 4, -5 }, Size { 2, 2 } });
		CHECK(b2[2] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}

	SUBCASE("cut um & bx")
	{
		auto b2 = um.cut_with(bx);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == Box { Pos { -3, -8 }, Size { 7, 2 } });
	}

	SUBCASE("cut bx & ur")
	{
		auto b2 = bx.cut_with(ur);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 9, 2 } });
		CHECK(b2[1] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}

	SUBCASE("cut ur & bx")
	{
		auto b2 = ur.cut_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { 4, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { 6, -5 }, Size { 3, 2 } });
	}

	SUBCASE("cut bx & uc")
	{
		auto b2 = bx.cut_with(uc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}
	
	SUBCASE("cut uc & bx")
	{
		auto b2 = uc.cut_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 17, 3 } });
		CHECK(b2[1] == Box { Pos { -5, -3 }, Size { 3, 2 } });
		CHECK(b2[2] == Box { Pos { 6, -3 }, Size { 3, 2 } });
	}

	SUBCASE("cut bx & ml")
	{
		auto b2 = bx.cut_with(ml);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -3, -3 }, Size { 9, 7 } });
		CHECK(b2[2] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("cut ml & bx")
	{
		auto b2 = ml.cut_with(bx);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == Box { Pos { -8, -3 }, Size { 3, 7 } });
	}

	SUBCASE("cut bx & mm")
	{
		auto b2 = bx.cut_with(mm);
		CHECK(b2.size() == 4);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -5, -3 }, Size { 2, 7 } });
		CHECK(b2[2] == Box { Pos { 4, -3 }, Size { 2, 7 } });
		CHECK(b2[3] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("cut mm & bx")
	{
		auto b2 = mm.cut_with(bx);
		CHECK(b2.size() == 0);
	}

	SUBCASE("cut bx & mr")
	{
		auto b2 = bx.cut_with(mr);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -5, -3 }, Size { 9, 7 } });
		CHECK(b2[2] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("cut mr & bx")
	{
		auto b2 = mr.cut_with(bx);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == Box { Pos { 6, -3 }, Size { 3, 7 } });
	}

	SUBCASE("cut bx & hc")
	{
		auto b2 = bx.cut_with(hc);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("cut hc & bx")
	{
		auto b2 = hc.cut_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -8, -3 }, Size { 3, 7 } });
		CHECK(b2[1] == Box { Pos { 6, -3 }, Size { 3, 7 } });
	}

	SUBCASE("cut bx & ll")
	{
		auto b2 = bx.cut_with(ll);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
		CHECK(b2[1] == Box { Pos { -3, 4 }, Size { 9, 2 } });
	}

	SUBCASE("cut ll & bx")
	{
		auto b2 = ll.cut_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -8, 4 }, Size { 3, 2 } });
		CHECK(b2[1] == Box { Pos { -8, 6 }, Size { 5, 3 } });
	}

	SUBCASE("cut bx & lm")
	{
		auto b2 = bx.cut_with(lm);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
		CHECK(b2[1] == Box { Pos { -5, 4 }, Size { 2, 2 } });
		CHECK(b2[2] == Box { Pos { 4, 4 }, Size { 2, 2 } });
	}

	SUBCASE("cut lm & bx")
	{
		auto b2 = lm.cut_with(bx);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == Box { Pos { -3, 6 }, Size { 7, 3 } });
	}

	SUBCASE("cut bx & lr")
	{
		auto b2 = bx.cut_with(lr);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
		CHECK(b2[1] == Box { Pos { -5, 4 }, Size { 9, 2 } });
	}

	SUBCASE("cut lr & bx")
	{
		auto b2 = lr.cut_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { 6, 4 }, Size { 3, 2 } });
		CHECK(b2[1] == Box { Pos { 4, 6 }, Size { 5, 3 } });
	}

	SUBCASE("cut bx & loc")
	{
		auto b2 = bx.cut_with(loc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
	}
	
	SUBCASE("cut loc & bx")
	{
		auto b2 = loc.cut_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -8, 4 }, Size { 3, 2 } });
		CHECK(b2[1] == Box { Pos { 6, 4 }, Size { 3, 2 } });
		CHECK(b2[2] == Box { Pos { -8, 6 }, Size { 17, 3 } });
	}

	SUBCASE("cut bx & lc")
	{
		auto b2 = bx.cut_with(lc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == Box { Pos { -3, -5 }, Size { 9, 11 } });
	}
	
	SUBCASE("cut lc & bx")
	{
		auto b2 = lc.cut_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -8, -5 }, Size { 3, 11 } });
		CHECK(b2[2] == Box { Pos { -8, 6 }, Size { 5, 3 } });
	}

	SUBCASE("cut bx & vc")
	{
		auto b2 = bx.cut_with(vc);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 2, 11 } });
		CHECK(b2[1] == Box { Pos { 4, -5 }, Size { 2, 11 } });
	}

	SUBCASE("cut vc & bx")
	{
		auto b2 = vc.cut_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -3, -8 }, Size { 7, 3 } });
		CHECK(b2[1] == Box { Pos { -3, 6 }, Size { 7, 3 } });
	}

	SUBCASE("cut bx & rc")
	{
		auto b2 = bx.cut_with(rc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 9, 11 } });
	}

	SUBCASE("cut rc & bx")
	{
		auto b2 = rc.cut_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { 4, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { 6, -5 }, Size { 3, 11 } });
		CHECK(b2[2] == Box { Pos { 4, 6 }, Size { 5, 3 } });
	}

	SUBCASE("cut bx & fu")
	{
		auto b2 = bx.cut_with(fu);
		CHECK(b2.size() == 0);
	}

	SUBCASE("cut fu & bx")
	{
		auto b2 = fu.cut_with(bx);
		CHECK(b2.size() == 4);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 17, 3 } });
		CHECK(b2[1] == Box { Pos { -8, -5 }, Size { 3, 11 } });
		CHECK(b2[1] == Box { Pos { 6, -5 }, Size { 3, 11 } });
		CHECK(b2[2] == Box { Pos { -8, 6 }, Size { 17, 3 } });
	}

	// non-overlapping boxen
	SUBCASE("cut mm & ul")
	{
		auto b2 = mm.cut_with(ul);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut ul & mm")
	{
		auto b2 = ul.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == ul);
	}

	SUBCASE("cut mm & um")
	{
		auto b2 = mm.cut_with(um);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut um & mm")
	{
		auto b2 = um.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == um);
	}

	SUBCASE("cut mm & ur")
	{
		auto b2 = mm.cut_with(ur);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut ur & mm")
	{
		auto b2 = ur.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == ur);
	}

	SUBCASE("cut mm & ml")
	{
		auto b2 = mm.cut_with(ml);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut ml & mm")
	{
		auto b2 = ml.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == ml);
	}

	SUBCASE("cut mm & mm")
	{
		auto b2 = mm.cut_with(mm);
		CHECK(b2.size() == 0);
	}

	SUBCASE("cut mm & mr")
	{
		auto b2 = mm.cut_with(mr);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut mr & mm")
	{
		auto b2 = mr.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mr);
	}

	SUBCASE("cut mm & hc")
	{
		auto b2 = mm.cut_with(hc);
		CHECK(b2.size() == 0);
	}

	SUBCASE("cut hc & mm")
	{
		auto b2 = hc.cut_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -8, -3 }, Size { 5, 7 } });
		CHECK(b2[1] == Box { Pos { 4, -3 }, Size { 5, 7 } });
	}

	SUBCASE("cut mm & ll")
	{
		auto b2 = mm.cut_with(ll);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut ll & mm")
	{
		auto b2 = ll.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == ll);
	}

	SUBCASE("cut mm & lc")
	{
		auto b2 = mm.cut_with(lc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut lc & mm")
	{
		auto b2 = lc.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == lc);
	}

	SUBCASE("cut mm & lr")
	{
		auto b2 = mm.cut_with(lr);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut lr & mm")
	{
		auto b2 = lr.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == lr);
	}

	SUBCASE("cut mm & loc")
	{
		auto b2 = mm.cut_with(loc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut loc & mm")
	{
		auto b2 = loc.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == loc);
	}

	SUBCASE("cut mm & lc")
	{
		auto b2 = mm.cut_with(lc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut lc & mm")
	{
		auto b2 = lc.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == lc);
	}

	SUBCASE("cut mm & vc")
	{
		auto b2 = mm.cut_with(vc);
		CHECK(b2.size() == 0);
	}

	SUBCASE("cut vc & mm")
	{
		auto b2 = vc.cut_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -3, -8 }, Size { 7, 5 } });
		CHECK(b2[1] == Box { Pos { -3, 4 }, Size { 7, 5 } });
	}

	SUBCASE("cut mm & rc")
	{
		auto b2 = mm.cut_with(rc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("cut rc & mm")
	{
		auto b2 = rc.cut_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == rc);
	}
}

TEST_CASE("Box union")
{
	Box bx { Pos { -5, -5 }, Size { 11, 11 } };
	//Box bz {};

	Box n0 { Pos { 2, 2 }, Size { 0, 0 } };
	Box n1 { Pos { 2, 2 }, Size { 1, 0 } };
	Box n2 { Pos { 2, 2 }, Size { 0, 1 } };
	Box ul { Pos { -8, -8 }, Size { 5, 5 } };
	Box um { Pos { -3, -8 }, Size { 7, 5 } };
	Box ur { Pos {  4, -8 }, Size { 5, 5 } };
	Box uc { Pos { -8, -8 }, Size { 17, 5 } };
	Box ml { Pos { -8, -3 }, Size { 5, 7 } };
	Box mm { Pos { -3, -3 }, Size { 7, 7 } };
	Box mr { Pos {  4, -3 }, Size { 5, 7 } };
	Box hc { Pos { -8, -3 }, Size { 17, 7 } };
	Box ll { Pos { -8,  4 }, Size { 5, 5 } };
	Box lm { Pos { -3,  4 }, Size { 7, 5 } };
	Box lr { Pos {  4,  4 }, Size { 5, 5 } };
	Box loc { Pos { -8, 4 }, Size { 17, 5 } };
	Box lc { Pos { -8, -8 }, Size { 5, 17 } };
	Box vc { Pos { -3, -8 }, Size { 7, 17 } };
	Box rc { Pos { 4, -8 }, Size { 5, 17 } };
	Box fu { Pos { -8, -8 }, Size { 17, 17 } };

	SUBCASE("union bx & n0")
	{
		auto b2 = bx.union_with(n0);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == bx);
	}

	SUBCASE("union n0 & bx")
	{
		auto b2 = n0.union_with(bx);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == bx);
	}

	SUBCASE("union bx & n1")
	{
		auto b2 = bx.union_with(n1);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == bx);
	}

	SUBCASE("union n1 & bx")
	{
		auto b2 = n1.union_with(bx);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == bx);
	}

	SUBCASE("union bx & n2")
	{
		auto b2 = bx.union_with(n2);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == bx);
	}

	SUBCASE("union n2 & bx")
	{
		auto b2 = n2.union_with(bx);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == bx);
	}

	SUBCASE("union bx & ul")
	{
		auto b2 = bx.union_with(ul);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -8, -5 }, Size { 14, 2 } });
		CHECK(b2[2] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}

	SUBCASE("union ul & bx")
	{
		auto b2 = ul.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -8, -5 }, Size { 14, 2 } });
		CHECK(b2[2] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}

	SUBCASE("union bx & um")
	{
		auto b2 = bx.union_with(um);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -3, -8 }, Size { 7, 3 } });
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 11 } });
	}

	SUBCASE("union um & bx")
	{
		auto b2 = um.union_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -3, -8 }, Size { 7, 3 } });
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 11 } });
	}

	SUBCASE("union bx & ur")
	{
		auto b2 = bx.union_with(ur);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { 4, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -5, -5 }, Size { 14, 2 } });
		CHECK(b2[2] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}

	SUBCASE("union ur & bx")
	{
		auto b2 = ur.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { 4, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -5, -5 }, Size { 14, 2 } });
		CHECK(b2[2] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}

	SUBCASE("union bx & uc")
	{
		auto b2 = bx.union_with(uc);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 17, 5 } });
		CHECK(b2[0] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}
	
	SUBCASE("union uc & bx")
	{
		auto b2 = uc.union_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 17, 5 } });
		CHECK(b2[0] == Box { Pos { -5, -3 }, Size { 11, 9 } });
	}

	SUBCASE("union bx & ml")
	{
		auto b2 = bx.union_with(ml);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -8, -3 }, Size { 14, 7 } });
		CHECK(b2[2] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("union ml & bx")
	{
		auto b2 = ml.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -8, -3 }, Size { 14, 7 } });
		CHECK(b2[2] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("union bx & mm")
	{
		auto b2 = bx.union_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("union mm & bx")
	{
		auto b2 = mm.union_with(bx);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("union bx & mr")
	{
		auto b2 = bx.union_with(mr);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -5, -3 }, Size { 14, 7 } });
		CHECK(b2[2] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("union mr & bx")
	{
		auto b2 = mr.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -5, -3 }, Size { 14, 7 } });
		CHECK(b2[2] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("union bx & hc")
	{
		auto b2 = bx.union_with(hc);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -8, -3 }, Size { 17, 7 } });
		CHECK(b2[2] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("union hc & bx")
	{
		auto b2 = hc.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 2 } });
		CHECK(b2[1] == Box { Pos { -8, -3 }, Size { 17, 7 } });
		CHECK(b2[2] == Box { Pos { -5, 4 }, Size { 11, 2 } });
	}

	SUBCASE("union bx & ll")
	{
		auto b2 = bx.union_with(ll);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
		CHECK(b2[1] == Box { Pos { -8, 4 }, Size { 14, 2 } });
		CHECK(b2[2] == Box { Pos { -8, 6 }, Size { 5, 3 } });
	}

	SUBCASE("union ll & bx")
	{
		auto b2 = ll.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
		CHECK(b2[1] == Box { Pos { -8, 4 }, Size { 14, 2 } });
		CHECK(b2[2] == Box { Pos { -8, 6 }, Size { 5, 3 } });
	}

	SUBCASE("union bx & lm")
	{
		auto b2 = bx.union_with(lm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 11 } });
		CHECK(b2[1] == Box { Pos { -3, 6 }, Size { 7, 3 } });
	}

	SUBCASE("union lm & bx")
	{
		auto b2 = lm.union_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 11 } });
		CHECK(b2[1] == Box { Pos { -3, 6 }, Size { 7, 3 } });
	}

	SUBCASE("union bx & lr")
	{
		auto b2 = bx.union_with(lr);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
		CHECK(b2[1] == Box { Pos { -5, 4 }, Size { 14, 2 } });
		CHECK(b2[2] == Box { Pos { -5, 6 }, Size { 5, 3 } });
	}

	SUBCASE("union lr & bx")
	{
		auto b2 = lr.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
		CHECK(b2[1] == Box { Pos { -5, 4 }, Size { 14, 2 } });
		CHECK(b2[2] == Box { Pos { -5, 6 }, Size { 5, 3 } });
	}

	SUBCASE("union bx & loc")
	{
		auto b2 = bx.union_with(loc);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
		CHECK(b2[1] == Box { Pos { -8, 4 }, Size { 17, 3 } });
	}
	
	SUBCASE("union loc & bx")
	{
		auto b2 = loc.union_with(bx);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == Box { Pos { -5, -5 }, Size { 11, 9 } });
		CHECK(b2[1] == Box { Pos { -8, 4 }, Size { 17, 3 } });
	}

	SUBCASE("union bx & lc")
	{
		auto b2 = bx.union_with(lc);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -8, -5 }, Size { 14, 11 } });
		CHECK(b2[2] == Box { Pos { -8, 6 }, Size { 5, 3 } });
	}
	
	SUBCASE("union lc & bx")
	{
		auto b2 = lc.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -8, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -8, -5 }, Size { 14, 11 } });
		CHECK(b2[2] == Box { Pos { -8, 6 }, Size { 5, 3 } });
	}

	SUBCASE("union bx & vc")
	{
		auto b2 = bx.union_with(vc);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -8, -5 }, Size { 14, 11 } });
		CHECK(b2[2] == Box { Pos { -5, 6 }, Size { 5, 3 } });
	}

	SUBCASE("union vc & bx")
	{
		auto b2 = vc.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -8, -5 }, Size { 14, 11 } });
		CHECK(b2[2] == Box { Pos { -5, 6 }, Size { 5, 3 } });
	}

	SUBCASE("union bx & rc")
	{
		auto b2 = bx.union_with(rc);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -5, -5 }, Size { 14, 11 } });
		CHECK(b2[2] == Box { Pos { -5, 6 }, Size { 5, 3 } });
	}

	SUBCASE("union rc & bx")
	{
		auto b2 = rc.union_with(bx);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == Box { Pos { -5, -8 }, Size { 5, 3 } });
		CHECK(b2[1] == Box { Pos { -5, -5 }, Size { 14, 11 } });
		CHECK(b2[2] == Box { Pos { -5, 6 }, Size { 5, 3 } });
	}

	SUBCASE("union bx & fu")
	{
		auto b2 = bx.union_with(fu);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == fu);
	}

	SUBCASE("union fu & bx")
	{
		auto b2 = fu.union_with(bx);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == fu);
	}

	// non-overlapping boxen
	SUBCASE("union mm & ul")
	{
		auto b2 = mm.union_with(ul);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == ul);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union ul & mm")
	{
		auto b2 = ul.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == ul);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union mm & um")
	{
		auto b2 = mm.union_with(um);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == ul);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union um & mm")
	{
		auto b2 = um.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == ul);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union mm & ur")
	{
		auto b2 = mm.union_with(ur);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == ur);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union ur & mm")
	{
		auto b2 = ur.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == ur);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union mm & ml")
	{
		auto b2 = mm.union_with(ml);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == ml);
	}

	SUBCASE("union ml & mm")
	{
		auto b2 = ml.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == ml);
	}

	SUBCASE("union mm & mm")
	{
		auto b2 = mm.union_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == mm);
	}

	SUBCASE("union mm & mr")
	{
		auto b2 = mm.union_with(mr);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == mr);
	}

	SUBCASE("union mr & mm")
	{
		auto b2 = mr.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == mr);
	}

	SUBCASE("union mm & hc")
	{
		auto b2 = mm.union_with(hc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == hc);
	}

	SUBCASE("union hc & mm")
	{
		auto b2 = hc.union_with(mm);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == hc);
	}

	SUBCASE("union mm & ll")
	{
		auto b2 = mm.union_with(ll);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == ll);
	}

	SUBCASE("union ll & mm")
	{
		auto b2 = ll.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == ll);
	}

	SUBCASE("union mm & lm")
	{
		auto b2 = mm.union_with(lm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == lm);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union lm & mm")
	{
		auto b2 = lm.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == lm);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union mm & lr")
	{
		auto b2 = mm.union_with(lr);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == lr);
	}

	SUBCASE("union lr & mm")
	{
		auto b2 = lr.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == lr);
	}

	SUBCASE("union mm & loc")
	{
		auto b2 = mm.union_with(loc);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == loc);
	}

	SUBCASE("union loc & mm")
	{
		auto b2 = loc.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == mm);
		CHECK(b2[1] == loc);
	}

	SUBCASE("union mm & lc")
	{
		auto b2 = mm.union_with(lc);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == lc);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union lc & mm")
	{
		auto b2 = lc.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == lc);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union mm & vc")
	{
		auto b2 = mm.union_with(vc);
		CHECK(b2.size() == 1);
		CHECK(b2[0] == vc);
	}

	SUBCASE("union vc & mm")
	{
		auto b2 = vc.union_with(mm);
		CHECK(b2.size() == 3);
		CHECK(b2[0] == um);
		CHECK(b2[1] == mm);
		CHECK(b2[2] == lm);
	}

	SUBCASE("union mm & rc")
	{
		auto b2 = mm.union_with(rc);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == rc);
		CHECK(b2[1] == mm);
	}

	SUBCASE("union rc & mm")
	{
		auto b2 = rc.union_with(mm);
		CHECK(b2.size() == 2);
		CHECK(b2[0] == rc);
		CHECK(b2[1] == mm);
	}
}
