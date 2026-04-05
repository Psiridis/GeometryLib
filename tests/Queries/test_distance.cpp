#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Queries/distance.hpp"

#include <gtest/gtest.h>

#include <cmath>

using namespace Geometry;

static constexpr double kTol = 1e-9;

// ── distance(Point, Line) ─────────────────────────────────────────────────────

TEST(DistancePointLine, PointOnLine)
{
	// Point lies on the line → distance = 0
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(3.0, 0.0, 0.0);

	EXPECT_NEAR(distance(p, line), 0.0, kTol);
}

TEST(DistancePointLine, PointAtUnitDistance)
{
	// Point (0,1,0) from x-axis → distance = 1
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(0.0, 1.0, 0.0);

	EXPECT_NEAR(distance(p, line), 1.0, kTol);
}

TEST(DistancePointLine, Pythagorean345)
{
	// Point (0,3,4) from x-axis → sqrt(3²+4²) = 5
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(0.0, 3.0, 4.0);

	EXPECT_NEAR(distance(p, line), 5.0, kTol);
}

TEST(DistancePointLine, NonUnitDirection)
{
	// Scaled direction must give the same result
	Line line(Point(0.0, 0.0, 0.0), Vector(3.0, 0.0, 0.0));
	Point p(0.0, 3.0, 4.0);

	EXPECT_NEAR(distance(p, line), 5.0, kTol);
}

TEST(DistancePointLine, FootBehindOrigin)
{
	// Point at (-5,3,0): foot lies behind origin, distance is still 3
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(-5.0, 3.0, 0.0);

	EXPECT_NEAR(distance(p, line), 3.0, kTol);
}

TEST(DistancePointLine, OffsetLineOrigin)
{
	// Same geometry but line origin offset along its direction
	Line line(Point(10.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(0.0, 4.0, 0.0);

	EXPECT_NEAR(distance(p, line), 4.0, kTol);
}

// ── distance(Point, Plane) ────────────────────────────────────────────────────

TEST(DistancePointPlane, PointOnPlane)
{
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Point p(3.0, 7.0, 0.0);

	EXPECT_NEAR(distance(p, plane), 0.0, kTol);
}

TEST(DistancePointPlane, PointAbovePlane)
{
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Point p(0.0, 0.0, 5.0);

	EXPECT_NEAR(distance(p, plane), 5.0, kTol);
}

TEST(DistancePointPlane, PointBelowPlaneIsPositive)
{
	// distance is always non-negative regardless of which side the point is on
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Point p(0.0, 0.0, -5.0);

	EXPECT_NEAR(distance(p, plane), 5.0, kTol);
}

TEST(DistancePointPlane, OffsetPlane)
{
	// Plane at z=3; point at z=7 → distance = 4
	Plane plane(Point(0.0, 0.0, 3.0), Vector(0.0, 0.0, 1.0));
	Point p(0.0, 0.0, 7.0);

	EXPECT_NEAR(distance(p, plane), 4.0, kTol);
}

TEST(DistancePointPlane, ObliqueNormal)
{
	// Plane with normal (1,0,0) (yz-plane); point at x=3 → distance = 3
	Plane plane(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(3.0, 5.0, 7.0);

	EXPECT_NEAR(distance(p, plane), 3.0, kTol);
}

// ── distance(Line, Line) ──────────────────────────────────────────────────────

TEST(DistanceLineLine, IdenticalLines)
{
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));

	EXPECT_NEAR(distance(a, b), 0.0, kTol);
}

TEST(DistanceLineLine, ParallelLines)
{
	// Both along x; one shifted by (0,3,4) → distance = 5
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 3.0, 4.0), Vector(1.0, 0.0, 0.0));

	EXPECT_NEAR(distance(a, b), 5.0, kTol);
}

TEST(DistanceLineLine, AntiParallelLines)
{
	// Same geometry, direction reversed
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 3.0, 4.0), Vector(-1.0, 0.0, 0.0));

	EXPECT_NEAR(distance(a, b), 5.0, kTol);
}

TEST(DistanceLineLine, IntersectingLines)
{
	// x-axis and y-axis (both through origin) → distance = 0
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));

	EXPECT_NEAR(distance(a, b), 0.0, kTol);
}

TEST(DistanceLineLine, SkewLinesUnitDistance)
{
	// x-axis and y-axis shifted by (0,0,1): skew distance = 1
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 0.0, 1.0), Vector(0.0, 1.0, 0.0));

	EXPECT_NEAR(distance(a, b), 1.0, kTol);
}

TEST(DistanceLineLine, SkewLinesKnownDistance)
{
	// x-axis and z-axis shifted by (0,3,0): skew distance = 3
	// d1=(1,0,0), d2=(0,0,1): cross=(0,-1,0), w=(0,3,0), |w·cross|/|cross| = 3
	Line a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line b(Point(0.0, 3.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_NEAR(distance(a, b), 3.0, kTol);
}

TEST(DistanceLineLine, SkewLinesScaledDirections)
{
	// Same skew geometry but directions scaled — result must be identical
	Line a(Point(0.0, 0.0, 0.0), Vector(5.0, 0.0, 0.0));
	Line b(Point(0.0, 0.0, 1.0), Vector(0.0, 3.0, 0.0));

	EXPECT_NEAR(distance(a, b), 1.0, kTol);
}

TEST(DistanceLineLine, Symmetry)
{
	// distance(a, b) == distance(b, a)
	Line a(Point(1.0, 0.0, 0.0), Vector(1.0, 1.0, 0.0));
	Line b(Point(0.0, 1.0, 2.0), Vector(0.0, 1.0, 1.0));

	EXPECT_NEAR(distance(a, b), distance(b, a), kTol);
}
