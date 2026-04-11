#ifndef GEOMETRY_DISTANCE_HPP
#define GEOMETRY_DISTANCE_HPP

#include "Geometry/Bounds/bounding_box.hpp"
#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Shapes/segment.hpp"
#include "Geometry/Shapes/triangle.hpp"

namespace Geometry
{
	/// Returns the shortest distance from the point to the line.
	///   d = |P − project(P, line)|
	[[nodiscard]] double distance(Point const& point, Line const& line) noexcept;

	/// Returns the shortest distance from the point to the plane.
	/// Delegates to Plane::distance(), which returns |signed_distance(P)|.
	[[nodiscard]] double distance(Point const& point, Plane const& plane) noexcept;

	/// Returns the shortest distance between two lines.
	///
	/// Three cases handled:
	///   Parallel lines:      perpendicular distance from b.origin() to line a.
	///   Intersecting lines:  0 (the scalar triple product formula gives this
	///   naturally). Skew lines:          |(o₂ − o₁) · (d₁ × d₂)| / |d₁ × d₂|
	[[nodiscard]] double distance(Line const& a, Line const& b) noexcept;

	/// Returns the shortest distance from the point to the segment.
	/// Projects the point onto the infinite line, clamps t to [0, 1], then
	/// measures to the resulting boundary point.
	[[nodiscard]] double distance(Point const& point, Segment const& segment) noexcept;

	/// Returns the shortest distance from the point to the triangle surface.
	/// If the projection of the point onto the triangle plane lies inside the
	/// triangle, the distance is equal to the point-to-plane distance.
	/// Otherwise it is the distance to the nearest edge or vertex.
	[[nodiscard]] double distance(Point const& point, Triangle const& triangle) noexcept;

	/// Returns the shortest distance from the point to the axis-aligned bounding
	/// box. Returns 0 if the point is inside or on the boundary of the box.
	[[nodiscard]] double distance(Point const& point, BoundingBox const& box) noexcept;

	/// Returns the shortest distance between two segments.
	[[nodiscard]] double distance(Segment const& a, Segment const& b) noexcept;

} // namespace Geometry

#endif // GEOMETRY_DISTANCE_HPP
