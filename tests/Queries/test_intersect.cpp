#include "Geometry/Bounds/bounding_box.hpp"
#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/ray.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Queries/intersect.hpp"
#include "Geometry/Shapes/triangle.hpp"

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

// ── Adversarial / robustness tests ───────────────────────────────────────────
//
// These tests verify scale-independence and translation-independence.
// Clean textbook inputs are not sufficient for a geometry library; these cover
// the cases most likely to expose tolerance bugs.

// ── Ray ∩ Plane (adversarial) ─────────────────────────────────────────────────

TEST(IntersectRayPlaneAdversarial, LargeCoordinateOriginHitsPlane)
{
	// Ray starts far from origin along z; direction is unit — must still intersect
	Ray ray(Point(0.0, 0.0, 1.0e7), Vector(0.0, 0.0, -1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(ray, plane);

	ASSERT_TRUE(result.has_value());
	EXPECT_NEAR(result->x(), 0.0, kTol);
	EXPECT_NEAR(result->y(), 0.0, kTol);
	EXPECT_NEAR(result->z(), 0.0, kTol);
}

TEST(IntersectRayPlaneAdversarial, ScaledDirectionGivesSameIntersection)
{
	// Scaling direction by 1e6 must not change the intersection point
	Ray ray(Point(0.0, 0.0, 1.0), Vector(0.0, 0.0, -1.0e6));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(ray, plane);

	ASSERT_TRUE(result.has_value());
	EXPECT_NEAR(result->z(), 0.0, kTol);
}

TEST(IntersectRayPlaneAdversarial, NearlyParallelRayWithLargeDirection)
{
	// Direction (1e6, 0, 1e-10): sin(angle with plane) = 1e-10 / 1e6 = 1e-16
	// Well below k_rel_eps → should be treated as parallel → nullopt
	Ray ray(Point(0.0, 0.0, 1.0), Vector(1.0e6, 0.0, 1.0e-10));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = intersect(ray, plane);

	EXPECT_FALSE(result.has_value());
}

// ── Line ∩ Line (adversarial) ─────────────────────────────────────────────────

TEST(IntersectLineLineAdversarial, IntersectingLinesTranslatedFarFromOrigin)
{
	// Same crossing geometry as a basic test, translated 1e6 along all axes
	double const offset = 1.0e6;
	Line line1(Point(offset, offset, offset), Vector(1.0, 0.0, 0.0));
	Line line2(Point(offset + 2.0, offset, offset), Vector(0.0, 1.0, 0.0));

	auto result = intersect(line1, line2);

	ASSERT_TRUE(result.has_value());
	EXPECT_NEAR(result->x(), offset + 2.0, kTol);
	EXPECT_NEAR(result->y(), offset, kTol);
	EXPECT_NEAR(result->z(), offset, kTol);
}

TEST(IntersectLineLineAdversarial, SkewLinesWithLargeCoordinates)
{
	// Same skew geometry (separation = 1) as a basic test, origins translated 1e6
	double const offset = 1.0e6;
	Line line1(Point(offset, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line line2(Point(offset, 0.0, 1.0), Vector(0.0, 1.0, 0.0));

	auto result = intersect(line1, line2);

	EXPECT_FALSE(result.has_value());
}

TEST(IntersectLineLineAdversarial, NearlyCoplanarLinesAcceptedAsIntersecting)
{
	// Two lines with sub-threshold skew distance (1e-12) must be treated as
	// intersecting. Skew_distance = 1e-12, |w| ≈ 1, relative ratio = 1e-12 << k_rel_eps.
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Line line2(Point(1.0, 0.0, 1.0e-12), Vector(0.0, 1.0, 0.0));

	// w = (1,0,1e-12), cross = (0,0,1)×... actually:
	// cross = (1,0,0)×(0,1,0) = (0,0,1), skew_num = w·cross = 1e-12
	// |cross| = 1, |w| ≈ 1 → relative ratio = 1e-12 << 1e-9 → intersects
	auto result = intersect(line1, line2);

	EXPECT_TRUE(result.has_value());
}

TEST(IntersectLineLineAdversarial, ScaledDirectionsGiveSameClassification)
{
	// Scaling both line directions by 1e3 must not change skew/intersect outcome
	Line line1(Point(0.0, 0.0, 0.0), Vector(1.0e3, 0.0, 0.0));
	Line line2(Point(0.0, 0.0, 1.0), Vector(0.0, 1.0e3, 0.0)); // skew by 1

	auto result = intersect(line1, line2);

	EXPECT_FALSE(result.has_value()); // still skew
}

// ── intersect(Ray, Triangle) ──────────────────────────────────────────────────
//
// Helper: unit triangle in the z = 0 plane, vertices at (0,0,0),(1,0,0),(0,1,0)

static Triangle xy_triangle()
{
	return Triangle(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
}

TEST(IntersectRayTriangle, RayHitsCentroid)
{
	// Ray from above straight down onto the centroid (1/3, 1/3, 0)
	Ray ray(Point(1.0 / 3.0, 1.0 / 3.0, 1.0), Vector(0.0, 0.0, -1.0));
	auto result = intersect(ray, xy_triangle());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 1.0 / 3.0, 1.0 / 3.0, 0.0);
}

TEST(IntersectRayTriangle, RayHitsVertex)
{
	// Ray aimed straight at p0 = (0,0,0)
	Ray ray(Point(0.0, 0.0, 1.0), Vector(0.0, 0.0, -1.0));
	auto result = intersect(ray, xy_triangle());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.0, 0.0);
}

TEST(IntersectRayTriangle, RayHitsEdgeMidpoint)
{
	// Midpoint of edge p0→p1 is (0.5, 0, 0)
	Ray ray(Point(0.5, 0.0, 1.0), Vector(0.0, 0.0, -1.0));
	auto result = intersect(ray, xy_triangle());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.5, 0.0, 0.0);
}

TEST(IntersectRayTriangle, RayMissesOutsideTriangle)
{
	// (1,1,z) is outside the unit triangle (u+v > 1)
	Ray ray(Point(1.0, 1.0, 1.0), Vector(0.0, 0.0, -1.0));
	auto result = intersect(ray, xy_triangle());
	EXPECT_FALSE(result.has_value());
}

TEST(IntersectRayTriangle, RayParallelToTrianglePlane)
{
	// Ray travels in the z = 1 plane — parallel to z = 0 triangle
	Ray ray(Point(0.0, 0.0, 1.0), Vector(1.0, 0.0, 0.0));
	auto result = intersect(ray, xy_triangle());
	EXPECT_FALSE(result.has_value());
}

TEST(IntersectRayTriangle, RayPointingAway)
{
	// Ray origin is above the triangle but direction points upward — misses
	Ray ray(Point(0.25, 0.25, 1.0), Vector(0.0, 0.0, 1.0));
	auto result = intersect(ray, xy_triangle());
	EXPECT_FALSE(result.has_value());
}

TEST(IntersectRayTriangle, RayFromBelowBackFaceHit)
{
	// Ray approaching from below (back face) — must still return a hit
	Ray ray(Point(1.0 / 3.0, 1.0 / 3.0, -1.0), Vector(0.0, 0.0, 1.0));
	auto result = intersect(ray, xy_triangle());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 1.0 / 3.0, 1.0 / 3.0, 0.0);
}

TEST(IntersectRayTriangle, ObliqueHit)
{
	// Ray from (0,0,1) along (1,0,-2): hits at t=0.5 → (0.5, 0, 0)
	Ray ray(Point(0.0, 0.0, 1.0), Vector(1.0, 0.0, -2.0));
	auto result = intersect(ray, xy_triangle());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.5, 0.0, 0.0);
}

TEST(IntersectRayTriangle, OffOriginTriangle)
{
	// Triangle far from origin; ray aimed at its centroid
	Triangle t(Point(10.0, 10.0, 5.0), Point(12.0, 10.0, 5.0), Point(10.0, 12.0, 5.0));
	Point centroid(11.0 + 1.0 / 3.0, 10.0 + 2.0 / 3.0, 5.0);
	// Adjust: centroid = ((10+12+10)/3, (10+10+12)/3, 5) = (32/3, 32/3, 5)
	Point c((10.0 + 12.0 + 10.0) / 3.0, (10.0 + 10.0 + 12.0) / 3.0, 5.0);
	Ray ray(c + Vector(0.0, 0.0, 3.0), Vector(0.0, 0.0, -1.0));
	auto result = intersect(ray, t);
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, c.x(), c.y(), 5.0);
}

TEST(IntersectRayTriangle, ScaledDirectionGivesSameHit)
{
	// Scaling the ray direction must not change the hit point
	Ray ray(Point(1.0 / 3.0, 1.0 / 3.0, 2.0), Vector(0.0, 0.0, -1.0e4));
	auto result = intersect(ray, xy_triangle());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 1.0 / 3.0, 1.0 / 3.0, 0.0);
}

// ── intersect(Ray, BoundingBox) ───────────────────────────────────────────────
//
// Helper: unit AABB [0,1]³

static BoundingBox unit_aabb()
{
	return BoundingBox(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
}

TEST(IntersectRayAABB, RayHitsFrontFace)
{
	// Ray along +x, starts outside; enters at x=0, face centre (0, 0.5, 0.5)
	Ray ray(Point(-1.0, 0.5, 0.5), Vector(1.0, 0.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.5, 0.5);
}

TEST(IntersectRayAABB, RayHitsBackFace)
{
	// Ray along -x from the positive side; enters at x=1
	Ray ray(Point(2.0, 0.5, 0.5), Vector(-1.0, 0.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 1.0, 0.5, 0.5);
}

TEST(IntersectRayAABB, RayOriginInsideBox)
{
	// Origin inside: entry is clamped to t=0, returned point is the origin
	Ray ray(Point(0.5, 0.5, 0.5), Vector(1.0, 0.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.5, 0.5, 0.5);
}

TEST(IntersectRayAABB, RayOriginOnFace)
{
	// Origin exactly on the min-x face; direction points inward
	Ray ray(Point(0.0, 0.5, 0.5), Vector(1.0, 0.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.5, 0.5);
}

TEST(IntersectRayAABB, RayMissesBesideBox)
{
	// Ray parallel to x-axis but offset in y — misses entirely
	Ray ray(Point(-1.0, 2.0, 0.5), Vector(1.0, 0.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	EXPECT_FALSE(result.has_value());
}

TEST(IntersectRayAABB, RayPointingAway)
{
	// Ray origin outside, direction points away from the box
	Ray ray(Point(-1.0, 0.5, 0.5), Vector(-1.0, 0.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	EXPECT_FALSE(result.has_value());
}

TEST(IntersectRayAABB, RayGrazesEdge)
{
	// Ray aimed exactly at the edge between x=1 and y=1 faces
	// Entry point is the corner (1,1,0.5) — touching counts as a hit
	Ray ray(Point(2.0, 2.0, 0.5), Vector(-1.0, -1.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 1.0, 1.0, 0.5);
}

TEST(IntersectRayAABB, RayParallelToFaceInsideSlab)
{
	// Ray at y=0.5, z=0.5, travels along x — inside y and z slabs, hits box
	Ray ray(Point(-2.0, 0.5, 0.5), Vector(1.0, 0.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.5, 0.5);
}

TEST(IntersectRayAABB, RayParallelToFaceOutsideSlab)
{
	// Ray at y=2.0 — outside the y slab, parallel → miss
	Ray ray(Point(-2.0, 2.0, 0.5), Vector(1.0, 0.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	EXPECT_FALSE(result.has_value());
}

TEST(IntersectRayAABB, DiagonalHit)
{
	// Ray from (-1,-1,-1) along (1,1,1) — enters at (0,0,0)
	Ray ray(Point(-1.0, -1.0, -1.0), Vector(1.0, 1.0, 1.0));
	auto result = intersect(ray, unit_aabb());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.0, 0.0);
}

TEST(IntersectRayAABB, OffOriginBox)
{
	// Box at [5,7]³; ray aimed at centre from outside
	BoundingBox box(Point(5.0, 5.0, 5.0), Point(7.0, 7.0, 7.0));
	Ray ray(Point(3.0, 6.0, 6.0), Vector(1.0, 0.0, 0.0));
	auto result = intersect(ray, box);
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 5.0, 6.0, 6.0);
}

TEST(IntersectRayAABB, ScaledDirectionGivesSameHit)
{
	// Scaling the direction by 1e4 must not change the hit point
	Ray ray(Point(-1.0, 0.5, 0.5), Vector(1.0e4, 0.0, 0.0));
	auto result = intersect(ray, unit_aabb());
	ASSERT_TRUE(result.has_value());
	ExpectPointEq(*result, 0.0, 0.5, 0.5);
}
