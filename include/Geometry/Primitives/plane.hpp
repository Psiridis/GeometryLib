#ifndef GEOMETRY_PLANE_HPP
#define GEOMETRY_PLANE_HPP

#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Settings/tolerance.hpp"

namespace Geometry
{
	/// An infinite plane in 3D space defined by a point on the plane and a unit normal.
	/// Throws std::invalid_argument if the normal vector is zero or non-finite,
	/// or (for the three-point constructor) if the points are collinear.
	class Plane
	{
		public:
			/// Constructs a plane through \p point with the given \p normal (need not be unit length).
			Plane(Point const& point, Vector const& normal);

			/// Constructs the plane through three non-collinear points.
			/// The normal follows the right-hand rule: n = (p2−p1) × (p3−p1), normalised.
			Plane(Point const& p1, Point const& p2, Point const& p3);

			[[nodiscard]] Vector const& normal() const noexcept; ///< Unit normal vector.

			/// Signed distance from the plane to \p p along the normal direction.
			/// Positive when \p p is on the same side as the normal, negative otherwise.
			[[nodiscard]] double signed_distance(Point const& p) const noexcept;

			/// Absolute (unsigned) distance from the plane to \p p.
			[[nodiscard]] double distance(Point const& p) const noexcept;

			/// Returns true if \p p lies on the plane within the given tolerances.
			[[nodiscard]] bool contains(Point const& p,
																	double abs_eps = tolerance::k_abs_eps,
																	double rel_eps = tolerance::k_rel_eps) const noexcept;

		private:
			Point m_point{};
			Vector m_normal{};
	};
} // namespace Geometry

#endif // GEOMETRY_PLANE_HPP