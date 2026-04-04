#ifndef GEOMETRY_LINE_HPP
#define GEOMETRY_LINE_HPP

#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

namespace Geometry
{
	class Line
	{
		public:
			Line(Point const& origin, Vector const& direction);
			Line(Point const& p1, Point const& p2);

			[[nodiscard]] Point const& origin() const noexcept;
			[[nodiscard]] Vector const& direction() const noexcept;
			[[nodiscard]] Point at(double t) const noexcept;

		private:
			Point m_origin{};
			Vector m_direction{};
	};
} // namespace Geometry

#endif // GEOMETRY_LINE_HPP