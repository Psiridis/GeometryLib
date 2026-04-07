#ifndef GEOMETRY_LINE_HPP
#define GEOMETRY_LINE_HPP

#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

namespace Geometry
{
	/// An infinite line in 3D space defined by a point on the line and a unit direction.
	/// Throws std::invalid_argument if the direction vector is zero or non-finite.
	class Line
	{
		public:
			/// Constructs a line through \p origin in \p direction (need not be unit length).
			Line(Point const& origin, Vector const& direction);

			/// Constructs the line through two distinct points \p p1 and \p p2.
			Line(Point const& p1, Point const& p2);

			[[nodiscard]] Point const& origin() const noexcept; ///< A point on the line.
			[[nodiscard]] Vector const& direction() const noexcept; ///< Unit direction vector.

			/// Returns the point at parameter \p t: origin + t * direction.
			[[nodiscard]] Point at(double t) const noexcept;

		private:
			Point m_origin{};
			Vector m_direction{};
	};
} // namespace Geometry

#endif // GEOMETRY_LINE_HPP