#ifndef GEOMETRY_PROJECT_HPP
#define GEOMETRY_PROJECT_HPP

#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/ray.hpp"
#include "Geometry/Primitives/vector.hpp"

#include <optional>

namespace Geometry
{
	/// Returns the closest point on the line to the given point.
	/// This is the foot of the perpendicular dropped from the point onto the line:
	///   p' = origin + ((point − origin) · direction / |direction|²) * direction
	[[nodiscard]] Point project(Point const& point, Line const& line) noexcept;

	/// Returns the closest point on the ray to the given point.
	/// Same as the line projection but t is clamped to [0, ∞):
	/// if the foot falls behind the origin, the closest point is the origin itself.
	[[nodiscard]] Point project(Point const& point, Ray const& ray) noexcept;

	/// Returns the closest point on the plane to the given point.
	/// Equivalent to translating the point along the plane normal by its signed distance:
	///   p' = point − signed_distance(point) * normal
	[[nodiscard]] Point project(Point const& point, Plane const& plane) noexcept;

	/// Projects a vector onto a plane by removing its normal component:
	///   v' = v − (v · n) * n
	/// When the vector is perpendicular to the plane (entirely along the normal),
	/// the tangential component is zero and the zero vector is returned.
	/// Unlike project(Line, Plane), no std::nullopt is needed: the zero vector
	/// is the correct algebraic result and a valid Vector value.
	[[nodiscard]] Vector project(Vector const& vec, Plane const& plane) noexcept;

	/// Projects a line onto a plane.
	/// The origin is projected as a point; the direction has its normal component removed:
	///   origin' = project(origin, plane)
	///   d'      = d − (d · n) * n
	/// Returns std::nullopt when the line is perpendicular to the plane
	/// (direction is parallel to the normal), collapsing to a single point.
	[[nodiscard]] std::optional<Line> project(Line const& line, Plane const& plane);

} // namespace Geometry

#endif // GEOMETRY_PROJECT_HPP
