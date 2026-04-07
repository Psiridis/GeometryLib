#ifndef GEOMETRY_RAY_HPP
#define GEOMETRY_RAY_HPP

#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

namespace Geometry
{
	/// A semi-infinite ray starting at an origin and extending in a unit direction.
	/// Throws std::invalid_argument if the direction vector is zero or non-finite.
	class Ray
	{
		public:
			/// Constructs a ray from \p origin in \p direction (need not be unit length).
			Ray(Point const& origin, Vector const& direction);

			/// Constructs a ray from \p origin towards \p point_on_ray.
			Ray(Point const& origin, Point const& point_on_ray);

			[[nodiscard]] Point const& origin() const noexcept; ///< Ray origin.
			[[nodiscard]] Vector const& direction() const noexcept; ///< Unit direction vector.

			/// Returns the point at parameter \p t ≥ 0: origin + t * direction.
			/// Throws std::invalid_argument if t is negative.
			[[nodiscard]] Point at(double t) const;

		private:
			Point m_origin{};
			Vector m_direction{};
	};
} // namespace Geometry

#endif // GEOMETRY_RAY_HPP
