#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

using namespace Geometry;

TEST (LineTest, ConstructFromOriginAndDirection)
{
	Point origin (1.0, 2.0, 3.0);
	Vector direction (4.0, 5.0, 6.0);

	Line line (origin, direction);

	EXPECT_TRUE (line.direction ().isValidDirection ());
}

TEST (LineTest, ConstructFromTwoDistinctPoints)
{
	Point p1 (1.0, 2.0, 3.0);
	Point p2 (4.0, 6.0, 8.0);

	Line line (p1, p2);

	EXPECT_TRUE (line.direction ().isValidDirection ());
}

TEST (LineTest, ConstructFromCoincidentPointsIsInvalid)
{
	Point p (1.0, 2.0, 3.0);
#ifdef NDEBUG
	EXPECT_THROW (Line (p, p), std::invalid_argument);
#else
	EXPECT_DEATH (Line (p, p), "GEOM_ASSERT");
#endif
}

TEST (LineTest, ConstructFromInvalidDirectionIsInvalid)
{
	Point origin (1.0, 2.0, 3.0);
	Vector zero (0.0, 0.0, 0.0);
#ifdef NDEBUG
	EXPECT_THROW (Line (origin, zero), std::invalid_argument);
#else
	EXPECT_DEATH (Line (origin, zero), "GEOM_ASSERT");
#endif
}

TEST (LineTest, PointAtZeroReturnsOrigin)
{
	Point origin (1.0, 2.0, 3.0);
	Vector direction (4.0, 5.0, 6.0);

	Line line (origin, direction);

	Point p = line.at (0.0);

	EXPECT_TRUE (p == origin);
}

TEST (LineTest, PointAtPositiveParameter)
{
	Point origin (1.0, 2.0, 3.0);
	Vector direction (4.0, 5.0, 6.0);

	Line line (origin, direction);

	Point p = line.at (2.0);

	EXPECT_DOUBLE_EQ (p.x (), 9.0);
	EXPECT_DOUBLE_EQ (p.y (), 12.0);
	EXPECT_DOUBLE_EQ (p.z (), 15.0);
}

TEST (LineTest, PointAtNegativeParameter)
{
	Point origin (1.0, 2.0, 3.0);
	Vector direction (4.0, 5.0, 6.0);

	Line line (origin, direction);

	Point p = line.at (-1.0);

	EXPECT_DOUBLE_EQ (p.x (), -3.0);
	EXPECT_DOUBLE_EQ (p.y (), -3.0);
	EXPECT_DOUBLE_EQ (p.z (), -3.0);
}

TEST (LineTest, PointDifferenceMatchesDirectionScaling)
{
	Point origin (1.0, 2.0, 3.0);
	Vector direction (2.0, 0.0, -1.0);

	Line line (origin, direction);

	Point p0 = line.at (0.0);
	Point p3 = line.at (3.0);

	Vector diff			= p3 - p0;
	Vector expected = 3.0 * direction;

	EXPECT_TRUE (diff == expected);
}

TEST (LineTest, OriginAccessorValue)
{
	Point origin (1.0, 2.0, 3.0);
	Vector direction (4.0, 5.0, 6.0);

	Line line (origin, direction);

	EXPECT_TRUE (line.origin () == origin);
}

TEST (LineTest, DirectionAccessorIsNotNormalized)
{
	Point origin (0.0, 0.0, 0.0);
	Vector direction (0.0, 0.0, 5.0);

	Line line (origin, direction);

	// Line must not normalize the stored direction
	EXPECT_DOUBLE_EQ (line.direction ().dx (), 0.0);
	EXPECT_DOUBLE_EQ (line.direction ().dy (), 0.0);
	EXPECT_DOUBLE_EQ (line.direction ().dz (), 5.0);
	EXPECT_DOUBLE_EQ (line.direction ().length (), 5.0);
}

TEST (LineTest, DirectionFromTwoPointsValue)
{
	Point p1 (1.0, 2.0, 3.0);
	Point p2 (4.0, 2.0, 3.0);

	Line line (p1, p2);

	Vector expected = p2 - p1;
	EXPECT_TRUE (line.direction () == expected);
}