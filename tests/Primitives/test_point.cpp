#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

#include <gtest/gtest.h>

using namespace Geometry;

TEST(PointTest, DefaultConstructor)
{
	Point p;
	EXPECT_EQ(p.x(), 0.0);
	EXPECT_EQ(p.y(), 0.0);
	EXPECT_EQ(p.z(), 0.0);
}

TEST(PointTest, ConstructWithValues)
{
	Point p(1., 2., 3.);
	EXPECT_DOUBLE_EQ(p.x(), 1.0);
	EXPECT_DOUBLE_EQ(p.y(), 2.0);
	EXPECT_DOUBLE_EQ(p.z(), 3.0);
}

TEST(PointTest, AccessorsReturnCorrectValues)
{
	Point p;
	EXPECT_DOUBLE_EQ(p.x(), 0.0);
	EXPECT_DOUBLE_EQ(p.y(), 0.0);
	EXPECT_DOUBLE_EQ(p.z(), 0.0);
}

TEST(PointTest, CopyConstructor)
{
	Point p1(1., 2., 3.);
	Point p2 = p1; // Copy constructor
	EXPECT_DOUBLE_EQ(p2.x(), 1.0);
	EXPECT_DOUBLE_EQ(p2.y(), 2.0);
	EXPECT_DOUBLE_EQ(p2.z(), 3.0);
}

TEST(PointTest, CopyAssignment)
{
	Point p1(1., 2., 3.);
	Point p2;
	p2 = p1; // Assignment operator
	EXPECT_DOUBLE_EQ(p2.x(), 1.0);
	EXPECT_DOUBLE_EQ(p2.y(), 2.0);
	EXPECT_DOUBLE_EQ(p2.z(), 3.0);
}

TEST(PointTest, MoveConstructor)
{
	Point p(1., 2., 3.);
	Point p2 = std::move(p); // Move constructor
	EXPECT_DOUBLE_EQ(p2.x(), 1.0);
	EXPECT_DOUBLE_EQ(p2.y(), 2.0);
	EXPECT_DOUBLE_EQ(p2.z(), 3.0);
}

TEST(PointTest, MoveAssignment)
{
	Point p(1., 2., 3.);
	Point p2;
	p2 = std::move(p); // Move assignment operator
	EXPECT_DOUBLE_EQ(p2.x(), 1.0);
	EXPECT_DOUBLE_EQ(p2.y(), 2.0);
	EXPECT_DOUBLE_EQ(p2.z(), 3.0);
}

TEST(PointTest, EqualityOfSamePoints)
{
	Point p1(1.0, 2.0, 3.0);
	Point p2(1.0, 2.0, 3.0);
	EXPECT_TRUE(p1 == p2);
}

TEST(PointTest, ApproximateEquality)
{
	Point p1(1.0, 2.0, 3.0);
	Point p2(1.0 + 1e-13, 2.0, 3.0);
	EXPECT_TRUE(p1 == p2);
}

TEST(PointTest, Inequality)
{
	Point p1(1.0, 2.0, 3.0);
	Point p2(1.0, 2.0, 4.0);
	EXPECT_TRUE(p1 != p2);
}

TEST(PointTest, DistanceSquared)
{
	Point p1(0.0, 0.0, 0.0);
	Point p2(3.0, 4.0, 0.0);
	EXPECT_DOUBLE_EQ(p1.distance_squared(p2), 25.0);
}

TEST(PointTest, Distance)
{
	Point p1(0.0, 0.0, 0.0);
	Point p2(3.0, 4.0, 0.0);
	EXPECT_DOUBLE_EQ(p1.distance(p2), 5.0);
}

TEST(PointTest, DistanceIsSymmetric)
{
	Point p1(1.0, 2.0, 3.0);
	Point p2(4.0, 6.0, 3.0);
	EXPECT_DOUBLE_EQ(p1.distance(p2), p2.distance(p1));
}

TEST(PointTest, SubtractPointGivesVector)
{
	Point p1(4.0, 6.0, 8.0);
	Point p2(1.0, 2.0, 3.0);
	Vector v = p1 - p2;
	EXPECT_DOUBLE_EQ(v.dx(), 3.0);
	EXPECT_DOUBLE_EQ(v.dy(), 4.0);
	EXPECT_DOUBLE_EQ(v.dz(), 5.0);
}

TEST(PointTest, AddVector)
{
	Point p(1.0, 2.0, 3.0);
	Vector v(1.0, 1.0, 1.0);
	Point result = p + v;
	EXPECT_DOUBLE_EQ(result.x(), 2.0);
	EXPECT_DOUBLE_EQ(result.y(), 3.0);
	EXPECT_DOUBLE_EQ(result.z(), 4.0);
}

TEST(PointTest, SubtractVector)
{
	Point p(3.0, 5.0, 7.0);
	Vector v(1.0, 2.0, 3.0);
	Point result = p - v;
	EXPECT_DOUBLE_EQ(result.x(), 2.0);
	EXPECT_DOUBLE_EQ(result.y(), 3.0);
	EXPECT_DOUBLE_EQ(result.z(), 4.0);
}

TEST(PointTest, AddAssignVector)
{
	Point p(1.0, 2.0, 3.0);
	Vector v(1.0, 1.0, 1.0);
	p += v;
	EXPECT_DOUBLE_EQ(p.x(), 2.0);
	EXPECT_DOUBLE_EQ(p.y(), 3.0);
	EXPECT_DOUBLE_EQ(p.z(), 4.0);
}

TEST(PointTest, SubtractAssignVector)
{
	Point p(3.0, 5.0, 7.0);
	Vector v(1.0, 2.0, 3.0);
	p -= v;
	EXPECT_DOUBLE_EQ(p.x(), 2.0);
	EXPECT_DOUBLE_EQ(p.y(), 3.0);
	EXPECT_DOUBLE_EQ(p.z(), 4.0);
}