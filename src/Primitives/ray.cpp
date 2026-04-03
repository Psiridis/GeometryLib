#include "Geometry/Primitives/ray.hpp"

#include <stdexcept>

#include "utils/assert_utils.hpp"

namespace Geometry
{
	Ray::Ray (Point const& origin, Vector const& direction)
	: m_origin (origin),
		m_direction (direction)
	{
		GEOM_ASSERT (m_direction.isValidDirection ());
		if (!m_direction.isValidDirection ())
		{
			throw std::invalid_argument ("Ray direction must be finite and non-zero");
		}
	}

	Ray::Ray (Point const& origin, Point const& point_on_ray)
	: m_origin (origin),
		m_direction (point_on_ray - origin)
	{
		GEOM_ASSERT (m_direction.isValidDirection ());
		if (!m_direction.isValidDirection ())
		{
			throw std::invalid_argument ("Ray direction must be finite and non-zero");
		}
	}

	Point const& Ray::origin () const noexcept
	{
		return m_origin;
	}

	Vector const& Ray::direction () const noexcept
	{
		return m_direction;
	}

	Point Ray::at (double t) const
	{
		GEOM_ASSERT (t >= 0.0);
		if (t < 0.0)
		{
			throw std::domain_error ("Ray parameter t must be >= 0");
		}
		return m_origin + t * m_direction;
	}
} // namespace Geometry
