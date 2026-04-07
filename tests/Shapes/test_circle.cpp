#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Shapes/circle.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <numbers>

using namespace Geometry;

static constexpr double kTol = 1e-9;

// ── Helpers ───────────────────────────────────────────────────────────────────

static Circle unit_xy()
{
	// Unit circle in z=0 plane centered at origin
	return Circle(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 1.0);
}

// ── Construction ──────────────────────────────────────────────────────────────

TEST(CircleConstruction, ValidCircle)
{
	Circle c = unit_xy();
	EXPECT_EQ(c.center(), Point(0.0, 0.0, 0.0));
	EXPECT_NEAR(c.radius(), 1.0, kTol);
	EXPECT_NEAR(c.normal().length(), 1.0, kTol); // normal is unit
}

TEST(CircleConstruction, NonUnitNormalIsNormalized)
{
	Circle c(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 5.0), 2.0);
	EXPECT_NEAR(c.normal().length(), 1.0, kTol);
	EXPECT_NEAR(c.normal().dz(), 1.0, kTol);
}

TEST(CircleConstruction, ZeroNormalAborts)
{
	EXPECT_DEATH(Circle(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 0.0), 1.0), "");
}

TEST(CircleConstruction, ZeroRadiusAborts)
{
	EXPECT_DEATH(Circle(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 0.0), "");
}

TEST(CircleConstruction, NegativeRadiusAborts)
{
	EXPECT_DEATH(Circle(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), -1.0), "");
}

// ── area / circumference ──────────────────────────────────────────────────────

TEST(CircleArea, UnitCircle)
{
	EXPECT_NEAR(unit_xy().area(), std::numbers::pi, kTol);
}

TEST(CircleArea, RadiusTwo)
{
	Circle c(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 2.0);
	EXPECT_NEAR(c.area(), 4.0 * std::numbers::pi, kTol);
}

TEST(CircleCircumference, UnitCircle)
{
	EXPECT_NEAR(unit_xy().circumference(), 2.0 * std::numbers::pi, kTol);
}

TEST(CircleCircumference, RadiusThree)
{
	Circle c(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 3.0);
	EXPECT_NEAR(c.circumference(), 6.0 * std::numbers::pi, kTol);
}

// ── plane ─────────────────────────────────────────────────────────────────────

TEST(CirclePlane, CenterLiesOnPlane)
{
	Circle c = unit_xy();
	EXPECT_TRUE(c.plane().contains(c.center()));
}

TEST(CirclePlane, NormalMatchesCircle)
{
	Circle c = unit_xy();
	EXPECT_NEAR(c.plane().normal().cross(c.normal()).length(), 0.0, kTol);
}

TEST(CirclePlane, OffsetCenter)
{
	Circle c(Point(3.0, 1.0, 2.0), Vector(0.0, 1.0, 0.0), 1.0);
	EXPECT_TRUE(c.plane().contains(c.center()));
}

// ── contains ─────────────────────────────────────────────────────────────────

TEST(CircleContains, CenterPoint)
{
	EXPECT_TRUE(unit_xy().contains(Point(0.0, 0.0, 0.0)));
}

TEST(CircleContains, PointOnBoundary)
{
	// Exactly on the circumference
	EXPECT_TRUE(unit_xy().contains(Point(1.0, 0.0, 0.0)));
	EXPECT_TRUE(unit_xy().contains(Point(0.0, 1.0, 0.0)));
}

TEST(CircleContains, PointInsideDisc)
{
	EXPECT_TRUE(unit_xy().contains(Point(0.5, 0.5, 0.0)));
}

TEST(CircleContains, PointOutsideRadius)
{
	EXPECT_FALSE(unit_xy().contains(Point(1.5, 0.0, 0.0)));
}

TEST(CircleContains, PointAbovePlaneRejected)
{
	// Coplanar check: point at (0,0,0.1) is not on the z=0 plane
	EXPECT_FALSE(unit_xy().contains(Point(0.0, 0.0, 0.1)));
}

TEST(CircleContains, PointProjectsWithinButOffPlane)
{
	// (0,0,0.5) projects to center but is above the plane → false
	EXPECT_FALSE(unit_xy().contains(Point(0.0, 0.0, 0.5)));
}

TEST(CircleContains, OffsetCircle)
{
	Circle c(Point(5.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 2.0);
	EXPECT_TRUE(c.contains(Point(5.0, 0.0, 0.0)));	// center
	EXPECT_TRUE(c.contains(Point(7.0, 0.0, 0.0)));	// on boundary
	EXPECT_FALSE(c.contains(Point(8.0, 0.0, 0.0))); // outside
}

// ── operator== / operator!= ───────────────────────────────────────────────────

TEST(CircleEquality, SameCircle)
{
	Circle a = unit_xy();
	Circle b = unit_xy();
	EXPECT_TRUE(a == b);
	EXPECT_FALSE(a != b);
}

TEST(CircleEquality, DifferentRadius)
{
	Circle a(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 1.0);
	Circle b(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 2.0);
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a != b);
}

TEST(CircleEquality, DifferentCenter)
{
	Circle a(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 1.0);
	Circle b(Point(1.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 1.0);
	EXPECT_FALSE(a == b);
}

TEST(CircleEquality, OppositeNormalsNotEqual)
{
	Circle a(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 1.0);
	Circle b(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, -1.0), 1.0);
	EXPECT_FALSE(a == b);
}
