#include "Geometry/Bounds/bounding_box.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

#include <gtest/gtest.h>

#include <vector>

using namespace Geometry;

static constexpr double kTol = 1e-9;

// ── Helpers ───────────────────────────────────────────────────────────────────

static BoundingBox unit_box()
{
	return BoundingBox(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
}

// ── Construction ──────────────────────────────────────────────────────────────

TEST(BoundingBoxConstruction, ValidBox)
{
	BoundingBox b = unit_box();
	EXPECT_EQ(b.min(), Point(0.0, 0.0, 0.0));
	EXPECT_EQ(b.max(), Point(1.0, 1.0, 1.0));
}

TEST(BoundingBoxConstruction, DegeneratePointBox)
{
	// min == max is a valid degenerate box (a single point)
	BoundingBox b(Point(1.0, 2.0, 3.0), Point(1.0, 2.0, 3.0));
	EXPECT_EQ(b.min(), b.max());
}

TEST(BoundingBoxConstruction, DegenerateFlatBox)
{
	// Zero thickness on one axis is allowed
	BoundingBox b(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 0.0));
	EXPECT_NEAR(b.volume(), 0.0, kTol);
}

TEST(BoundingBoxConstruction, InvertedXAborts)
{
	EXPECT_DEATH(BoundingBox(Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 1.0)), "");
}

TEST(BoundingBoxConstruction, InvertedYAborts)
{
	EXPECT_DEATH(BoundingBox(Point(0.0, 1.0, 0.0), Point(1.0, 0.0, 1.0)), "");
}

TEST(BoundingBoxConstruction, InvertedZAborts)
{
	EXPECT_DEATH(BoundingBox(Point(0.0, 0.0, 1.0), Point(1.0, 1.0, 0.0)), "");
}

// ── from_points ───────────────────────────────────────────────────────────────

TEST(BoundingBoxFromPoints, SinglePoint)
{
	std::vector<Point> pts = { Point(3.0, 4.0, 5.0) };
	BoundingBox b					 = BoundingBox::from_points(pts);
	EXPECT_EQ(b.min(), Point(3.0, 4.0, 5.0));
	EXPECT_EQ(b.max(), Point(3.0, 4.0, 5.0));
}

TEST(BoundingBoxFromPoints, MultiplePoints)
{
	std::vector<Point> pts = {
		Point(1.0, 2.0, -1.0),
		Point(-3.0, 4.0, 0.0),
		Point(2.0, -1.0, 5.0),
	};
	BoundingBox b = BoundingBox::from_points(pts);
	EXPECT_EQ(b.min(), Point(-3.0, -1.0, -1.0));
	EXPECT_EQ(b.max(), Point(2.0, 4.0, 5.0));
}

TEST(BoundingBoxFromPoints, EmptySpanAborts)
{
	std::vector<Point> pts;
	EXPECT_DEATH(BoundingBox::from_points(pts), "");
}

// ── center / diagonal ─────────────────────────────────────────────────────────

TEST(BoundingBoxCenter, UnitBox)
{
	EXPECT_EQ(unit_box().center(), Point(0.5, 0.5, 0.5));
}

TEST(BoundingBoxCenter, OffsetBox)
{
	BoundingBox b(Point(2.0, 4.0, 6.0), Point(4.0, 8.0, 10.0));
	EXPECT_EQ(b.center(), Point(3.0, 6.0, 8.0));
}

TEST(BoundingBoxDiagonal, UnitBox)
{
	Vector d = unit_box().diagonal();
	EXPECT_NEAR(d.dx(), 1.0, kTol);
	EXPECT_NEAR(d.dy(), 1.0, kTol);
	EXPECT_NEAR(d.dz(), 1.0, kTol);
}

TEST(BoundingBoxDiagonal, RectangularBox)
{
	BoundingBox b(Point(0.0, 0.0, 0.0), Point(3.0, 4.0, 5.0));
	Vector d = b.diagonal();
	EXPECT_NEAR(d.dx(), 3.0, kTol);
	EXPECT_NEAR(d.dy(), 4.0, kTol);
	EXPECT_NEAR(d.dz(), 5.0, kTol);
}

// ── volume / surface_area ─────────────────────────────────────────────────────

TEST(BoundingBoxVolume, UnitBox)
{
	EXPECT_NEAR(unit_box().volume(), 1.0, kTol);
}

TEST(BoundingBoxVolume, RectangularBox)
{
	BoundingBox b(Point(0.0, 0.0, 0.0), Point(2.0, 3.0, 4.0));
	EXPECT_NEAR(b.volume(), 24.0, kTol);
}

TEST(BoundingBoxVolume, DegenerateFlatBox)
{
	BoundingBox b(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 0.0));
	EXPECT_NEAR(b.volume(), 0.0, kTol);
}

TEST(BoundingBoxSurfaceArea, UnitBox)
{
	EXPECT_NEAR(unit_box().surface_area(), 6.0, kTol);
}

TEST(BoundingBoxSurfaceArea, RectangularBox)
{
	// 2 × 3 × 4 box:  2*(2*3 + 3*4 + 4*2) = 2*(6+12+8) = 52
	BoundingBox b(Point(0.0, 0.0, 0.0), Point(2.0, 3.0, 4.0));
	EXPECT_NEAR(b.surface_area(), 52.0, kTol);
}

TEST(BoundingBoxSurfaceArea, FlatBoxHasNonzeroArea)
{
	BoundingBox b(Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 0.0));
	// Two 1×1 faces on top/bottom, but dz=0 so those face-pairs collapse.
	// Remaining pairs: 2*(1*1 + 1*0 + 0*1) = 2
	EXPECT_NEAR(b.surface_area(), 2.0, kTol);
}

// ── contains(Point) ───────────────────────────────────────────────────────────

TEST(BoundingBoxContainsPoint, Interior)
{
	EXPECT_TRUE(unit_box().contains(Point(0.5, 0.5, 0.5)));
}

TEST(BoundingBoxContainsPoint, MinCorner)
{
	EXPECT_TRUE(unit_box().contains(Point(0.0, 0.0, 0.0)));
}

TEST(BoundingBoxContainsPoint, MaxCorner)
{
	EXPECT_TRUE(unit_box().contains(Point(1.0, 1.0, 1.0)));
}

TEST(BoundingBoxContainsPoint, OnFace)
{
	EXPECT_TRUE(unit_box().contains(Point(0.5, 0.5, 0.0)));
}

TEST(BoundingBoxContainsPoint, OutsideX)
{
	EXPECT_FALSE(unit_box().contains(Point(1.5, 0.5, 0.5)));
}

TEST(BoundingBoxContainsPoint, OutsideY)
{
	EXPECT_FALSE(unit_box().contains(Point(0.5, -0.1, 0.5)));
}

TEST(BoundingBoxContainsPoint, OutsideZ)
{
	EXPECT_FALSE(unit_box().contains(Point(0.5, 0.5, 1.1)));
}

// ── contains(BoundingBox) ─────────────────────────────────────────────────────

TEST(BoundingBoxContainsBox, ContainsItself)
{
	EXPECT_TRUE(unit_box().contains(unit_box()));
}

TEST(BoundingBoxContainsBox, LargerContainsSmaller)
{
	BoundingBox big(Point(-1.0, -1.0, -1.0), Point(2.0, 2.0, 2.0));
	EXPECT_TRUE(big.contains(unit_box()));
}

TEST(BoundingBoxContainsBox, SmallerDoesNotContainLarger)
{
	BoundingBox big(Point(-1.0, -1.0, -1.0), Point(2.0, 2.0, 2.0));
	EXPECT_FALSE(unit_box().contains(big));
}

TEST(BoundingBoxContainsBox, PartialOverlapNotContained)
{
	BoundingBox other(Point(0.5, 0.5, 0.5), Point(1.5, 1.5, 1.5));
	EXPECT_FALSE(unit_box().contains(other));
}

// ── intersects ────────────────────────────────────────────────────────────────

TEST(BoundingBoxIntersects, OverlappingBoxes)
{
	BoundingBox other(Point(0.5, 0.5, 0.5), Point(1.5, 1.5, 1.5));
	EXPECT_TRUE(unit_box().intersects(other));
}

TEST(BoundingBoxIntersects, TouchingFace)
{
	// Boxes share the face x = 1
	BoundingBox other(Point(1.0, 0.0, 0.0), Point(2.0, 1.0, 1.0));
	EXPECT_TRUE(unit_box().intersects(other));
}

TEST(BoundingBoxIntersects, SeparatedBoxes)
{
	BoundingBox other(Point(2.0, 0.0, 0.0), Point(3.0, 1.0, 1.0));
	EXPECT_FALSE(unit_box().intersects(other));
}

TEST(BoundingBoxIntersects, ContainedBoxIntersects)
{
	BoundingBox inner(Point(0.2, 0.2, 0.2), Point(0.8, 0.8, 0.8));
	EXPECT_TRUE(unit_box().intersects(inner));
}

TEST(BoundingBoxIntersects, SelfIntersects)
{
	EXPECT_TRUE(unit_box().intersects(unit_box()));
}

// ── expand(Point) ─────────────────────────────────────────────────────────────

TEST(BoundingBoxExpandPoint, ExpandOutside)
{
	BoundingBox b = unit_box();
	b.expand(Point(2.0, -1.0, 3.0));
	EXPECT_EQ(b.min(), Point(0.0, -1.0, 0.0));
	EXPECT_EQ(b.max(), Point(2.0, 1.0, 3.0));
}

TEST(BoundingBoxExpandPoint, ExpandInsideNoChange)
{
	BoundingBox b = unit_box();
	b.expand(Point(0.5, 0.5, 0.5));
	EXPECT_EQ(b.min(), Point(0.0, 0.0, 0.0));
	EXPECT_EQ(b.max(), Point(1.0, 1.0, 1.0));
}

TEST(BoundingBoxExpandPoint, ExpandOnBoundaryNoChange)
{
	BoundingBox b = unit_box();
	b.expand(Point(1.0, 1.0, 1.0));
	EXPECT_EQ(b.min(), Point(0.0, 0.0, 0.0));
	EXPECT_EQ(b.max(), Point(1.0, 1.0, 1.0));
}

TEST(BoundingBoxExpandPoint, ChainedExpand)
{
	BoundingBox b = unit_box();
	b.expand(Point(2.0, 0.0, 0.0)).expand(Point(0.0, 3.0, 0.0));
	EXPECT_EQ(b.max(), Point(2.0, 3.0, 1.0));
}

// ── expand(BoundingBox) ───────────────────────────────────────────────────────

TEST(BoundingBoxExpandBox, ExpandToLargerBox)
{
	BoundingBox b = unit_box();
	BoundingBox other(Point(-1.0, -1.0, -1.0), Point(2.0, 2.0, 2.0));
	b.expand(other);
	EXPECT_EQ(b.min(), Point(-1.0, -1.0, -1.0));
	EXPECT_EQ(b.max(), Point(2.0, 2.0, 2.0));
}

TEST(BoundingBoxExpandBox, ExpandBySmallerBoxNoChange)
{
	BoundingBox b = unit_box();
	BoundingBox inner(Point(0.2, 0.2, 0.2), Point(0.8, 0.8, 0.8));
	b.expand(inner);
	EXPECT_EQ(b.min(), Point(0.0, 0.0, 0.0));
	EXPECT_EQ(b.max(), Point(1.0, 1.0, 1.0));
}

// ── operator== / operator!= ───────────────────────────────────────────────────

TEST(BoundingBoxEquality, SameBox)
{
	EXPECT_TRUE(unit_box() == unit_box());
	EXPECT_FALSE(unit_box() != unit_box());
}

TEST(BoundingBoxEquality, DifferentMin)
{
	BoundingBox a = unit_box();
	BoundingBox b(Point(0.1, 0.0, 0.0), Point(1.0, 1.0, 1.0));
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a != b);
}

TEST(BoundingBoxEquality, DifferentMax)
{
	BoundingBox a = unit_box();
	BoundingBox b(Point(0.0, 0.0, 0.0), Point(2.0, 1.0, 1.0));
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a != b);
}
