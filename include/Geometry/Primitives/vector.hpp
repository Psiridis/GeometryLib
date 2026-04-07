#ifndef GEOMETRY_VECTOR_HPP
#define GEOMETRY_VECTOR_HPP

namespace Geometry
{
	/// A 3D displacement vector represented by (dx, dy, dz) components.
	/// Vectors are value types: copyable, default-constructible to the zero vector.
	class Vector
	{
		public:
			constexpr Vector() noexcept = default;
			constexpr Vector(double dx, double dy, double dz) noexcept;

			[[nodiscard]] static Vector unit_x() noexcept; ///< Unit vector along the x axis.
			[[nodiscard]] static Vector unit_y() noexcept; ///< Unit vector along the y axis.
			[[nodiscard]] static Vector unit_z() noexcept; ///< Unit vector along the z axis.

			[[nodiscard]] constexpr double dx() const noexcept; ///< x component.
			[[nodiscard]] constexpr double dy() const noexcept; ///< y component.
			[[nodiscard]] constexpr double dz() const noexcept; ///< z component.

			/// Euclidean length (magnitude) of the vector.
			[[nodiscard]] double length() const noexcept;

			/// Squared length — cheaper than length() for comparisons.
			[[nodiscard]] double length_squared() const noexcept;

			/// Normalises the vector in-place. Throws if the vector is zero or non-finite.
			void normalize();

			/// Returns a normalised copy. Throws if the vector is zero or non-finite.
			[[nodiscard]] Vector normalized() const;

			[[nodiscard]] bool isFinite() const noexcept; ///< True if all components are finite.
			[[nodiscard]] bool isZero() const noexcept; ///< True if all components are zero within tolerance.
			[[nodiscard]] bool isValidDirection() const noexcept; ///< True if finite and non-zero (safe to normalise).

			/// Dot product with \p v.
			[[nodiscard]] double dot(Vector const& v) const noexcept;

			/// Cross product: *this × v.
			[[nodiscard]] Vector cross(Vector const& v) const noexcept;

			/// Angle in radians between this vector and \p v, in [0, π].
			[[nodiscard]] double angle(Vector const& v) const;

			/// Unary negation.
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