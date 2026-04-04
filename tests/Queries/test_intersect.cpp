#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/ray.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Queries/intersect.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <optional>

using namespace Geometry;

// ── Helpers ───────────────────────────────────────────────────────────────────

static constexpr double kTol = 1e-9;

static void ExpectPointEq(Point const& p, double x, double y, double z)
{
	EXPECT_NEAR(p.x(), x, kTol);
	EXPECT_NEAR(p.y(), y, kTol);
	EXPECT_NEAR(p.z(), z, kTol);
}

// ── intersect(Ray, Plane) ─────────────────────────────────────────────────────

TEST(IntersectRayPlane, RayHitsPlanePerpendicularly)
{
	// Ray along +z, plane at z = 0 with upward normal
	Ray ray(Point(0.0, 0.0, 5.0), Vector(0.0, 0.0, -1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(ray, plane);

	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.0, 0.0);
}

TEST(IntersectRayPlane, RayHitsPlaneAtAngle)
{
	// Ray from (0,0,1) heading toward (1,0,0); hits the z=0 plane at (1,0,0)
	Ray ray(Point(0.0, 0.0, 1.0), Vector(1.0, 0.0, -1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(ray, plane);

	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 1.0, 0.0, 0.0);
}

TEST(IntersectRayPlane, RayOriginOnPlane)
{
	// Ray starts in the plane; t = 0, so intersection is at origin
	Ray ray(Point(1.0, 2.0, 0.0), Vector(0.0, 1.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(ray, plane);

	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 1.0, 2.0, 0.0);
}

TEST(IntersectRayPlane, RayParallelToPlane)
{
	// Ray travels along x; normal is along z → no intersection
	Ray ray(Point(0.0, 0.0, 1.0), Vector(1.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(ray, plane);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectRayPlane, RayPointingAway)
{
	// Ray points away from the plane (t would be negative)
	Ray ray(Point(0.0, 0.0, 5.0), Vector(0.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(ray, plane);

	EXPECT_FALSE(result.has_value());
}

// ── intersect(Line, Plane) ────────────────────────────────────────────────────

TEST(IntersectLinePlane, LineHitsPlanePositiveT)
{
	Line line(Point(0.0, 0.0, 3.0), Vector(0.0, 0.0, -1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(line, plane);

	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.0, 0.0);
}

TEST(IntersectLinePlane, LineHitsPlaneNegativeT)
{
	// Direction points away, but Line is infinite so still intersects
	Line line(Point(0.0, 0.0, 3.0), Vector(0.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(line, plane);

	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.0, 0.0);
}

TEST(IntersectLinePlane, LineParallelToPlane)
{
	Line line(Point(0.0, 0.0, 1.0), Vector(1.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(line, plane);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectLinePlane, LineLiesInPlane)
{
	// Line is entirely within the plane (also parallel, returns nullopt)
	Line line(Point(1.0, 2.0, 0.0), Vector(1.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(line, plane);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectLinePlane, ObliqueIntersection)
{
	// Line from (0,0,2) along (1,0,-2): hits z=0 at (1,0,0)
	Line line(Point(0.0, 0.0, 2.0), Vector(1.0, 0.0, -2.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(line, plane);

	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 1.0, 0.0, 0.0);
}

// ── intersect(Plane, Plane) ───────────────────────────────────────────────────

TEST(IntersectPlanePlane, TwoAxisAlignedPlanes)
{
	// x=0 plane and y=0 plane intersect along the z-axis
	Plane plane1(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Plane plane2(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));

	auto result = intersect(plane1, plane2);

	ASSERT_TRUE(result.has_value());

	// The intersection line must lie in both planes
	Point p	 = result->origin();
	Point p2 = result->at(1.0);
	EXPECT_TRUE(plane1.contains(p));
	EXPECT_TRUE(plane1.contains(p2));
	EXPECT_TRUE(plane2.contains(p));
	EXPECT_TRUE(plane2.contains(p2));

	// Direction must be parallel to z-axis
	Vector dir = result->direction();
	// d × z should be zero for a z-parallel direction
	Vector z_axis(0.0, 0.0, 1.0);
	EXPECT_NEAR(dir.cross(z_axis).length(), 0.0, kTol);
}

TEST(IntersectPlanePlane, TwoObliquePlanes)
{
	// Plane 1: normal (1,1,0)/√2, through origin
	// Plane 2: z=0 plane
	Plane plane1(Point(0.0, 0.0, 0.0), Vector(1.0, 1.0, 0.0));
	Plane plane2(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(plane1, plane2);

	ASSERT_TRUE(result.has_value());

	// Verify several points on the line lie in both planes
	for (double t : { -10.0, 0.0, 5.0 })
	{
		Point p = result->at(t);
		EXPECT_TRUE(plane1.contains(p)) << "plane1 at t=" << t;
		EXPECT_TRUE(plane2.contains(p)) << "plane2 at t=" << t;
	}
}

TEST(IntersectPlanePlane, ParallelPlanesReturnNullopt)
{
	Plane plane1(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane plane2(Point(0.0, 0.0, 5.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(plane1, plane2);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectPlanePlane, CoincidentPlanesReturnNullopt)
{
	// Same plane described slightly differently
	Plane plane1(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane plane2(Point(1.0, 1.0, 0.0), Vector(0.0, 0.0, -1.0));

	auto result = intersect(plane1, plane2);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectPlanePlane, AntiParallelNormalsReturnNullopt)
{
	// Opposite normals but different offset → parallel planes
	Plane plane1(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));
	Plane plane2(Point(0.0, 3.0, 0.0), Vector(0.0, -1.0, 0.0));

	auto result = intersect(plane1, plane2);

	EXPECT_FALSE(result.has_value());
}

// ── intersect(Line, Line) ─────────────────────────────────────────────────────

TEST(IntersectLineLine, TwoIntersectingLines)
{
	// L1: x-axis,  L2: passes through (2,0,0) along y
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line line2(Point(2.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));

	auto result = intersect(line1, line2);

	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 2.0, 0.0, 0.0);
}

TEST(IntersectLineLine, TwoIntersectingLinesAtOrigin)
{
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line line2(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));

	auto result = intersect(line1, line2);

	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.0, 0.0);
}

TEST(IntersectLineLine, DiagonalIntersection)
{
	// L1: from (0,0,0) along (1,1,0); L2: from (2,0,0) along (-1,1,0)
	// They cross at (1,1,0)
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0, 1.0, 0.0));
	Line line2(Point(2.0, 0.0, 0.0), Vector(-1.0, 1.0, 0.0));

	auto result = intersect(line1, line2);

	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 1.0, 1.0, 0.0);
}

TEST(IntersectLineLine, ParallelLinesReturnNullopt)
{
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line line2(Point(0.0, 1.0, 0.0), Vector(1.0, 0.0, 0.0));

	auto result = intersect(line1, line2);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectLineLine, AntiParallelLinesReturnNullopt)
{
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line line2(Point(0.0, 1.0, 0.0), Vector(-1.0, 0.0, 0.0));

	auto result = intersect(line1, line2);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectLineLine, CoincidentLinesReturnNullopt)
{
	// Same line → direction cross product is zero → parallel case
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line line2(Point(3.0, 0.0, 0.0), Vector(2.0, 0.0, 0.0));

	auto result = intersect(line1, line2);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectLineLine, SkewLinesReturnNullopt)
{
	// L1: x-axis (z=0);  L2: offset by z=1, along y
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line line2(Point(0.0, 0.0, 1.0), Vector(0.0, 1.0, 0.0));

	auto result = intersect(line1, line2);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectLineLine, Skew3DLines)
{
	// L1: passes through (0,0,0) along (1,0,0)
	// L2: passes through (0,1,1) along (0,0,1) — offset in both y and z
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line line2(Point(0.0, 1.0, 1.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(line1, line2);

	EXPECT_FALSE(result.has_value());
}
