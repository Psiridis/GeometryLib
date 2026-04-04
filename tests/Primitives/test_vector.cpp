#include "Geometry/Primitives/vector.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <numbers>
#include <utility>

using namespace Geometry;

TEST(VectorTest, DefaultConstructor)
{
	Vector v;
	EXPECT_DOUBLE_EQ(v.length(), 0.0);
}

TEST(VectorTest, ConstructWithValues)
{
	Vector v(1., 0., 0.);
	EXPECT_DOUBLE_EQ(v.dx(), 1.0);
	EXPECT_DOUBLE_EQ(v.dy(), 0.0);
	EXPECT_DOUBLE_EQ(v.dz(), 0.0);
}

TEST(VectorTest, CopyConstructor)
{
	Vector v1(1., 2., 3.);
	Vector v2(v1);
	EXPECT_DOUBLE_EQ(v2.dx(), v1.dx());
	EXPECT_DOUBLE_EQ(v2.dy(), v1.dy());
	EXPECT_DOUBLE_EQ(v2.dz(), v1.dz());
}

TEST(VectorTest, CopyAssignment)
{
	Vector v1(1., 2., 3.);
	Vector v2;
	v2 = v1;
	EXPECT_DOUBLE_EQ(v2.dx(), v1.dx());
	EXPECT_DOUBLE_EQ(v2.dy(), v1.dy());
	EXPECT_DOUBLE_EQ(v2.dz(), v1.dz());
}

TEST(VectorTest, MoveConstructor)
{
	Vector v1(1., 2., 3.);
	Vector v2(std::move(v1));
	EXPECT_DOUBLE_EQ(v2.dx(), 1.0);
	EXPECT_DOUBLE_EQ(v2.dy(), 2.0);
	EXPECT_DOUBLE_EQ(v2.dz(), 3.0);
}

TEST(VectorTest, MoveAssignment)
{
	Vector v1(1., 2., 3.);
	Vector v2;
	v2 = std::move(v1);
	EXPECT_DOUBLE_EQ(v2.dx(), 1.0);
	EXPECT_DOUBLE_EQ(v2.dy(), 2.0);
	EXPECT_DOUBLE_EQ(v2.dz(), 3.0);
}

TEST(VectorTest, ValidVector)
{
	Vector v(1., 2., 3.);
	EXPECT_TRUE(v.isFinite());
	EXPECT_FALSE(v.isZero());
	EXPECT_TRUE(v.isValidDirection());
}

TEST(VectorTest, ZeroVectorInvalid)
{
	Vector v(0., 0., 0.);
	EXPECT_TRUE(v.isFinite());
	EXPECT_TRUE(v.isZero());
	EXPECT_FALSE(v.isValidDirection());
}

TEST(VectorTest, LengthCalculation)
{
	Vector v(3., 4., 0.);
	EXPECT_DOUBLE_EQ(v.length(), 5.0);
}

TEST(VectorTest, LengthSquaredCalculation)
{
	Vector v(3., 4., 0.);
	EXPECT_DOUBLE_EQ(v.length_squared(), 25.0);
}

TEST(VectorTest, NormalizeCalculation)
{
	Vector v(3., 4., 0.);
	v.normalize();
	EXPECT_DOUBLE_EQ(v.length(), 1.0);
}

TEST(VectorTest, NormalizeZeroVectorThrows)
{
	Vector v(0.0, 0.0, 0.0);
#ifdef NDEBUG
	EXPECT_THROW([&]() { v.normalize(); }(), std::domain_error);
#else
	EXPECT_DEATH(v.normalize(), "GEOM_ASSERT");
#endif
}

TEST(VectorTest, NormalizedCalculation)
{
	Vector v(3., 4., 0.);
	Vector v_normalized = v.normalized();
	EXPECT_DOUBLE_EQ(v_normalized.length(), 1.0);
}

TEST(VectorTest, NormalizedZeroVectorThrows)
{
	Vector v(0.0, 0.0, 0.0);
#ifdef NDEBUG
	EXPECT_THROW(
	[&]()
	{
		auto r = v.normalized();
		(void)r;
	}(),
	std::domain_error);
#else
	EXPECT_DEATH(
	{
		auto r = v.normalized();
		(void)r;
	},
	"GEOM_ASSERT");
#endif
}

TEST(VectorTest, DotProduct)
{
	Vector v1(1., 0., 0.);
	Vector v2(0., 1., 0.);
	double dot_product = v1.dot(v2);
	EXPECT_DOUBLE_EQ(dot_product, 0.0);
}

TEST(VectorTest, CrossProduct)
{
	Vector v1(1., 0., 0.);
	Vector v2(0., 1., 0.);
	Vector cross_product = v1.cross(v2);
	EXPECT_DOUBLE_EQ(cross_product.dx(), 0.0);
	EXPECT_DOUBLE_EQ(cross_product.dy(), 0.0);
	EXPECT_DOUBLE_EQ(cross_product.dz(), 1.0);
}

TEST(VectorTest, CrossProductOrthogonality)
{
	Vector v1(1., 2., 3.);
	Vector v2(4., 5., 6.);
	Vector cross_product = v1.cross(v2);
	EXPECT_DOUBLE_EQ(cross_product.dot(v1), 0.0);
	EXPECT_DOUBLE_EQ(cross_product.dot(v2), 0.0);
}

TEST(VectorTest, AngleBetweenVectors)
{
	Vector v1(1., 0., 0.);
	Vector v2(0., 1., 0.);
	double angle								= v1.angle(v2);
	double constexpr pi					= std::numbers::pi;
	double const expected_angle = pi / 2.0;
	EXPECT_DOUBLE_EQ(angle, expected_angle);
}

TEST(VectorTest, AngleWithZeroVectorThrows)
{
	Vector v1(1., 0., 0.);
	Vector v2(0., 0., 0.);
#ifdef NDEBUG
	EXPECT_THROW(
	[&]()
	{
		auto r = v1.angle(v2);
		(void)r;
	}(),
	std::domain_error);
#else
	EXPECT_DEATH(
	{
		auto r = v1.angle(v2);
		(void)r;
	},
	"GEOM_ASSERT");
#endif
}

TEST(VectorTest, Negation)
{
	Vector v(1., -2., 3.);
	Vector negated = -v;
	EXPECT_DOUBLE_EQ(negated.dx(), -1.0);
	EXPECT_DOUBLE_EQ(negated.dy(), 2.0);
	EXPECT_DOUBLE_EQ(negated.dz(), -3.0);
}

TEST(VectorTest, Addition)
{
	Vector v1(1., 2., 3.);
	Vector v2(4., 5., 6.);
	Vector sum = v1 + v2;
	EXPECT_DOUBLE_EQ(sum.dx(), 5.0);
	EXPECT_DOUBLE_EQ(sum.dy(), 7.0);
	EXPECT_DOUBLE_EQ(sum.dz(), 9.0);
}

TEST(VectorTest, Subtraction)
{
	Vector v1(4., 5., 6.);
	Vector v2(1., 2., 3.);
	Vector difference = v1 - v2;
	EXPECT_DOUBLE_EQ(difference.dx(), 3.0);
	EXPECT_DOUBLE_EQ(difference.dy(), 3.0);
	EXPECT_DOUBLE_EQ(difference.dz(), 3.0);
}

TEST(VectorTest, ScalarMultiplicationLeft)
{
	Vector v(1., 2., 3.);
	Vector scaled = 2.0 * v;
	EXPECT_DOUBLE_EQ(scaled.dx(), 2.0);
	EXPECT_DOUBLE_EQ(scaled.dy(), 4.0);
	EXPECT_DOUBLE_EQ(scaled.dz(), 6.0);
}

TEST(VectorTest, ScalarMultiplicationRight)
{
	Vector v(1., 2., 3.);
	Vector scaled = v * 2.0;
	EXPECT_DOUBLE_EQ(scaled.dx(), 2.0);
	EXPECT_DOUBLE_EQ(scaled.dy(), 4.0);
	EXPECT_DOUBLE_EQ(scaled.dz(), 6.0);
}

TEST(VectorTest, ScalarDivision)
{
	Vector v(2., 4., 6.);
	Vector scaled = v / 2.0;
	EXPECT_DOUBLE_EQ(scaled.dx(), 1.0);
	EXPECT_DOUBLE_EQ(scaled.dy(), 2.0);
	EXPECT_DOUBLE_EQ(scaled.dz(), 3.0);
}

TEST(VectorTest, ScalarDivisionByZeroThrows)
{
	Vector v(1., 2., 3.);

	EXPECT_THROW(
	[&]()
	{
		auto r = v / 0.0;
		(void)r;
	}(),
	std::domain_error);
}

TEST(VectorTest, Equality)
{
	Vector v1(1., 2., 3.);
	Vector v2(1., 2., 3.);
	EXPECT_TRUE(v1 == v2);
}

TEST(VectorTest, ApproximateEquality)
{
	Vector v1(1.0, 2.0, 3.0);
	Vector v2(1.0 + 1e-10, 2.0, 3.0);
	EXPECT_TRUE(v1 == v2);
}

TEST(VectorTest, Inequality)
{
	Vector v1(1., 2., 3.);
	Vector v2(4., 5., 6.);
	EXPECT_TRUE(v1 != v2);
}

TEST(VectorTest, UnitVectors)
{
	Vector unit_x = Vector::unit_x();
	Vector unit_y = Vector::unit_y();
	Vector unit_z = Vector::unit_z();

	EXPECT_DOUBLE_EQ(unit_x.dx(), 1.0);
	EXPECT_DOUBLE_EQ(unit_x.dy(), 0.0);
	EXPECT_DOUBLE_EQ(unit_x.dz(), 0.0);
	EXPECT_DOUBLE_EQ(unit_x.length(), 1.0);

	EXPECT_DOUBLE_EQ(unit_y.dx(), 0.0);
	EXPECT_DOUBLE_EQ(unit_y.dy(), 1.0);
	EXPECT_DOUBLE_EQ(unit_y.dz(), 0.0);
	EXPECT_DOUBLE_EQ(unit_y.length(), 1.0);

	EXPECT_DOUBLE_EQ(unit_z.dx(), 0.0);
	EXPECT_DOUBLE_EQ(unit_z.dy(), 0.0);
	EXPECT_DOUBLE_EQ(unit_z.dz(), 1.0);
	EXPECT_DOUBLE_EQ(unit_z.length(), 1.0);
}

TEST(VectorTest, AddAssign)
{
	Vector v1(1.0, 2.0, 3.0);
	Vector v2(4.0, 5.0, 6.0);
	v1 += v2;
	EXPECT_DOUBLE_EQ(v1.dx(), 5.0);
	EXPECT_DOUBLE_EQ(v1.dy(), 7.0);
	EXPECT_DOUBLE_EQ(v1.dz(), 9.0);
}

TEST(VectorTest, SubtractAssign)
{
	Vector v1(4.0, 5.0, 6.0);
	Vector v2(1.0, 2.0, 3.0);
	v1 -= v2;
	EXPECT_DOUBLE_EQ(v1.dx(), 3.0);
	EXPECT_DOUBLE_EQ(v1.dy(), 3.0);
	EXPECT_DOUBLE_EQ(v1.dz(), 3.0);
}

TEST(VectorTest, ScalarMultiplyAssign)
{
	Vector v(1.0, 2.0, 3.0);
	v *= 2.0;
	EXPECT_DOUBLE_EQ(v.dx(), 2.0);
	EXPECT_DOUBLE_EQ(v.dy(), 4.0);
	EXPECT_DOUBLE_EQ(v.dz(), 6.0);
}

TEST(VectorTest, ScalarDivideAssign)
{
	Vector v(2.0, 4.0, 6.0);
	v /= 2.0;
	EXPECT_DOUBLE_EQ(v.dx(), 1.0);
	EXPECT_DOUBLE_EQ(v.dy(), 2.0);
	EXPECT_DOUBLE_EQ(v.dz(), 3.0);
}

TEST(VectorTest, ScalarDivideAssignByZeroThrows)
{
	Vector v(1.0, 2.0, 3.0);
	EXPECT_THROW(v /= 0.0, std::domain_error);
}

TEST(VectorTest, IsFiniteWithNaN)
{
	double const nan = std::numeric_limits<double>::quiet_NaN();
	Vector v(nan, 0.0, 0.0);
	EXPECT_FALSE(v.isFinite());
	EXPECT_FALSE(v.isValidDirection());
}

TEST(VectorTest, IsFiniteWithInfinity)
{
	double const inf = std::numeric_limits<double>::infinity();
	Vector v(inf, 0.0, 0.0);
	EXPECT_FALSE(v.isFinite());
	EXPECT_FALSE(v.isValidDirection());
}

TEST(VectorTest, DotProductNumericResult)
{
	Vector v1(1.0, 2.0, 3.0);
	Vector v2(4.0, 5.0, 6.0);
	EXPECT_DOUBLE_EQ(v1.dot(v2), 32.0); // 1*4 + 2*5 + 3*6
}

TEST(VectorTest, CrossProductMagnitude)
{
	Vector v1(1.0, 0.0, 0.0);
	Vector v2(0.0, 1.0, 0.0);
	Vector cross = v1.cross(v2);
	double const expected_magnitude = v1.length() * v2.length() * std::sin(v1.angle(v2));
	EXPECT_DOUBLE_EQ(cross.length(), expected_magnitude);
}

TEST(VectorTest, AngleParallelVectors)
{
	Vector v1(1.0, 0.0, 0.0);
	Vector v2(2.0, 0.0, 0.0);
	EXPECT_DOUBLE_EQ(v1.angle(v2), 0.0);
}

TEST(VectorTest, AngleAntiParallelVectors)
{
	Vector v1(1.0, 0.0, 0.0);
	Vector v2(-1.0, 0.0, 0.0);
	EXPECT_DOUBLE_EQ(v1.angle(v2), std::numbers::pi);
}
