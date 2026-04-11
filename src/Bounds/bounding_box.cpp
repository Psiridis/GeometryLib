#include "Geometry/Bounds/bounding_box.hpp"

#include <algorithm>
#include <stdexcept>

#include "utils/assert_utils.hpp"

namespace Geometry
{
	BoundingBox::BoundingBox(Point const& min, Point const& max)
	: m_min(min),
		m_max(max)
	{
		bool const valid =
		m_min.x() <= m_max.x() && m_min.y() <= m_max.y() && m_min.z() <= m_max.z();
		GEOM_ASSERT(valid);
		if (!valid)
			throw std::invalid_argument(
			"BoundingBox: min must be component-wise <= max");
	}

	BoundingBox BoundingBox::from_points(std::span<Point const> points)
	{
		bool const valid = !points.empty();
		GEOM_ASSERT(valid);
		if (!valid)
			throw std::invalid_argument(
			"BoundingBox::from_points: span must not be empty");

		double min_x = points[0].x(), min_y = points[0].y(), min_z = points[0].z();
		double max_x = min_x, max_y = min_y, max_z = min_z;

		for (auto const& p : points.subspan(1))
		{
			min_x = std::min(min_x, p.x());
			min_y = std::min(min_y, p.y());
			min_z = std::min(min_z, p.z());
			max_x = std::max(max_x, p.x());
			max_y = std::max(max_y, p.y());
			max_z = std::max(max_z, p.z());
		}

		return BoundingBox(Point(min_x, min_y, min_z), Point(max_x, max_y, max_z));
	}

	Point const& BoundingBox::min() const noexcept
	{
		return m_min;
	}

	Point const& BoundingBox::max() const noexcept
	{
		return m_max;
	}

	Point BoundingBox::center() const noexcept
	{
		return m_min + 0.5 * (m_max - m_min);
	}

	Vector BoundingBox::diagonal() const noexcept
	{
		return m_max - m_min;
	}

	double BoundingBox::volume() const noexcept
	{
		Vector const d = diagonal();
		return d.dx() * d.dy() * d.dz();
	}

	double BoundingBox::surface_area() const noexcept
	{
		Vector const d	= diagonal();
		double const wx = d.dx();
		double const wy = d.dy();
		double const wz = d.dz();
		return 2.0 * (wx * wy + wy * wz + wz * wx);
	}

	bool BoundingBox::contains(Point const& p) const noexcept
	{
		return p.x() >= m_min.x() && p.x() <= m_max.x() && p.y() >= m_min.y() &&
		p.y() <= m_max.y() && p.z() >= m_min.z() && p.z() <= m_max.z();
	}

	bool BoundingBox::contains(BoundingBox const& other) const noexcept
	{
		return contains(other.m_min) && contains(other.m_max);
	}

	bool BoundingBox::intersects(BoundingBox const& other) const noexcept
	{
		return m_min.x() <= other.m_max.x() && m_max.x() >= other.m_min.x() &&
		m_min.y() <= other.m_max.y() && m_max.y() >= other.m_min.y() &&
		m_min.z() <= other.m_max.z() && m_max.z() >= other.m_min.z();
	}

	BoundingBox& BoundingBox::expand(Point const& p) noexcept
	{
		m_min = Point(std::min(m_min.x(), p.x()), std::min(m_min.y(), p.y()),
									std::min(m_min.z(), p.z()));
		m_max = Point(std::max(m_max.x(), p.x()), std::max(m_max.y(), p.y()),
									std::max(m_max.z(), p.z()));
		return *this;
	}

	BoundingBox& BoundingBox::expand(BoundingBox const& other) noexcept
	{
		expand(other.m_min);
		expand(other.m_max);
		return *this;
	}

	bool BoundingBox::operator==(BoundingBox const& other) const noexcept
	{
		return m_min == other.m_min && m_max == other.m_max;
	}

	bool BoundingBox::operator!=(BoundingBox const& other) const noexcept
	{
		return !(*this == other);
	}
} // namespace Geometry
