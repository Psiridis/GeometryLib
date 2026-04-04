#ifndef GEOMETRY_POINT_HPP
#define GEOMETRY_POINT_HPP

namespace Geometry
{
	class Vector;

	class Point
	{
		public:
			constexpr Point() noexcept = default;
			constexpr Point(double x, double y, double z) noexcept;

			constexpr double x() const noexcept;
			constexpr double y() const noexcept;
			constexpr double z() const noexcept;

			[[nodiscard]] double distance(Point const& p) const;
			[[nodiscard]] double distance_squared(Point const& p) const noexcept;

			[[nodiscard]] Vector operator-(Point const& other) const noexcept;

			[[nodiscard]] bool operator==(Point const& p) const noexcept;
			[[nodiscard]] bool operator!=(Point const& p) const noexcept;

			[[nodiscard]] Point operator+(Vector const& v) const noexcept;
			Point& operator+=(Vector const& v) noexcept;

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