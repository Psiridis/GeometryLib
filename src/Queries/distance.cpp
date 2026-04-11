#include "Geometry/Queries/distance.hpp"

#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Queries/parallel.hpp"
#include "Geometry/Queries/project.hpp"

#include <algorithm>
#include <cmath>

#include "utils/numerical_utils.hpp"

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

	// ── distance(Point, Segment) ──────────────────────────────────────────────
	//
	// Project P onto the segment (t clamped to [0, 1]) and return the distance
	// from P to that closest point.
	//
	double distance(Point const& point, Segment const& segment) noexcept
	{
		return (point - project(point, segment)).length();
	}

	// ── distance(Point, Triangle) ─────────────────────────────────────────────
	//
	// 1. Project P onto the triangle's supporting plane.
	// 2. If the projected point lies inside the triangle, the answer is the
	//    point-to-plane distance.
	// 3. Otherwise clamp to the nearest of the three edges, choosing the minimum.
	//
	double distance(Point const& point, Triangle const& triangle) noexcept
	{
		Point const p_plane = project(point, triangle.plane());

		if (triangle.contains(p_plane))
			return distance(point, triangle.plane());

		// Closest point on each edge
		Segment const e0(triangle.p0(), triangle.p1());
		Segment const e1(triangle.p1(), triangle.p2());
		Segment const e2(triangle.p2(), triangle.p0());

		double const d0 = distance(point, e0);
		double const d1 = distance(point, e1);
		double const d2 = distance(point, e2);

		return std::min({ d0, d1, d2 });
	}

	// ── distance(Point, BoundingBox) ──────────────────────────────────────────
	//
	// For each axis, compute the excess distance outside the [min, max] interval.
	// Sum the squared excesses and take the square root.
	// Returns 0 when the point is inside or on the boundary.
	//
	double distance(Point const& point, BoundingBox const& box) noexcept
	{
		auto excess = [](double v, double lo, double hi) noexcept -> double
		{
			if (v < lo)
				return lo - v;
			if (v > hi)
				return v - hi;
			return 0.0;
		};

		double const dx = excess(point.x(), box.min().x(), box.max().x());
		double const dy = excess(point.y(), box.min().y(), box.max().y());
		double const dz = excess(point.z(), box.min().z(), box.max().z());

		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	// ── distance(Segment, Segment) ────────────────────────────────────────────
	//
	// General closest-point algorithm for two finite segments.
	// Find parameters s ∈ [0,1] and t ∈ [0,1] minimising |P(s) − Q(t)|.
	//
	// Based on the parametric closest-point formula for infinite lines:
	//   s = (b·e − c·d) / (a·c − b²)
	//   t = (a·e − b·d) / (a·c − b²)
	// where a=d1·d1, b=d1·d2, c=d2·d2, d=d1·w, e=d2·w, w=P0−Q0.
	// If lines are parallel, clamp s=0 and project.
	//
	double distance(Segment const& a, Segment const& b) noexcept
	{
		Vector const d1 = a.end() - a.start();
		Vector const d2 = b.end() - b.start();
		Vector const w	= a.start() - b.start();

		double const A	 = d1.dot(d1); // |d1|²
		double const B	 = d1.dot(d2);
		double const C	 = d2.dot(d2); // |d2|²
		double const D	 = d1.dot(w);
		double const E	 = d2.dot(w);
		double const det = A * C - B * B;

		double s, t;

		if (det < utils::k_abs_eps * utils::k_abs_eps * A * C)
		{
			// Parallel segments: fix s=0, project b.start onto a
			s = 0.0;
			t = std::clamp(E / C, 0.0, 1.0);
		}
		else
		{
			s = std::clamp((B * E - C * D) / det, 0.0, 1.0);
			t = std::clamp((A * E - B * D) / det, 0.0, 1.0);
		}

		// Re-project to honour boundary clamping on the other segment
		s = std::clamp(((b.start() + t * d2) - a.start()).dot(d1) / A, 0.0, 1.0);
		t = std::clamp(((a.start() + s * d1) - b.start()).dot(d2) / C, 0.0, 1.0);

		Point const closest_a = a.start() + s * d1;
		Point const closest_b = b.start() + t * d2;
		return (closest_a - closest_b).length();
	}

} // namespace Geometry
