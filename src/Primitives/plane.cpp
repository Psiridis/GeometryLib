#include "Geometry/Primitives/plane.hpp"

#include <cmath>
#include <stdexcept>

#include "utils/assert_utils.hpp"
#include "utils/numerical_utils.hpp"

namespace Geometry
{
	Plane::Plane(Point const& point, Vector const& normal)
	: m_point(point),
		m_normal(normal)
	{
		GEOM_ASSERT(m_normal.isValidDirection());
		if (!m_normal.isValidDirection())
		{
			throw std::invalid_argument("Plane normal must be finite and non-zero");
		}
		m_normal.normalize();
	}

	Plane::Plane(Point const& p1, Point const& p2, Point const& p3)
	: m_point(p1),
		m_normal((p2 - p1).cross(p3 - p1))
	{
		GEOM_ASSERT(m_normal.isValidDirection());
		if (!m_normal.isValidDirection())
		{
			throw std::invalid_argument("Plane points must not be collinear");
		}
		m_normal.normalize();
	}

	Vector const& Plane::normal() const noexcept
	{
		return m_normal;
	}

	double Plane::signed_distance(Point const& p) const noexcept
	{
		return (p - m_point).dot(m_normal);
	}

	double Plane::distance(Point const& p) const noexcept
	{
		return std::abs(signed_distance(p));
	}

	bool Plane::contains(Point const& p, double abs_eps, double rel_eps) const noexcept
	{
		return utils::almost_equal(signed_distance(p), 0.0, abs_eps, rel_eps);
	}
} // namespace Geometry