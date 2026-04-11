#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/ray.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Queries/parallel.hpp"
#include "Geometry/Shapes/segment.hpp"

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

// ── is_parallel(Ray, Ray) ─────────────────────────────────────────────────────

TEST(IsParallelRayRay, SameDirection)
{
	Ray a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Ray b(Point(0.0, 5.0, 0.0), Vector(1.0, 0.0, 0.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelRayRay, AntiParallelDirection)
{
	Ray a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Ray b(Point(0.0, 5.0, 0.0), Vector(-1.0, 0.0, 0.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelRayRay, ScaledDirection)
{
	Ray a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Ray b(Point(0.0, 5.0, 0.0), Vector(100.0, 0.0, 0.0));

	EXPECT_TRUE(is_parallel(a, b));
}

TEST(IsParallelRayRay, PerpendicularRays)
{
	Ray a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Ray b(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));

	EXPECT_FALSE(is_parallel(a, b));
}

TEST(IsParallelRayRay, ObliqueRays)
{
	Ray a(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Ray b(Point(0.0, 0.0, 0.0), Vector(1.0, 1.0, 0.0));

	EXPECT_FALSE(is_parallel(a, b));
}

// ── is_parallel(Ray, Plane) ───────────────────────────────────────────────────

TEST(IsParallelRayPlane, RayAlongPlane)
{
	// z=0 plane; ray along x-axis lies inside the plane
	Ray ray(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(ray, plane));
}

TEST(IsParallelRayPlane, RayParallelOffsetFromPlane)
{
	// Ray parallel to z=0 but at y=5
	Ray ray(Point(0.0, 5.0, 3.0), Vector(1.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(ray, plane));
}

TEST(IsParallelRayPlane, RayPerpendicularToPlane)
{
	// z=0 plane; ray along z-axis aims directly through it
	Ray ray(Point(0.0, 0.0, -1.0), Vector(0.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_FALSE(is_parallel(ray, plane));
}

TEST(IsParallelRayPlane, ObliqueRay)
{
	// Ray at 45° to z=0
	Ray ray(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_FALSE(is_parallel(ray, plane));
}

TEST(IsParallelRayPlane, Symmetry)
{
	// is_parallel(ray, plane): direction perpendicular to normal → parallel
	Ray ray(Point(1.0, 2.0, 3.0), Vector(0.0, 1.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(ray, plane));
}

// ── is_parallel(Line, Plane) ────────────────────────────────────────────────

TEST(IsParallelLinePlane, LineAlongPlane)
{
	// z=0 plane; line along x-axis lies flat in the plane
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(line, plane));
}

TEST(IsParallelLinePlane, LineParallelOffset)
{
	// Line at z=5 running along x — parallel to z=0 but not coplanar
	Line line(Point(0.0, 0.0, 5.0), Vector(1.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(line, plane));
}

TEST(IsParallelLinePlane, LinePerpendicularToPlane)
{
	// z=0 plane; line along z-axis pierces through it
	Line line(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_FALSE(is_parallel(line, plane));
}

TEST(IsParallelLinePlane, ObliqueLineAndPlane)
{
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_FALSE(is_parallel(line, plane));
}

TEST(IsParallelLinePlane, ScaledDirection)
{
	// A scaled direction must give the same result
	Line line(Point(0.0, 0.0, 0.0), Vector(100.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(line, plane));
}

// ── is_parallel(Segment, Plane) ─────────────────────────────────────────────

TEST(IsParallelSegmentPlane, SegmentAlongPlane)
{
	// Segment in z=0 plane → parallel
	Segment seg(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(seg, plane));
}

TEST(IsParallelSegmentPlane, SegmentParallelOffset)
{
	// Segment at z=3 running along y — parallel to z=0 plane
	Segment seg(Point(0.0, 0.0, 3.0), Point(0.0, 1.0, 3.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(is_parallel(seg, plane));
}

TEST(IsParallelSegmentPlane, SegmentCrossesPlane)
{
	Segment seg(Point(0.0, 0.0, -1.0), Point(0.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_FALSE(is_parallel(seg, plane));
}

TEST(IsParallelSegmentPlane, ObliqueSegment)
{
	Segment seg(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_FALSE(is_parallel(seg, plane));
}
