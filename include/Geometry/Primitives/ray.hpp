#ifndef GEOMETRY_RAY_HPP
#define GEOMETRY_RAY_HPP

#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

namespace Geometry
{
	class Ray
	{
		public:
			Ray (Point const& origin, Vector const& direction);
			Ray (Point const& origin, Point const& point_on_ray);

			[[nodiscard]] Point const& origin () const noexcept;
			[[nodiscard]] Vector const& direction () const noexcept;
			[[nodiscard]] Point at (double t) const;

		private:
			Point m_origin{};
			Vector m_direction{};
	};
} // namespace Geometry

#endif // GEOMETRY_RAY_HPP
