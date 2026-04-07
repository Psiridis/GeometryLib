#ifndef GEOMETRY_POINT_HPP
#define GEOMETRY_POINT_HPP

namespace Geometry
{
	class Vector;

	/// A point in 3D Euclidean space represented by (x, y, z) coordinates.
	/// Points are value types: copyable, default-constructible to the origin.
	class Point
	{
		public:
			constexpr Point() noexcept = default;
			constexpr Point(double x, double y, double z) noexcept;

			constexpr double x() const noexcept; ///< x coordinate.
			constexpr double y() const noexcept; ///< y coordinate.
			constexpr double z() const noexcept; ///< z coordinate.

			/// Euclidean distance to another point.
			[[nodiscard]] double distance(Point const& p) const;

			/// Squared Euclidean distance — cheaper than distance() when only comparison is needed.
			[[nodiscard]] double distance_squared(Point const& p) const noexcept;

			/// Vector from this point to \p other (other - this).
			[[nodiscard]] Vector operator-(Point const& other) const noexcept;

			[[nodiscard]] bool operator==(Point const& p) const noexcept;
			[[nodiscard]] bool operator!=(Point const& p) const noexcept;

			/// Translates this point by vector \p v.
			[[nodiscard]] Point operator+(Vector const& v) const noexcept;
			Point& operator+=(Vector const& v) noexcept;

			/// Translates this point by the negation of vector \p v.
			[[nodiscard]] Point operator-(Vector const& v) const noexcept;
			Point& operator-=(Vector const& v) noexcept;

		private:
			double m_x{ 0.0 };
			double m_y{ 0.0 };
			double m_z{ 0.0 };
	};

	constexpr Point::Point(double x, double y, double z) noexcept
	: m_x(x),
		m_y(y),
		m_z(z)
	{
	}

	constexpr double Point::x() const noexcept
	{
		return m_x;
	}

	constexpr double Point::y() const noexcept
	{
		return m_y;
	}

	constexpr double Point::z() const noexcept
	{
		return m_z;
	}
} // namespace Geometry

#endif // GEOMETRY_POINT_HPP