#ifndef GEOMETRY_VECTOR_HPP
#define GEOMETRY_VECTOR_HPP

namespace Geometry
{
	class Vector
	{
		public:
			constexpr Vector() noexcept = default;
			constexpr Vector(double dx, double dy, double dz) noexcept;

			[[nodiscard]] static Vector unit_x() noexcept;
			[[nodiscard]] static Vector unit_y() noexcept;
			[[nodiscard]] static Vector unit_z() noexcept;

			[[nodiscard]] constexpr double dx() const noexcept;
			[[nodiscard]] constexpr double dy() const noexcept;
			[[nodiscard]] constexpr double dz() const noexcept;

			[[nodiscard]] double length() const noexcept;
			[[nodiscard]] double length_squared() const noexcept;

			void normalize();
			[[nodiscard]] Vector normalized() const;

			[[nodiscard]] bool isFinite() const noexcept;
			[[nodiscard]] bool isZero() const noexcept;
			[[nodiscard]] bool isValidDirection() const noexcept;

			[[nodiscard]] double dot(Vector const& v) const noexcept;
			[[nodiscard]] Vector cross(Vector const& v) const noexcept;
			[[nodiscard]] double angle(Vector const& v) const;

			[[nodiscard]] Vector operator-() const noexcept;

			[[nodiscard]] Vector operator+(Vector const& v) const noexcept;
			Vector& operator+=(Vector const& v) noexcept;

			[[nodiscard]] Vector operator-(Vector const& v) const noexcept;
			Vector& operator-=(Vector const& v) noexcept;

			[[nodiscard]] Vector operator*(double scalar) const noexcept;
			Vector& operator*=(double scalar) noexcept;

			[[nodiscard]] Vector operator/(double scalar) const;
			Vector& operator/=(double scalar);

			[[nodiscard]] bool operator==(Vector const& v) const noexcept;
			[[nodiscard]] bool operator!=(Vector const& v) const noexcept;

		private:
			double m_dx = 0.0;
			double m_dy = 0.0;
			double m_dz = 0.0;
	};

	inline Vector operator*(double scalar, Vector const& v) noexcept
	{
		return v * scalar;
	}

	constexpr Vector::Vector(double x, double y, double z) noexcept
	: m_dx(x),
		m_dy(y),
		m_dz(z)
	{
	}

	constexpr double Vector::dx() const noexcept
	{
		return m_dx;
	}

	constexpr double Vector::dy() const noexcept
	{
		return m_dy;
	}

	constexpr double Vector::dz() const noexcept
	{
		return m_dz;
	}
} // namespace Geometry

#endif // GEOMETRY_VECTOR_HPP