#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>

using namespace Geometry;

TEST(PlaneTest, ConstructFromPointAndNormal)
{
	Point point(0.0, 0.0, 0.0);
	Vector normal(0.0, 0.0, 2.0);

	Plane plane(point, normal);

	EXPECT_TRUE(plane.normal().isValidDirection());
	EXPECT_DOUBLE_EQ(plane.normal().length(), 1.0);
}

TEST(PlaneTest, ConstructFromThreeNonCollinearPoints)
{
	Point p1(0.0, 0.0, 0.0);
	Point p2(1.0, 0.0, 0.0);
	Point p3(0.0, 1.0, 0.0);

	Plane plane(p1, p2, p3);

	EXPECT_TRUE(plane.normal().isValidDirection());
	EXPECT_DOUBLE_EQ(plane.normal().length(), 1.0);
	EXPECT_TRUE(plane.contains(p1));
	EXPECT_TRUE(plane.contains(p2));
	EXPECT_TRUE(plane.contains(p3));
}

TEST(PlaneTest, ConstructFromInvalidNormalThrows)
{
	Point point(0.0, 0.0, 0.0);
	Vector normal(0.0, 0.0, 0.0);
#ifdef NDEBUG
	EXPECT_THROW(Plane(point, normal), std::invalid_argument);
#else
	EXPECT_DEATH(Plane(point, normal), "GEOM_ASSERT");
#endif
}

TEST(PlaneTest, ConstructFromCollinearPointsThrows)
{
	Point p1(0.0, 0.0, 0.0);
	Point p2(1.0, 1.0, 1.0);
	Point p3(2.0, 2.0, 2.0);
#ifdef NDEBUG
	EXPECT_THROW(Plane(p1, p2, p3), std::invalid_argument);
#else
	EXPECT_DEATH(Plane(p1, p2, p3), "GEOM_ASSERT");
#endif
}

TEST(PlaneTest, NormalAccessorReturnsExpectedValue)
{
	Point point(1.0, 2.0, 3.0);
	Vector normal(0.0, 0.0, 1.0);

	Plane plane(point, normal);

	EXPECT_TRUE(plane.normal() == Vector(0.0, 0.0, 1.0));
}

TEST(PlaneTest, ConstructionPointIsOnPlane)
{
	Point point(1.0, 2.0, 3.0);
	Vector normal(0.0, 0.0, 1.0);

	Plane plane(point, normal);

	EXPECT_TRUE(plane.contains(point));
}

TEST(PlaneTest, SignedDistanceForXYPlane)
{
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_DOUBLE_EQ(plane.signed_distance(Point(1.0, 2.0, 5.0)), 5.0);
	EXPECT_DOUBLE_EQ(plane.signed_distance(Point(1.0, 2.0, -3.0)), -3.0);
}

TEST(PlaneTest, DistanceIsAbsoluteSignedDistance)
{
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));

	EXPECT_DOUBLE_EQ(plane.distance(Point(0.0, 0.0, 4.0)), 4.0);
	EXPECT_DOUBLE_EQ(plane.distance(Point(0.0, 0.0, -4.0)), 4.0);
}

TEST(PlaneTest, ContainsPointOnPlane)
{
	Plane plane(Point(0.0, 0.0, 2.0), Vector(0.0, 0.0, 1.0));

	EXPECT_TRUE(plane.contains(Point(1.0, 2.0, 2.0)));
}

TEST(PlaneTest, DoesNotContainPointOffPlane)
{
	Plane plane(Point(0.0, 0.0, 2.0), Vector(0.0, 0.0, 1.0));

	EXPECT_FALSE(plane.contains(Point(1.0, 2.0, 2.5)));
}

TEST(PlaneTest, CopyConstructor)
{
	Plane p1(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane p2 = p1;
	EXPECT_TRUE(p2.normal() == p1.normal());
}

TEST(PlaneTest, CopyAssignment)
{
	Plane p1(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Plane p2(Point(1.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	p2 = p1;
	EXPECT_TRUE(p2.normal() == p1.normal());
}

TEST(PlaneTest, MoveConstructor)
{
	Plane p1(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Vector const expected_normal = p1.normal();
	Plane p2										 = std::move(p1);
	EXPECT_TRUE(p2.normal() == expected_normal);
}

TEST(PlaneTest, MoveAssignment)
{
	Plane p1(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	Vector const expected_normal = p1.normal();
	Plane p2(Point(1.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	p2 = std::move(p1);
	EXPECT_TRUE(p2.normal() == expected_normal);
}

TEST(PlaneTest, ObliqueNormalIsNormalized)
{
	// Non-axis-aligned normal — must still be unit length after construction
	Plane plane(Point(1.0, 1.0, 1.0), Vector(1.0, 1.0, 1.0));
	EXPECT_DOUBLE_EQ(plane.normal().length(), 1.0);
}

TEST(PlaneTest, SignedDistanceObliqueNormal)
{
	// Plane through origin with normal (1,1,0)/sqrt(2)
	Plane plane(Point(0.0, 0.0, 0.0), Vector(1.0, 1.0, 0.0));
	double const expected = std::sqrt(2.0); // distance of (1,1,0) from plane
	EXPECT_DOUBLE_EQ(plane.signed_distance(Point(1.0, 1.0, 0.0)), expected);
}

TEST(PlaneTest, ContainsWithCustomTolerance)
{
	Plane plane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	// Point is 1e-6 off the plane — outside default tolerance but inside 1e-5
	Point p(0.0, 0.0, 1e-6);
	EXPECT_FALSE(plane.contains(p));
	EXPECT_TRUE(plane.contains(p, 1e-5));
}

TEST(PlaneTest, WindingOrderDeterminesNormalDirection)
{
	// Counter-clockwise winding (looking from +z): p1->p2->p3 should give +z normal
	Point p1(0.0, 0.0, 0.0);
	Point p2(1.0, 0.0, 0.0);
	Point p3(0.0, 1.0, 0.0);

	Plane plane(p1, p2, p3);

	EXPECT_GT(plane.normal().dz(), 0.0);
}