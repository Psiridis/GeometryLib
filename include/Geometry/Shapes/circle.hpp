#ifndef GEOMETRY_CIRCLE_HPP
#define GEOMETRY_CIRCLE_HPP

#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Settings/tolerance.hpp"

namespace Geometry
{
	/// A finite circle (disc boundary) in 3D space defined by a center point,
	/// a unit normal that determines the supporting plane, and a positive radius.
	///
	/// The circle is the set of all coplanar points at distance exactly `radius`
	/// from `center`.  The filled disc (interior + boundary) is tested by contains().
	///
	/// Throws std::invalid_argument if:
	///   - the normal is zero or non-finite, or
	///   - the radius is non-positive or non-finite.
	class Circle
	{
		public:
			Circle(Point const& center, Vector const& normal, double radius);

			[[nodiscard]] Point const& center() const noexcept;
			[[nodiscard]] Vector const& normal() const noexcept;
			[[nodiscard]] double radius() const noexcept;

			/// Area of the disc:  π r²
			[[nodiscard]] double area() const noexcept;

			/// Circumference of the circle:  2π r
			[[nodiscard]] double circumference() const noexcept;

			/// The infinite plane that contains the circle (same normal and center).
			[[nodiscard]] Plane plane() const;

			/// Returns true if the point lies inside or on the boundary of the disc.
			/// The point must be coplanar with the circle (distance to plane ≈ 0);
			/// a non-coplanar point always returns false.
			/// For non-coplanar points use project(point, plane()) then contains().
			[[nodiscard]] bool contains(Point const& p, double tol = tolerance::k_rel_eps) const noexcept;

			[[nodiscard]] bool operator==(Circle const& other) const noexcept;
			[[nodiscard]] bool operator!=(Circle const& other) const noexcept;

		private:
			Point m_center{};
			Vector m_normal{};
			double m_radius{};
	};
} // namespace Geometry

#endif // GEOMETRY_CIRCLE_HPP
