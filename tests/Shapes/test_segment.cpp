#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Shapes/segment.hpp"

#include <gtest/gtest.h>

#include <cmath>

using namespace Geometry;

static constexpr double kTol = 1e-9;

// ── Construction ──────────────────────────────────────────────────────────────

TEST(SegmentConstruction, ValidEndpoints)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	EXPECT_EQ(s.start(), Point(0.0, 0.0, 0.0));
	EXPECT_EQ(s.end(), Point(1.0, 0.0, 0.0));
}

TEST(SegmentConstruction, CoincidentPointsAborts)
{
	// In debug builds GEOM_ASSERT fires before the throw → process aborts.
	// In release builds the throw fires instead — tested separately if needed.
	EXPECT_DEATH(Segment(Point(1.0, 2.0, 3.0), Point(1.0, 2.0, 3.0)), "");
}

// ── length / length_squared ───────────────────────────────────────────────────

TEST(SegmentLength, UnitSegment)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	EXPECT_NEAR(s.length(), 1.0, kTol);
	EXPECT_NEAR(s.length_squared(), 1.0, kTol);
}

TEST(SegmentLength, Pythagorean345)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(3.0, 4.0, 0.0));
	EXPECT_NEAR(s.length(), 5.0, kTol);
	EXPECT_NEAR(s.length_squared(), 25.0, kTol);
}

TEST(SegmentLength, DiagonalIn3D)
{
	// (1,1,1) → length = sqrt(3)
	Segment s(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
	EXPECT_NEAR(s.length(), std::sqrt(3.0), kTol);
	EXPECT_NEAR(s.length_squared(), 3.0, kTol);
}

// ── midpoint ──────────────────────────────────────────────────────────────────

TEST(SegmentMidpoint, AxisAligned)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point m = s.midpoint();
	EXPECT_NEAR(m.x(), 2.0, kTol);
	EXPECT_NEAR(m.y(), 0.0, kTol);
	EXPECT_NEAR(m.z(), 0.0, kTol);
}

TEST(SegmentMidpoint, ArbitraryEndpoints)
{
	Segment s(Point(1.0, 3.0, -1.0), Point(3.0, 1.0, 3.0));
	Point m = s.midpoint();
	EXPECT_NEAR(m.x(), 2.0, kTol);
	EXPECT_NEAR(m.y(), 2.0, kTol);
	EXPECT_NEAR(m.z(), 1.0, kTol);
}

// ── direction ─────────────────────────────────────────────────────────────────

TEST(SegmentDirection, UnitResult)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(3.0, 4.0, 0.0));
	Vector d = s.direction();
	EXPECT_NEAR(d.length(), 1.0, kTol);
	EXPECT_NEAR(d.dx(), 3.0 / 5.0, kTol);
	EXPECT_NEAR(d.dy(), 4.0 / 5.0, kTol);
	EXPECT_NEAR(d.dz(), 0.0, kTol);
}

TEST(SegmentDirection, AxisAligned)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 7.0));
	Vector d = s.direction();
	EXPECT_NEAR(d.dx(), 0.0, kTol);
	EXPECT_NEAR(d.dy(), 0.0, kTol);
	EXPECT_NEAR(d.dz(), 1.0, kTol);
}

// ── at(t) ─────────────────────────────────────────────────────────────────────

TEST(SegmentAt, tZeroIsStart)
{
	Segment s(Point(1.0, 2.0, 3.0), Point(5.0, 6.0, 7.0));
	Point p = s.at(0.0);
	EXPECT_NEAR(p.x(), 1.0, kTol);
	EXPECT_NEAR(p.y(), 2.0, kTol);
	EXPECT_NEAR(p.z(), 3.0, kTol);
}

TEST(SegmentAt, tOneIsEnd)
{
	Segment s(Point(1.0, 2.0, 3.0), Point(5.0, 6.0, 7.0));
	Point p = s.at(1.0);
	EXPECT_NEAR(p.x(), 5.0, kTol);
	EXPECT_NEAR(p.y(), 6.0, kTol);
	EXPECT_NEAR(p.z(), 7.0, kTol);
}

TEST(SegmentAt, tHalfIsMidpoint)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point p = s.at(0.5);
	EXPECT_NEAR(p.x(), 2.0, kTol);
}

TEST(SegmentAt, tBelowZeroClampedToStart)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	Point p = s.at(-5.0);
	EXPECT_NEAR(p.x(), 0.0, kTol);
}

TEST(SegmentAt, tAboveOneClampedToEnd)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	Point p = s.at(3.0);
	EXPECT_NEAR(p.x(), 1.0, kTol);
}

// ── contains ─────────────────────────────────────────────────────────────────

TEST(SegmentContains, StartPoint)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	EXPECT_TRUE(s.contains(Point(0.0, 0.0, 0.0)));
}

TEST(SegmentContains, EndPoint)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	EXPECT_TRUE(s.contains(Point(4.0, 0.0, 0.0)));
}

TEST(SegmentContains, Midpoint)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	EXPECT_TRUE(s.contains(Point(2.0, 0.0, 0.0)));
}

TEST(SegmentContains, PointBeyondEnd)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	EXPECT_FALSE(s.contains(Point(5.0, 0.0, 0.0)));
}

TEST(SegmentContains, PointOffSegment)
{
	// Point at (2,1,0) is off the x-axis segment
	Segment s(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	EXPECT_FALSE(s.contains(Point(2.0, 1.0, 0.0)));
}

TEST(SegmentContains, DiagonalSegment)
{
	Segment s(Point(0.0, 0.0, 0.0), Point(3.0, 4.0, 0.0));
	// (1.5, 2.0, 0) is the exact midpoint
	EXPECT_TRUE(s.contains(Point(1.5, 2.0, 0.0)));
}

// ── operator== / operator!= ───────────────────────────────────────────────────

TEST(SegmentEquality, SameSegment)
{
	Segment a(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	Segment b(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	EXPECT_TRUE(a == b);
	EXPECT_FALSE(a != b);
}

TEST(SegmentEquality, ReversedIsNotEqual)
{
	// Segment is directed: (A,B) ≠ (B,A)
	Segment a(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	Segment b(Point(1.0, 0.0, 0.0), Point(0.0, 0.0, 0.0));
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a != b);
}

TEST(SegmentEquality, DifferentEndpoint)
{
	Segment a(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	Segment b(Point(0.0, 0.0, 0.0), Point(2.0, 0.0, 0.0));
	EXPECT_FALSE(a == b);
}
