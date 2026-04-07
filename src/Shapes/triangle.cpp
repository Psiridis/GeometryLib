#include "Geometry/Shapes/triangle.hpp"

#include <stdexcept>

#include "utils/assert_utils.hpp"
#include "utils/numerical_utils.hpp"

namespace Geometry
{
	Triangle::Triangle(Point const& p0, Point const& p1, Point const& p2)
	: m_p0(p0),
		m_p1(p1),
		m_p2(p2),
		m_normal((p1 - p0).cross(p2 - p0)),
		m_area(0.0)
	{
		bool const valid = m_normal.isValidDirection();
		GEOM_ASSERT(valid);
		if (!valid)
			throw std::invalid_argument("Triangle vertices must not be collinear");

		m_area = 0.5 * m_normal.length();
		m_normal.normalize();
	}

	Point const& Triangle::p0() const noexcept
	{
		return m_p0;
	}
	Point const& Triangle::p1() const noexcept
	{
		return m_p1;
	}
	Point const& Triangle::p2() const noexcept
	{
		return m_p2;
	}

	Vector const& Triangle::normal() const noexcept
	{
		return m_normal;
	}

	double Triangle::area() const noexcept
	{
		return m_area;
	}

	Point Triangle::centroid() const noexcept
	{
		return m_p0 + (1.0 / 3.0) * ((m_p1 - m_p0) + (m_p2 - m_p0));
	}

	double Triangle::perimeter() const noexcept
	{
		return m_p0.distance(m_p1) + m_p1.distance(m_p2) + m_p2.distance(m_p0);
	}

	Plane Triangle::plane() const
	{
		return Plane(m_p0, m_normal);
	}

	// ── contains(Point) ───────────────────────────────────────────────────────
	//
	// Barycentric coordinate test.  For a point P assumed coplanar with the
	// triangle, compute the signed sub-triangle areas using cross products and
	// check that all three barycentric weights are non-negative:
	//
	//   α = area(P,p1,p2) / area(p0,p1,p2)
	//   β = area(p0,P,p2) / area(p0,p1,p2)
	//   γ = area(p0,p1,P) / area(p0,p1,p2)
	//
	// A sign-consistent shortcut: project each sub-area onto the triangle
	// normal and check the dot product is non-negative.  Dividing by 2·area
	// is unnecessary since we only need the sign.
	//
	bool Triangle::contains(Point const& p) const noexcept
	{
		// Edge vectors
		Vector const e01 = m_p1 - m_p0;
		Vector const e12 = m_p2 - m_p1;
		Vector const e20 = m_p0 - m_p2;

		// Cross products of each edge with (P − edge start)
		// A positive dot with the triangle normal means P is on the inside
		bool const inside0 = e01.cross(p - m_p0).dot(m_normal) >= -utils::k_abs_eps;
		bool const inside1 = e12.cross(p - m_p1).dot(m_normal) >= -utils::k_abs_eps;
		bool const inside2 = e20.cross(p - m_p2).dot(m_normal) >= -utils::k_abs_eps;

		return inside0 && inside1 && inside2;
	}

	bool Triangle::operator==(Triangle const& other) const noexcept
	{
		return m_p0 == other.m_p0 && m_p1 == other.m_p1 && m_p2 == other.m_p2;
	}

	bool Triangle::operator!=(Triangle const& other) const noexcept
	{
		return !(*this == other);
	}

} // namespace Geometry
