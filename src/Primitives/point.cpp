#include "Geometry/Primitives/point.hpp"

#include "Geometry/Primitives/vector.hpp"

#include <cmath>

#include "utils/numerical_utils.hpp"

namespace Geometry
{
	double Point::distance(Point const& p) const
	{
		return std::sqrt(distance_squared(p));
	}

	double Point::distance_squared(Point const& p) const noexcept
	{
		double const dx = m_x - p.m_x;
		double const dy = m_y - p.m_y;
		double const dz = m_z - p.m_z;
		return dx * dx + dy * dy + dz * dz;
	}

	Vector Point::operator-(Point const& other) const noexcept
	{
		return Vector(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
	}

	bool Point::operator==(Point const& p) const noexcept
	{
		bool const x_equal = utils::almost_equal(m_x, p.m_x);
		bool const y_equal = utils::almost_equal(m_y, p.m_y);
		bool const z_equal = utils::almost_equal(m_z, p.m_z);
		return x_equal && y_equal && z_equal;
	}

	bool Point::operator!=(Point const& p) const noexcept
	{
		return !(*this == p);
	}

	Point Point::operator+(Vector const& v) const noexcept
	{
		return Point(m_x + v.dx(), m_y + v.dy(), m_z + v.dz());
	}

	Point& Point::operator+=(Vector const& v) noexcept
	{
		m_x += v.dx();
		m_y += v.dy();
		m_z += v.dz();
		return *this;
	}

	Point Point::operator-(Vector const& v) const noexcept
	{
		return Point(m_x - v.dx(), m_y - v.dy(), m_z - v.dz());
	}

	Point& Point::operator-=(Vector const& v) noexcept
	{
		m_x -= v.dx();
		m_y -= v.dy();
		m_z -= v.dz();
		return *this;
	}
} // namespace Geometry
