#include "Geometry/Shapes/circle.hpp"

#include <cmath>
#include <numbers>
#include <stdexcept>

#include "utils/assert_utils.hpp"
#include "utils/numerical_utils.hpp"

namespace Geometry
{
	Circle::Circle(Point const& center, Vector const& normal, double radius)
	: m_center(center),
		m_normal(normal),
		m_radius(radius)
	{
		bool const valid_normal = m_normal.isValidDirection();
		GEOM_ASSERT(valid_normal);
		if (!valid_normal)
			throw std::invalid_argument("Circle normal must be finite and non-zero");

		bool const valid_radius = std::isfinite(m_radius) && m_radius > 0.0;
		GEOM_ASSERT(valid_radius);
		if (!valid_radius)
			throw std::invalid_argument("Circle radius must be finite and positive");

		m_normal.normalize();
	}

	Point const& Circle::center() const noexcept
	{
		return m_center;
	}
	Vector const& Circle::normal() const noexcept
	{
		return m_normal;
	}
	double Circle::radius() const noexcept
	{
		return m_radius;
	}

	double Circle::area() const noexcept
	{
		return std::numbers::pi * m_radius * m_radius;
	}

	double Circle::circumference() const noexcept
	{
		return 2.0 * std::numbers::pi * m_radius;
	}

	Plane Circle::plane() const
	{
		return Plane(m_center, m_normal);
	}

	// ── contains(Point) ───────────────────────────────────────────────────────
	//
	// A point P lies in the disc if and only if:
	//   1. |signed_distance(P, plane)| ≤ tol   (P is coplanar)
	//   2. |P − center|² ≤ radius²              (P is within the radius)
	//
	// Condition 1 uses the supplied tolerance so that points at the numerical
	// boundary of the plane are treated consistently.
	// Condition 2 avoids a square root by comparing squared quantities.
	//
	bool Circle::contains(Point const& p, double tol) const noexcept
	{
		// Check coplanarity
		double const signed_dist = (p - m_center).dot(m_normal);
		if (std::abs(signed_dist) > tol)
			return false;

		// Check radial distance (squared, no sqrt needed)
		double const dist2 = m_center.distance_squared(p);
		return dist2 <= (m_radius + tol) * (m_radius + tol);
	}

	bool Circle::operator==(Circle const& other) const noexcept
	{
		return m_center == other.m_center && m_normal == other.m_normal &&
		utils::almost_equal(m_radius, other.m_radius);
	}

	bool Circle::operator!=(Circle const& other) const noexcept
	{
		return !(*this == other);
	}

} // namespace Geometry
