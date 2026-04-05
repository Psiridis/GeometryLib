#include "Geometry/Queries/distance.hpp"

#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Queries/parallel.hpp"
#include "Geometry/Queries/project.hpp"

#include <cmath>

namespace Geometry
{
	// ── distance(Point, Line) ─────────────────────────────────────────────────
	//
	// Project P onto the line to find the foot of the perpendicular, then
	// return the length of the vector from the foot back to P:
	//
	//   d = |P − project(P, line)|
	//
	double distance(Point const& point, Line const& line) noexcept
	{
		return (point - project(point, line)).length();
	}

	// ── distance(Point, Plane) ────────────────────────────────────────────────
	//
	// The perpendicular distance from a point to a plane is the absolute value
	// of the signed distance.  Plane::distance() already returns |signed_distance(P)|.
	//
	double distance(Point const& point, Plane const& plane) noexcept
	{
		return plane.distance(point);
	}

	// ── distance(Line, Line) ──────────────────────────────────────────────────
	//
	// Three cases:
	//
	//   Parallel:     the lines have no crossing point; use point-to-line
	//                 distance from b.origin() to a.
	//
	//   Intersecting: the scalar triple product is zero → d = 0.
	//
	//   Skew:         the shortest connector is perpendicular to both directions.
	//                 Its length is the scalar projection of (o₂ − o₁) onto
	//                 the common perpendicular d₁ × d₂:
	//
	//                   d = |(o₂ − o₁) · (d₁ × d₂)| / |d₁ × d₂|
	//
	double distance(Line const& a, Line const& b) noexcept
	{
		if (is_parallel(a, b))
			return distance(b.origin(), a);

		Vector const cross = a.direction().cross(b.direction());
		Vector const w		 = b.origin() - a.origin();
		return std::abs(w.dot(cross)) / cross.length();
	}

} // namespace Geometry
