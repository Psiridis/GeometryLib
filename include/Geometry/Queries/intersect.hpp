#ifndef GEOMETRY_INTERSECT_HPP
#define GEOMETRY_INTERSECT_HPP

#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/ray.hpp"
#include "Geometry/Shapes/triangle.hpp"

#include <optional>

namespace Geometry
{
	/// Returns the intersection point of a ray and a plane, or std::nullopt if
	/// the ray is parallel to the plane or the intersection is behind the origin.
	[[nodiscard]] std::optional<Point> intersect(Ray const& ray, Plane const& plane);

	/// Returns the intersection point of a line and a plane, or std::nullopt if
	/// the line is parallel to (or lies in) the plane.
	[[nodiscard]] std::optional<Point> intersect(Line const& line, Plane const& plane);

	/// Returns the line of intersection of two planes, or std::nullopt if the
	/// planes are parallel (or coincident).
	[[nodiscard]] std::optional<Line> intersect(Plane const& plane1, Plane const& plane2);

	/// Returns the intersection point of two lines, or std::nullopt if the
	/// lines are parallel or skew (non-coplanar).
	[[nodiscard]] std::optional<Point> intersect(Line const& line1, Line const& line2);

	/// Returns the intersection point of a ray and a triangle using the
	/// Möller–Trumbore algorithm, or std::nullopt if:
	///   - the ray is parallel to the triangle plane, or
	///   - the intersection point lies outside the triangle, or
	///   - the intersection is behind the ray origin (t < 0).
	/// Back-face hits (ray approaching from the side opposite the triangle
	/// normal) are included; pass the result through Triangle::contains() or
	/// check the normal if one-sided behaviour is required.
	[[nodiscard]] std::optional<Point> intersect(Ray const& ray, Triangle const& triangle);

} // namespace Geometry

#endif // GEOMETRY_INTERSECT_HPP
