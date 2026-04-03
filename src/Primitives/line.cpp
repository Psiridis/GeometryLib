#include "Geometry/Primitives/line.hpp"

#include <stdexcept>

#include "utils/assert_utils.hpp"

namespace Geometry
{
	Line::Line (Point const& origin, Vector const& direction)
	: m_origin (origin),
		m_direction (direction)
	{
		GEOM_ASSERT (m_direction.isValidDirection ());
		if (!m_direction.isValidDirection ())
		{
			throw std::invalid_argument (
			"Line direction must be finite and non-zero");
		}
	}

	Line::Line (Point const& p1, Point const& p2)
	: m_origin (p1),
		m_direction (p2 - p1)
	{
		GEOM_ASSERT (m_direction.isValidDirection ());
		if (!m_direction.isValidDirection ())
		{
			throw std::invalid_argument (
			"Line direction must be finite and non-zero");
		}
	}

	Point const& Line::origin () const noexcept
	{
		return m_origin;
	}

	Vector const& Line::direction () const noexcept
	{
		return m_direction;
	}

	Point Line::at (double t) const noexcept
	{
		return m_origin + t * m_direction;
	}
} // namespace Geometry