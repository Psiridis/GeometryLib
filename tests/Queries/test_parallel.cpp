#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Queries/parallel.hpp"

#include <gtest/gtest.h>

using namespace Geometry;

// ── is_parallel(Line, Line) ───────────────────────────────────────────────────

TEST(IsParallelLine, SameDirection)
{
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 5.0, 0.0), Vector(1.0, 0.0, 0.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelLine, AntiParallelDirection)
{
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 5.0, 0.0), Vector(-1.0, 0.0, 0.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelLine, ScaledDirection)
{
	// Different magnitudes, same geometric direction
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 5.0, 0.0), Vector(100.0, 0.0, 0.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelLine, CoincidentLine)
{
	// Same line — directions are identical
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(3.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelLine, PerpendicularLines)
{
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));

	EXPECT_FALSE(is_parallel(a, b));
}

TEST(IsParallelLine, ObliqueLines)
{
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 0.0, 0.0), Vector(1.0, 1.0, 0.0));

	EXPECT_FALSE(is_parallel(a, b));
}

TEST(IsParallelLine, SkewLines)
{
	// Different directions and different planes — not parallel
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 0.0, 1.0), Vector(0.0, 1.0, 0.0));

	EXPECT_FALSE(is_parallel(a, b));
}

// ── is_parallel(Plane, Plane) ─────────────────────────────────────────────────

TEST(IsParallelPlane, SameNormal)
{
	Plane a(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane b(Point(0.0, 0.0, 5.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelPlane, AntiParallelNormal)
{
	Plane a(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane b(Point(0.0, 0.0, 5.0), Vector(0.0, 0.0, -1.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelPlane, CoincidentPlanes)
{
	// Same plane, same normal
	Plane a(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane b(Point(1.0, 2.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelPlane, PerpendicularPlanes)
{
	Plane a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Plane b(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));

	EXPECT_FALSE(is_parallel(a, b));
}

TEST(IsParallelPlane, ObliquePlanes)
{
	Plane a(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane b(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 1.0));

	EXPECT_FALSE(is_parallel(a, b));
}
