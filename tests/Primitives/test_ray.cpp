#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/ray.hpp"
#include "Geometry/Primitives/vector.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

using namespace Geometry;

TEST(RayTest, ConstructFromOriginAndDirection)
{
	Point origin(1.0, 2.0, 3.0);
	Vector direction(4.0, 5.0, 6.0);

	Ray ray(origin, direction);

	EXPECT_TRUE(ray.direction().isValidDirection());
}

TEST(RayTest, ConstructFromOriginAndPointOnRay)
{
	Point origin(1.0, 2.0, 3.0);
	Point point_on_ray(4.0, 6.0, 8.0);

	Ray ray(origin, point_on_ray);

	EXPECT_TRUE(ray.direction().isValidDirection());
}

TEST(RayTest, ConstructFromCoincidentPointsIsInvalid)
{
	Point p(1.0, 2.0, 3.0);
#ifdef NDEBUG
	EXPECT_THROW(Ray(p, p), std::invalid_argument);
#else
	EXPECT_DEATH(Ray(p, p), "GEOM_ASSERT");
#endif
}

TEST(RayTest, ConstructFromInvalidDirectionIsInvalid)
{
	Point origin(1.0, 2.0, 3.0);
	Vector zero(0.0, 0.0, 0.0);
#ifdef NDEBUG
	EXPECT_THROW(Ray(origin, zero), std::invalid_argument);
#else
	EXPECT_DEATH(Ray(origin, zero), "GEOM_ASSERT");
#endif
}

TEST(RayTest, PointAtZeroReturnsOrigin)
{
	Point origin(1.0, 2.0, 3.0);
	Vector direction(4.0, 5.0, 6.0);

	Ray ray(origin, direction);

	Point p = ray.at(0.0);

	EXPECT_TRUE(p == origin);
}

TEST(RayTest, PointAtPositiveParameter)
{
	Point origin(1.0, 2.0, 3.0);
	Vector direction(4.0, 5.0, 6.0);

	Ray ray(origin, direction);

	Point p = ray.at(2.0);

	EXPECT_DOUBLE_EQ(p.x(), 9.0);
	EXPECT_DOUBLE_EQ(p.y(), 12.0);
	EXPECT_DOUBLE_EQ(p.z(), 15.0);
}

TEST(RayTest, PointAtNegativeParameter)
{
	Point origin(1.0, 2.0, 3.0);
	Vector direction(4.0, 5.0, 6.0);

	Ray ray(origin, direction);

#ifdef NDEBUG
	EXPECT_THROW(ray.at(-1.0), std::domain_error);
#else
	EXPECT_DEATH({ [[maybe_unused]] auto r = ray.at(-1.0); }, "GEOM_ASSERT");
#endif
}

TEST(RayTest, OriginAccessor)
{
	Point origin(1.0, 2.0, 3.0);
	Vector direction(4.0, 5.0, 6.0);

	Ray ray(origin, direction);

	EXPECT_TRUE(ray.origin() == origin);
}

TEST(RayTest, DirectionAccessor)
{
	Point origin(1.0, 2.0, 3.0);
	Vector direction(4.0, 5.0, 6.0);

	Ray ray(origin, direction);

	EXPECT_TRUE(ray.direction() == direction);
}

TEST(RayTest, PointDifferenceMatchesDirectionScaling)
{
	Point origin(1.0, 2.0, 3.0);
	Vector direction(2.0, 0.0, -1.0);

	Ray ray(origin, direction);

	Point p0 = ray.at(0.0);
	Point p3 = ray.at(3.0);

	Vector diff			= p3 - p0;
	Vector expected = 3.0 * direction;

	EXPECT_TRUE(diff == expected);
}

TEST(RayTest, DirectionFromTwoPointsValue)
{
	Point origin(1.0, 2.0, 3.0);
	Point point_on_ray(4.0, 2.0, 3.0);

	Ray ray(origin, point_on_ray);

	Vector expected = point_on_ray - origin;
	EXPECT_TRUE(ray.direction() == expected);
}
