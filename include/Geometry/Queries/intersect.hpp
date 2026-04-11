#ifndef GEOMETRY_INTERSECT_HPP
#define GEOMETRY_INTERSECT_HPP

#include "Geometry/Bounds/bounding_box.hpp"
#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/ray.hpp"
#include "Geometry/Shapes/segment.hpp"
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

	/// Returns the entry point of a ray into an axis-aligned bounding box using
	/// the slab method, or std::nullopt if the ray misses the box entirely or
	/// the box is entirely behind the ray origin.
	///
	/// When the ray origin is inside the box the returned point is the ray
	/// origin itself (t_entry is clamped to 0).
	///
	/// A ray that grazes an edge or corner (t_entry == t_exit) is counted as a
	/// hit — the returned point lies exactly on the boundary.
	[[nodiscard]] std::optional<Point> intersect(Ray const& ray, BoundingBox const& box);

	/// Returns the intersection point of a segment and a plane, or std::nullopt
	/// if the segment is parallel to the plane or the intersection lies outside
	/// the segment (t outside [0, 1]).
	[[nodiscard]] std::optional<Point> intersect(Segment const& segment, Plane const& plane);

	/// Returns the intersection point of a segment and a triangle, or std::nullopt
	/// if the segment misses the triangle or does not cross its plane within [0, 1].
	[[nodiscard]] std::optional<Point>
	intersect(Segment const& segment, Triangle const& triangle);

	/// Returns the entry point of a segment into an axis-aligned bounding box,
	/// or std::nullopt if the segment misses the box entirely.
	/// Uses the slab method with t clamped to [0, 1].
	[[nodiscard]] std::optional<Point>
	intersect(Segment const& segment, BoundingBox const& box);

} // namespace Geometry

#endif // GEOMETRY_INTERSECT_HPP
