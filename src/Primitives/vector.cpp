#include "Geometry/Primitives/vector.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "utils/assert_utils.hpp"
#include "utils/numerical_utils.hpp"

namespace Geometry
{
	void Vector::normalize ()
	{
		GEOM_ASSERT (isValidDirection ());
		if (!isValidDirection ())
		{
			throw std::domain_error ("Cannot normalize an invalid direction vector");
		}
		*this /= length ();
	}

	Vector Vector::normalized () const
	{
		GEOM_ASSERT (isValidDirection ());
		if (!isValidDirection ())
		{
			throw std::domain_error ("Cannot normalize an invalid direction vector");
		}
		double const len = length ();
		return *this / len;
	}

	bool Vector::isFinite () const noexcept
	{
		return std::isfinite (m_dx) && std::isfinite (m_dy) && std::isfinite (m_dz);
	}

	bool Vector::isZero () const noexcept
	{
		return utils::almost_equal (length_squared (), 0.0);
	}

	bool Vector::isValidDirection () const noexcept
	{
		return isFinite () && !isZero ();
	}

	double Vector::length () const noexcept
	{
		return std::sqrt (length_squared ());
	}

	double Vector::length_squared () const noexcept
	{
		return m_dx * m_dx + m_dy * m_dy + m_dz * m_dz;
	}

	Vector Vector::cross (Vector const& v) const noexcept
	{
		return Vector (m_dy * v.m_dz - m_dz * v.m_dy, m_dz * v.m_dx - m_dx * v.m_dz,
									 m_dx * v.m_dy - m_dy * v.m_dx);
	}

	double Vector::dot (Vector const& v) const noexcept
	{
		return m_dx * v.m_dx + m_dy * v.m_dy + m_dz * v.m_dz;
	}

	double Vector::angle (Vector const& v) const
	{
		GEOM_ASSERT (isValidDirection () && v.isValidDirection ());
		if (!isValidDirection () || !v.isValidDirection ())
		{
			throw std::domain_error (
			"Cannot compute angle between invalid direction vectors");
		}
		double const len1 = length ();
		double const len2 = v.length ();

		double cos_theta = dot (v) / (len1 * len2);

		cos_theta = std::clamp (cos_theta, -1.0, 1.0);

		return std::acos (cos_theta);
	}

	Vector Vector::operator- () const noexcept
	{
		return Vector (-m_dx, -m_dy, -m_dz);
	}

	Vector Vector::operator+ (Vector const& v) const noexcept
	{
		return Vector (m_dx + v.m_dx, m_dy + v.m_dy, m_dz + v.m_dz);
	}

	Vector& Vector::operator+= (Vector const& v) noexcept
	{
		m_dx += v.m_dx;
		m_dy += v.m_dy;
		m_dz += v.m_dz;
		return *this;
	}

	Vector Vector::operator- (Vector const& v) const noexcept
	{
		return Vector (m_dx - v.m_dx, m_dy - v.m_dy, m_dz - v.m_dz);
	}

	Vector& Vector::operator-= (Vector const& v) noexcept
	{
		m_dx -= v.m_dx;
		m_dy -= v.m_dy;
		m_dz -= v.m_dz;
		return *this;
	}

	Vector Vector::operator* (double scalar) const noexcept
	{
		return Vector (m_dx * scalar, m_dy * scalar, m_dz * scalar);
	}

	Vector& Vector::operator*= (double scalar) noexcept
	{
		m_dx *= scalar;
		m_dy *= scalar;
		m_dz *= scalar;
		return *this;
	}

	Vector Vector::operator/ (double scalar) const
	{
		if (utils::almost_equal (scalar, 0.0))
		{
			throw std::domain_error ("Cannot divide a vector by zero");
		}

		return Vector (m_dx / scalar, m_dy / scalar, m_dz / scalar);
	}

	Vector& Vector::operator/= (double scalar)
	{
		if (utils::almost_equal (scalar, 0.0))
		{
			throw std::domain_error ("Cannot divide a vector by zero");
		}

		m_dx /= scalar;
		m_dy /= scalar;
		m_dz /= scalar;
		return *this;
	}

	bool Vector::operator== (Vector const& v) const noexcept
	{
		bool const dx_equal = utils::almost_equal (m_dx, v.m_dx);
		bool const dy_equal = utils::almost_equal (m_dy, v.m_dy);
		bool const dz_equal = utils::almost_equal (m_dz, v.m_dz);
		return dx_equal && dy_equal && dz_equal;
	}

	bool Vector::operator!= (Vector const& v) const noexcept
	{
		return !(*this == v);
	}

	Vector Vector::unit_x () noexcept
	{
		return Vector (1.0, 0.0, 0.0);
	}

	Vector Vector::unit_y () noexcept
	{
		return Vector (0.0, 1.0, 0.0);
	}

	Vector Vector::unit_z () noexcept
	{
		return Vector (0.0, 0.0, 1.0);
	}
} // namespace Geometry