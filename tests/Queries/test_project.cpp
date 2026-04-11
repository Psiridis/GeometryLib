#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/ray.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Queries/project.hpp"
#include "Geometry/Shapes/segment.hpp"

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

// ── project(Point, Line) ──────────────────────────────────────────────────────

TEST(ProjectPointLine, PointOnLine)
{
	// A point already on the line projects to itself
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(3.0, 0.0, 0.0);

	ExpectPointEq(project(p, line), 3.0, 0.0, 0.0);
}

TEST(ProjectPointLine, PointAboveLine)
{
	// Point (0, 3, 0) projected onto the x-axis → (0, 0, 0)
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(0.0, 3.0, 0.0);

	ExpectPointEq(project(p, line), 0.0, 0.0, 0.0);
}

TEST(ProjectPointLine, PointOffLineWithOffset)
{
	// Point (4, 3, 0) onto x-axis → (4, 0, 0)
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(4.0, 3.0, 0.0);

	ExpectPointEq(project(p, line), 4.0, 0.0, 0.0);
}

TEST(ProjectPointLine, PointBehindOrigin)
{
	// Point (-2, 1, 0) onto x-axis → (-2, 0, 0); negative t is valid for a Line
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(-2.0, 1.0, 0.0);

	ExpectPointEq(project(p, line), -2.0, 0.0, 0.0);
}

TEST(ProjectPointLine, NonUnitDirection)
{
	// Direction (2, 0, 0) — not normalised; result must be identical
	Line line(Point(0.0, 0.0, 0.0), Vector(2.0, 0.0, 0.0));
	Point p(4.0, 3.0, 0.0);

	ExpectPointEq(project(p, line), 4.0, 0.0, 0.0);
}

TEST(ProjectPointLine, DiagonalLine)
{
	// Line along (1,1,0), point at (2, 0, 0)
	// t = (2,0,0)·(1,1,0) / |(1,1,0)|² = 2 / 2 = 1  → foot = (1, 1, 0)
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 1.0, 0.0));
	Point p(2.0, 0.0, 0.0);

	ExpectPointEq(project(p, line), 1.0, 1.0, 0.0);
}

TEST(ProjectPointLine, ProjectedPointLiesOnLine)
{
	// The foot must satisfy the line equation for some t
	Line line(Point(1.0, 2.0, 3.0), Vector(1.0, 2.0, 2.0));
	Point p(5.0, 0.0, 1.0);

	Point foot = project(p, line);

	// (foot - origin) must be parallel to direction → cross product ≈ 0
	Vector diff	 = foot - line.origin();
	Vector cross = diff.cross(line.direction());
	EXPECT_NEAR(cross.length(), 0.0, kTol);
}

TEST(ProjectPointLine, FootIsClosestPointOnLine)
{
	// Any other point on the line must be further from p
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(3.0, 4.0, 0.0);

	Point foot			 = project(p, line);
	double dist_foot = p.distance(foot);

	for (double t : { -2.0, 1.0, 2.0, 4.0, 5.0 })
	{
		EXPECT_LE(dist_foot, p.distance(line.at(t)) + kTol);
	}
}

// ── project(Point, Plane) ─────────────────────────────────────────────────────

TEST(ProjectPointPlane, PointOnPlane)
{
	// A point already on the plane projects to itself
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Point p(3.0, 4.0, 0.0);

	ExpectPointEq(project(p, plane), 3.0, 4.0, 0.0);
}

TEST(ProjectPointPlane, PointAbovePlane)
{
	// Point (0, 0, 5) onto z=0 plane → (0, 0, 0)
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Point p(0.0, 0.0, 5.0);

	ExpectPointEq(project(p, plane), 0.0, 0.0, 0.0);
}

TEST(ProjectPointPlane, PointBelowPlane)
{
	// Point (0, 0, -3) onto z=0 plane → (0, 0, 0)
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Point p(0.0, 0.0, -3.0);

	ExpectPointEq(project(p, plane), 0.0, 0.0, 0.0);
}

TEST(ProjectPointPlane, PointOffsetWithXY)
{
	// Point (2, 3, 7) onto z=0 plane → (2, 3, 0)
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Point p(2.0, 3.0, 7.0);

	ExpectPointEq(project(p, plane), 2.0, 3.0, 0.0);
}

TEST(ProjectPointPlane, NonOriginPlane)
{
	// Plane at z=5 with upward normal; point (1, 2, 8) → (1, 2, 5)
	Plane plane(Point(0.0, 0.0, 5.0), Vector(0.0, 0.0, 1.0));
	Point p(1.0, 2.0, 8.0);

	ExpectPointEq(project(p, plane), 1.0, 2.0, 5.0);
}

TEST(ProjectPointPlane, ObliqueNormal)
{
	// Plane through origin with normal (1,0,0); point (5, 3, 2) → (0, 3, 2)
	Plane plane(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(5.0, 3.0, 2.0);

	ExpectPointEq(project(p, plane), 0.0, 3.0, 2.0);
}

TEST(ProjectPointPlane, ProjectedPointLiesOnPlane)
{
	Plane plane(Point(1.0, 2.0, 3.0), Vector(1.0, 1.0, 1.0));
	Point p(5.0, 0.0, 1.0);

	Point foot = project(p, plane);
	EXPECT_TRUE(plane.contains(foot));
}

TEST(ProjectPointPlane, FootIsClosestPointOnPlane)
{
	// The foot must have minimum distance — any point on the plane is further
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Point p(1.0, 1.0, 4.0);

	Point foot			 = project(p, plane);
	double dist_foot = p.distance(foot);

	// Sample other points on the plane
	for (auto [x, y] : std::initializer_list<std::pair<double, double>>{
			 { 0.0, 0.0 }, { 3.0, 0.0 }, { 0.0, 3.0 }, { -2.0, 5.0 } })
	{
		EXPECT_LE(dist_foot, p.distance(Point(x, y, 0.0)) + kTol);
	}
}

// ── project(Point, Ray) ───────────────────────────────────────────────────────

TEST(ProjectPointRay, PointOnRay)
{
	// A point already on the ray projects to itself
	Ray ray(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(3.0, 0.0, 0.0);

	ExpectPointEq(project(p, ray), 3.0, 0.0, 0.0);
}

TEST(ProjectPointRay, PointAboveRay)
{
	// Point (4, 3, 0) onto x-axis ray → (4, 0, 0) — t > 0, same as line
	Ray ray(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(4.0, 3.0, 0.0);

	ExpectPointEq(project(p, ray), 4.0, 0.0, 0.0);
}

TEST(ProjectPointRay, PointBehindOriginClampsToOrigin)
{
	// Foot would be at t = -2; must clamp to origin
	Ray ray(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(-2.0, 1.0, 0.0);

	ExpectPointEq(project(p, ray), 0.0, 0.0, 0.0);
}

TEST(ProjectPointRay, PointExactlyAtOriginPerpendicular)
{
	// Point directly above origin — t = 0, clamps to origin
	Ray ray(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(0.0, 5.0, 0.0);

	ExpectPointEq(project(p, ray), 0.0, 0.0, 0.0);
}

TEST(ProjectPointRay, NonUnitDirection)
{
	// Direction (3, 0, 0) — result same as unit direction
	Ray ray(Point(0.0, 0.0, 0.0), Vector(3.0, 0.0, 0.0));
	Point p(4.0, 2.0, 0.0);

	ExpectPointEq(project(p, ray), 4.0, 0.0, 0.0);
}

TEST(ProjectPointRay, FootIsClosestPointOnRay)
{
	Ray ray(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	Point p(3.0, 4.0, 0.0);

	Point foot			 = project(p, ray);
	double dist_foot = p.distance(foot);

	for (double t : { 0.0, 1.0, 2.0, 4.0, 5.0 })
	{
		EXPECT_LE(dist_foot, p.distance(ray.origin() + t * ray.direction()) + kTol);
	}
}

// ── project(Line, Plane) ──────────────────────────────────────────────────────

TEST(ProjectLinePlane, LineParallelToPlane)
{
	// Line along x at z=3; projection onto z=0 plane → same direction, origin at z=0
	Line line(Point(0.0, 0.0, 3.0), Vector(1.0, 0.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = project(line, plane);

	ASSERT_TRUE(result.has_value());
	EXPECT_TRUE(plane.contains(result->origin()));
	// Direction must stay along x
	Vector cross = result->direction().cross(Vector(1.0, 0.0, 0.0));
	EXPECT_NEAR(cross.length(), 0.0, kTol);
}

TEST(ProjectLinePlane, LineOnPlane)
{
	// Line already on the plane — projects to itself
	Line line(Point(1.0, 2.0, 0.0), Vector(1.0, 1.0, 0.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = project(line, plane);

	ASSERT_TRUE(result.has_value());
	EXPECT_TRUE(plane.contains(result->origin()));
	Vector cross = result->direction().cross(line.direction());
	EXPECT_NEAR(cross.length(), 0.0, kTol);
}

TEST(ProjectLinePlane, ObliqueLineProjected)
{
	// Line from (0,0,0) along (1,0,1) — oblique to z=0 plane
	// Projected direction: (1,0,1) - ((1,0,1)·(0,0,1))*(0,0,1) = (1,0,0)
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = project(line, plane);

	ASSERT_TRUE(result.has_value());
	EXPECT_TRUE(plane.contains(result->origin()));
	Vector cross = result->direction().cross(Vector(1.0, 0.0, 0.0));
	EXPECT_NEAR(cross.length(), 0.0, kTol);
}

TEST(ProjectLinePlane, ProjectedOriginLiesOnPlane)
{
	// Any projected line's origin must lie on the plane
	Line line(Point(3.0, 1.0, 7.0), Vector(1.0, 2.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));

	auto result = project(line, plane);

	ASSERT_TRUE(result.has_value());
	EXPECT_TRUE(plane.contains(result->origin()));
}

TEST(ProjectLinePlane, ProjectedLineDirectionLiesInPlane)
{
	// Projected direction must be perpendicular to the plane normal
	Line line(Point(0.0, 0.0, 0.0), Vector(1.0, 2.0, 3.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = project(line, plane);

	ASSERT_TRUE(result.has_value());
	// d' · n must be ≈ 0
	EXPECT_NEAR(result->direction().dot(plane.normal()), 0.0, kTol);
}

TEST(ProjectLinePlane, PerpendicularLineReturnsNullopt)
{
	// Line along z — perpendicular to the z=0 plane → collapses to a point
	Line line(Point(1.0, 2.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	auto result = project(line, plane);

	EXPECT_FALSE(result.has_value());
}

// ── project(Vector, Plane) ────────────────────────────────────────────────────

TEST(ProjectVectorPlane, VectorParallelToPlane)
{
	// (1,0,0) has no z component — projection onto z=0 is unchanged
	Vector vec(1.0, 0.0, 0.0);
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	Vector result = project(vec, plane);

	EXPECT_NEAR(result.dx(), 1.0, kTol);
	EXPECT_NEAR(result.dy(), 0.0, kTol);
	EXPECT_NEAR(result.dz(), 0.0, kTol);
}

TEST(ProjectVectorPlane, VectorPerpendicularToPlane)
{
	// (0,0,1) is entirely along z — projection onto z=0 is zero vector
	Vector vec(0.0, 0.0, 1.0);
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	Vector result = project(vec, plane);

	EXPECT_NEAR(result.dx(), 0.0, kTol);
	EXPECT_NEAR(result.dy(), 0.0, kTol);
	EXPECT_NEAR(result.dz(), 0.0, kTol);
}

TEST(ProjectVectorPlane, ObliqueVectorProjected)
{
	// (1,0,1): normal component (0,0,1), tangential → (1,0,0)
	Vector vec(1.0, 0.0, 1.0);
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	Vector result = project(vec, plane);

	EXPECT_NEAR(result.dx(), 1.0, kTol);
	EXPECT_NEAR(result.dy(), 0.0, kTol);
	EXPECT_NEAR(result.dz(), 0.0, kTol);
}

TEST(ProjectVectorPlane, ResultIsPerpendicularToNormal)
{
	// For any vector the result must satisfy result·n ≈ 0
	Vector vec(3.0, 1.0, 2.0);
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	Vector result = project(vec, plane);

	EXPECT_NEAR(result.dot(plane.normal()), 0.0, kTol);
}

TEST(ProjectVectorPlane, GeneralVector)
{
	// (1,2,3) onto z=0 → (1,2,0)
	Vector vec(1.0, 2.0, 3.0);
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	Vector result = project(vec, plane);

	EXPECT_NEAR(result.dx(), 1.0, kTol);
	EXPECT_NEAR(result.dy(), 2.0, kTol);
	EXPECT_NEAR(result.dz(), 0.0, kTol);
}

TEST(ProjectVectorPlane, NonAxisAlignedPlane)
{
	// Plane with normal (1,1,0)/sqrt(2): project (1,0,0)
	// v·n = 1/sqrt(2), result = (1,0,0) - (1/sqrt(2))*(1/sqrt(2),1/sqrt(2),0)
	//                         = (1 - 0.5, 0 + 0.5, 0) = (0.5, 0.5, 0) — wait
	// Actually n is NOT unit here; Plane stores unit normal.
	// Use Plane with unit normal (1,1,0)/sqrt(2).
	double const inv_sqrt2 = 1.0 / std::sqrt(2.0);
	Vector vec(1.0, 0.0, 0.0);
	Plane plane(Point(0.0, 0.0, 0.0), Vector(inv_sqrt2, inv_sqrt2, 0.0));

	Vector result = project(vec, plane);

	// v·n = inv_sqrt2; result = (1,0,0) - inv_sqrt2*(inv_sqrt2,inv_sqrt2,0)
	//                         = (1 - 0.5, 0 - 0.5, 0) = (0.5, -0.5, 0)
	EXPECT_NEAR(result.dx(), 0.5, kTol);
	EXPECT_NEAR(result.dy(), -0.5, kTol);
	EXPECT_NEAR(result.dz(), 0.0, kTol);
	EXPECT_NEAR(result.dot(plane.normal()), 0.0, kTol);
}

// ── project(Point, Segment) ──────────────────────────────────────────────────

TEST(ProjectPointSegment, PointOnSegment)
{
	// Point already on the segment projects to itself
	Segment seg(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point p(2.0, 0.0, 0.0);

	ExpectPointEq(project(p, seg), 2.0, 0.0, 0.0);
}

TEST(ProjectPointSegment, PointPerpendicularToMidpoint)
{
	// Foot of perpendicular from (2, 3, 0) onto the x-axis segment [0,4]
	Segment seg(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point p(2.0, 3.0, 0.0);

	ExpectPointEq(project(p, seg), 2.0, 0.0, 0.0);
}

TEST(ProjectPointSegment, PointBeyondEnd)
{
	// Project clamps to the end point when t > 1
	Segment seg(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point p(10.0, 2.0, 0.0);

	ExpectPointEq(project(p, seg), 4.0, 0.0, 0.0);
}

TEST(ProjectPointSegment, PointBeforeStart)
{
	// Project clamps to the start point when t < 0
	Segment seg(Point(0.0, 0.0, 0.0), Point(4.0, 0.0, 0.0));
	Point p(-5.0, 2.0, 0.0);

	ExpectPointEq(project(p, seg), 0.0, 0.0, 0.0);
}

TEST(ProjectPointSegment, PointAtStartIsStart)
{
	Segment seg(Point(1.0, 2.0, 3.0), Point(5.0, 2.0, 3.0));
	Point p(1.0, 2.0, 3.0);

	ExpectPointEq(project(p, seg), 1.0, 2.0, 3.0);
}

TEST(ProjectPointSegment, DiagonalSegment)
{
	// Segment from (0,0,0) to (1,1,0); project (1,0,0)
	// d=(1,1,0), w=(1,0,0), t = (1·1+0·1+0) / 2 = 0.5
	// foot = (0.5, 0.5, 0)
	Segment seg(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 0.0));
	Point p(1.0, 0.0, 0.0);

	ExpectPointEq(project(p, seg), 0.5, 0.5, 0.0);
}
