#ifndef GEOMETRY_PLANE_HPP
#define GEOMETRY_PLANE_HPP

#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

namespace Geometry
{
	class Plane
	{
		public:
			Plane (Point const& point, Vector const& normal);
			Plane (Point const& p1, Point const& p2, Point const& p3);

			[[nodiscard]] Vector const& normal () const noexcept;

			[[nodiscard]] double signed_distance (Point const& p) const noexcept;
			[[nodiscard]] double distance (Point const& p) const noexcept;
			[[nodiscard]] bool
			contains (Point const& p, double abs_eps = 1e-12, double rel_eps = 1e-9) const noexcept;

		private:
			Point m_point{};
			Vector m_normal{};
	};
} // namespace Geometry

#endif // GEOMETRY_PLANE_HPP