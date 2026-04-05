#ifndef GEOMETRY_DISTANCE_HPP
#define GEOMETRY_DISTANCE_HPP

#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"

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

} // namespace Geometry

#endif // GEOMETRY_DISTANCE_HPP
