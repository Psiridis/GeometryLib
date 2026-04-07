#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Shapes/triangle.hpp"

#include <gtest/gtest.h>

#include <cmath>

using namespace Geometry;

static constexpr double kTol = 1e-9;

// ── Helpers ───────────────────────────────────────────────────────────────────

static Triangle unit_xy()
{
	// Right-angle triangle in the z=0 plane with vertices at (0,0,0),(1,0,0),(0,1,0)
	return Triangle(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
}

// ── Construction ──────────────────────────────────────────────────────────────

TEST(TriangleConstruction, ValidVertices)
{
	Triangle t = unit_xy();
	EXPECT_EQ(t.p0(), Point(0.0, 0.0, 0.0));
	EXPECT_EQ(t.p1(), Point(1.0, 0.0, 0.0));
	EXPECT_EQ(t.p2(), Point(0.0, 1.0, 0.0));
}

TEST(TriangleConstruction, CollinearPointsAborts)
{
	// Three points on the x-axis → collinear → abort
	EXPECT_DEATH(Triangle(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(2.0, 0.0, 0.0)), "");
}

TEST(TriangleConstruction, RepeatedPointAborts)
{
	EXPECT_DEATH(Triangle(Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0)), "");
}

// ── normal ────────────────────────────────────────────────────────────────────

TEST(TriangleNormal, UnitLength)
{
	Triangle t = unit_xy();
	EXPECT_NEAR(t.normal().length(), 1.0, kTol);
}

TEST(TriangleNormal, WindingOrderRightHandRule)
{
	// p0→p1→p2 counter-clockwise in z=0 → normal points +z
	Triangle t = unit_xy();
	EXPECT_NEAR(t.normal().dx(), 0.0, kTol);
	EXPECT_NEAR(t.normal().dy(), 0.0, kTol);
	EXPECT_NEAR(t.normal().dz(), 1.0, kTol);
}

TEST(TriangleNormal, ClockwiseWindingNegativeZ)
{
	// Reverse p1 and p2 → normal flips to -z
	Triangle t(Point(0.0, 0.0, 0.0), Point(0.0, 1.0, 0.0), Point(1.0, 0.0, 0.0));
	EXPECT_NEAR(t.normal().dz(), -1.0, kTol);
}

TEST(TriangleNormal, ObliqueTriangle)
{
	// Triangle in x=0 plane → normal along ±x
	Triangle t(Point(0.0, 0.0, 0.0), Point(0.0, 1.0, 0.0), Point(0.0, 0.0, 1.0));
	EXPECT_NEAR(std::abs(t.normal().dx()), 1.0, kTol);
	EXPECT_NEAR(t.normal().dy(), 0.0, kTol);
	EXPECT_NEAR(t.normal().dz(), 0.0, kTol);
}

// ── area ─────────────────────────────────────────────────────────────────────

TEST(TriangleArea, UnitRightAngleTriangle)
{
	// Area = 0.5 * base * height = 0.5
	EXPECT_NEAR(unit_xy().area(), 0.5, kTol);
}

TEST(TriangleArea, KnownEquilateral)
{
	// Equilateral with side 2: area = sqrt(3)
	Triangle t(Point(0.0, 0.0, 0.0), Point(2.0, 0.0, 0.0), Point(1.0, std::sqrt(3.0), 0.0));
	EXPECT_NEAR(t.area(), std::sqrt(3.0), kTol);
}

TEST(TriangleArea, ScaledTriangle)
{
	// Scale unit triangle by 3 → area × 9 = 4.5
	Triangle t(Point(0.0, 0.0, 0.0), Point(3.0, 0.0, 0.0), Point(0.0, 3.0, 0.0));
	EXPECT_NEAR(t.area(), 4.5, kTol);
}

// ── centroid ──────────────────────────────────────────────────────────────────

TEST(TriangleCentroid, UnitRightAngle)
{
	// Centroid = (0+1+0)/3, (0+0+1)/3, 0 = (1/3, 1/3, 0)
	Point c = unit_xy().centroid();
	EXPECT_NEAR(c.x(), 1.0 / 3.0, kTol);
	EXPECT_NEAR(c.y(), 1.0 / 3.0, kTol);
	EXPECT_NEAR(c.z(), 0.0, kTol);
}

TEST(TriangleCentroid, SymmetricTriangle)
{
	// Symmetric about origin → centroid at origin
	Triangle t(Point(-1.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
	Point c = t.centroid();
	EXPECT_NEAR(c.x(), 0.0, kTol);
	EXPECT_NEAR(c.y(), 1.0 / 3.0, kTol);
	EXPECT_NEAR(c.z(), 0.0, kTol);
}

// ── perimeter ────────────────────────────────────────────────────────────────

TEST(TrianglePerimeter, UnitRightAngle)
{
	// legs: 1, 1; hypotenuse: sqrt(2)
	EXPECT_NEAR(unit_xy().perimeter(), 2.0 + std::sqrt(2.0), kTol);
}

TEST(TrianglePerimeter, Equilateral)
{
	// All sides = 2 → perimeter = 6
	Triangle t(Point(0.0, 0.0, 0.0), Point(2.0, 0.0, 0.0), Point(1.0, std::sqrt(3.0), 0.0));
	EXPECT_NEAR(t.perimeter(), 6.0, kTol);
}

// ── plane ─────────────────────────────────────────────────────────────────────

TEST(TrianglePlane, AllVerticesOnPlane)
{
	Triangle t = unit_xy();
	Plane p		 = t.plane();
	EXPECT_TRUE(p.contains(t.p0()));
	EXPECT_TRUE(p.contains(t.p1()));
	EXPECT_TRUE(p.contains(t.p2()));
}

TEST(TrianglePlane, NormalMatchesTriangle)
{
	Triangle t = unit_xy();
	Plane p		 = t.plane();
	// Cross product of plane and triangle normals must be zero (same direction)
	EXPECT_NEAR(p.normal().cross(t.normal()).length(), 0.0, kTol);
}

// ── contains ─────────────────────────────────────────────────────────────────

TEST(TriangleContains, Centroid)
{
	Triangle t = unit_xy();
	EXPECT_TRUE(t.contains(t.centroid()));
}

TEST(TriangleContains, Vertices)
{
	Triangle t = unit_xy();
	EXPECT_TRUE(t.contains(t.p0()));
	EXPECT_TRUE(t.contains(t.p1()));
	EXPECT_TRUE(t.contains(t.p2()));
}

TEST(TriangleContains, PointOnEdge)
{
	Triangle t = unit_xy();
	// Midpoint of p0→p1 edge
	EXPECT_TRUE(t.contains(Point(0.5, 0.0, 0.0)));
}

TEST(TriangleContains, PointOutsideEdge)
{
	Triangle t = unit_xy();
	EXPECT_FALSE(t.contains(Point(1.0, 1.0, 0.0)));
}

TEST(TriangleContains, PointFarOutside)
{
	Triangle t = unit_xy();
	EXPECT_FALSE(t.contains(Point(5.0, 5.0, 0.0)));
}

TEST(TriangleContains, PointBehindOrigin)
{
	Triangle t = unit_xy();
	EXPECT_FALSE(t.contains(Point(-1.0, 0.5, 0.0)));
}

// ── operator== / operator!= ───────────────────────────────────────────────────

TEST(TriangleEquality, SameTriangle)
{
	Triangle a = unit_xy();
	Triangle b = unit_xy();
	EXPECT_TRUE(a == b);
	EXPECT_FALSE(a != b);
}

TEST(TriangleEquality, DifferentVertex)
{
	Triangle a = unit_xy();
	Triangle b(Point(0.0, 0.0, 0.0), Point(2.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a != b);
}

TEST(TriangleEquality, PermutedVerticesNotEqual)
{
	// Vertex order matters — different winding is a different triangle
	Triangle a(Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0));
	Triangle b(Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0), Point(0.0, 0.0, 0.0));
	EXPECT_FALSE(a == b);
}
