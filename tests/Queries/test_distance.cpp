#include "Geometry/Bounds/bounding_box.hpp"
#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Queries/distance.hpp"
#include "Geometry/Shapes/segment.hpp"
#include "Geometry/Shapes/triangle.hpp"

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

// ── distance(Point, Segment) ──────────────────────────────────────────────

TEST(DistancePointSegment, PointOnSegment)
{
	Segment seg(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point p(2.0, 0.0, 0.0);

	EXPECT_NEAR(distance(p, seg), 0.0, kTol);
}

TEST(DistancePointSegment, PointPerpendicularToMidpoint)
{
	// (2, 3, 0) to segment on x-axis [0, 4] -- foot at (2, 0, 0)
	Segment seg(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point p(2.0, 3.0, 0.0);

	EXPECT_NEAR(distance(p, seg), 3.0, kTol);
}

TEST(DistancePointSegment, PointBeyondEnd)
{
	// Point past the end clamps to the endpoint
	Segment seg(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point p(7.0, 0.0, 0.0);

	EXPECT_NEAR(distance(p, seg), 3.0, kTol);
}

TEST(DistancePointSegment, PointBeforeStart)
{
	// Point before the start clamps to the start endpoint
	Segment seg(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point p(-3.0, 0.0, 0.0);

	EXPECT_NEAR(distance(p, seg), 3.0, kTol);
}

// ── distance(Point, Triangle) ─────────────────────────────────────────────

TEST(DistancePointTriangle, PointAboveInterior)
{
	// Triangle in z=0; point at (0.25, 0.25, 5) → interior projection, dist=5
	Triangle tri(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
	Point p(0.25, 0.25, 5.0);

	EXPECT_NEAR(distance(p, tri), 5.0, kTol);
}

TEST(DistancePointTriangle, PointOnPlaneInside)
{
	// Point on the plane and inside the triangle → distance = 0
	Triangle tri(Point(0.0, 0.0, 0.0), Point(2.0, 0.0, 0.0), Point(0.0, 2.0, 0.0));
	Point p(0.5, 0.5, 0.0);

	EXPECT_NEAR(distance(p, tri), 0.0, kTol);
}

TEST(DistancePointTriangle, PointBesideEdge)
{
	// Point (-1, 0.5, 0) projects outside the triangle near the edge p0-p2;
	// closest point is on segment [(0,0,0),(0,1,0)], foot at (0,0.5,0), dist=1
	Triangle tri(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
	Point p(-1.0, 0.5, 0.0);

	EXPECT_NEAR(distance(p, tri), 1.0, kTol);
}

TEST(DistancePointTriangle, PointAboveVertex)
{
	// Point directly above vertex p0=(0,0,0) but outside the triangle
	// Closest point is p0, distance = sqrt(1+0+0+4) = sqrt(1+4) = sqrt(5)
	Triangle tri(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
	Point p(-1.0, -1.0, 0.0);

	// The nearest point on any edge is p0=(0,0,0)  (both e0 and e2 clamp there)
	EXPECT_NEAR(distance(p, tri), std::sqrt(2.0), kTol);
}

// ── distance(Point, BoundingBox) ──────────────────────────────────────────

TEST(DistancePointAABB, PointInsideBox)
{
	BoundingBox box(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
	Point p(0.5, 0.5, 0.5);

	EXPECT_NEAR(distance(p, box), 0.0, kTol);
}

TEST(DistancePointAABB, PointOnSurface)
{
	// On the face x=0
	BoundingBox box(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
	Point p(0.0, 0.5, 0.5);

	EXPECT_NEAR(distance(p, box), 0.0, kTol);
}

TEST(DistancePointAABB, PointOutsideOneFace)
{
	// Point at x=3, inside y/z slabs → excess = 3-1 = 2
	BoundingBox box(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
	Point p(3.0, 0.5, 0.5);

	EXPECT_NEAR(distance(p, box), 2.0, kTol);
}

TEST(DistancePointAABB, PointOutsideTwoFaces)
{
	// Excess: dx=2 (x=3 → 3-1=2), dy=3 (y=-3 → 0-(-3)=3); dist = sqrt(4+9) = sqrt(13)
	BoundingBox box(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
	Point p(3.0, -3.0, 0.5);

	EXPECT_NEAR(distance(p, box), std::sqrt(13.0), kTol);
}

TEST(DistancePointAABB, PointOutsideCorner)
{
	// Excess in all three axes: (1,1,1) → dist = sqrt(3)
	BoundingBox box(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
	Point p(2.0, 2.0, 2.0);

	EXPECT_NEAR(distance(p, box), std::sqrt(3.0), kTol);
}

// ── distance(Segment, Segment) ─────────────────────────────────────────────

TEST(DistanceSegmentSegment, TouchingEndpoints)
{
	// Two collinear segments touching at (1,0,0)
	Segment a(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	Segment b(Point(1.0, 0.0, 0.0), Point(2.0, 0.0, 0.0));

	EXPECT_NEAR(distance(a, b), 0.0, kTol);
}

TEST(DistanceSegmentSegment, Overlapping)
{
	// Overlapping collinear segments → distance = 0
	Segment a(Point(0.0, 0.0, 0.0), Point(3.0, 0.0, 0.0));
	Segment b(Point(1.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));

	EXPECT_NEAR(distance(a, b), 0.0, kTol);
}

TEST(DistanceSegmentSegment, ParallelWithGap)
{
	// Two parallel x-axis segments separated by 3 in y, same x range
	Segment a(Point(0.0, 0.0, 0.0), Point(2.0, 0.0, 0.0));
	Segment b(Point(0.0, 3.0, 0.0), Point(2.0, 3.0, 0.0));

	EXPECT_NEAR(distance(a, b), 3.0, kTol);
}

TEST(DistanceSegmentSegment, SkewSegments)
{
	// a: x-axis [0,1], b: z-axis at y=2 [0,1] → closest pair (1,0,0)/(1..no)
	// a along x, b along z shifted by y=2: closest points (0.5..no)
	// a: [0,0,0]-[1,0,0], b: [0,2,0]-[0,2,1]
	// Closest: (0,0,0) on a to (0,2,0) on b? No, let us use a clear case.
	// a: [(0,0,0),(2,0,0)], b: [(1,3,0),(1,3,2)] shifted 3 in y
	// closest on a: (1,0,0), on b: (1,3,0), dist=3
	Segment a(Point(0.0, 0.0, 0.0), Point(2.0, 0.0, 0.0));
	Segment b(Point(1.0, 3.0, 0.0), Point(1.0, 3.0, 2.0));

	EXPECT_NEAR(distance(a, b), 3.0, kTol);
}

TEST(DistanceSegmentSegment, Symmetry)
{
	Segment a(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 0.0));
	Segment b(Point(1.0, 0.0, 1.0), Point(0.0, 1.0, 2.0));

	EXPECT_NEAR(distance(a, b), distance(b, a), kTol);
}
