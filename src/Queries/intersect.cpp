#include "Geometry/Queries/intersect.hpp"

#include "Geometry/Queries/parallel.hpp"

#include <cmath>

#include "utils/numerical_utils.hpp"

namespace Geometry
{
	// ── Ray ∩ Plane ───────────────────────────────────────────────────────────
	//
	// Ray:   P(t) = origin + t * direction,  t ≥ 0
	// Plane: n · (P(t) − p₀) = 0   for any point P(t) on the plane
	// Expanding P(t):
	//  → t  = −signed_distance(origin) / (n · direction)
	//
	std::optional<Point> intersect(Ray const& ray, Plane const& plane)
	{
		double denom = plane.normal().dot(ray.direction());

		if (utils::almost_equal(denom, 0.0))
			return std::nullopt; // ray is parallel to (or lies in) the plane

		double t = -plane.signed_distance(ray.origin()) / denom;

		if (t < 0.0)
			return std::nullopt; // intersection is behind the ray origin

		return ray.origin() + t * ray.direction();
	}

	// ── Line ∩ Plane ──────────────────────────────────────────────────────────
	//
	// Same formula as Ray ∩ Plane but without the t ≥ 0 constraint.
	//
	std::optional<Point> intersect(Line const& line, Plane const& plane)
	{
		double denom = plane.normal().dot(line.direction());

		if (utils::almost_equal(denom, 0.0))
			return std::nullopt; // line is parallel to (or lies in) the plane

		double t = -plane.signed_distance(line.origin()) / denom;

		return line.at(t);
	}

	// ── Plane ∩ Plane ─────────────────────────────────────────────────────────
	//
	// Direction of intersection line: d = n₁ × n₂
	// A point on the line (closest to world origin) is derived by solving:
	//   n₁ · X = c₁,  n₂ · X = c₂,  d · X = 0
	// where  cᵢ = nᵢ · pᵢ = −signed_distance(origin) for each plane.
	//
	// Closed-form solution:
	//   p = (c₁ (n₂ × d) − c₂ (n₁ × d)) / |d|²
	//
	std::optional<Line> intersect(Plane const& plane1, Plane const& plane2)
	{
		if (is_parallel(plane1, plane2))
			return std::nullopt; // planes are parallel or coincident

		Vector d		= plane1.normal().cross(plane2.normal());
		double len2 = d.length_squared();

		// c₁ = n₁ · p₁  (plane equation constant, recovered via signed_distance)
		double c1 = -plane1.signed_distance(Point{});
		double c2 = -plane2.signed_distance(Point{});

		Vector p_vec =
		(c1 * plane2.normal().cross(d) - c2 * plane1.normal().cross(d)) * (1.0 / len2);

		return Line(Point{} + p_vec, d);
	}

	// ── Line ∩ Line ───────────────────────────────────────────────────────────
	//
	// Lines: P₁(s) = o₁ + s d₁,  P₂(t) = o₂ + t d₂
	// Let   cross = d₁ × d₂,  w = o₂ − o₁
	//
	// Parallel:  |cross|² ≈ 0  → nullopt
	// Skew:      |w · cross| / |cross| > ε  → nullopt
	// Intersect: s = (w × d₂) · cross / |cross|²,  return P₁(s)
	//
	std::optional<Point> intersect(Line const& line1, Line const& line2)
	{
		if (is_parallel(line1, line2))
			return std::nullopt; // lines are parallel (or anti-parallel)

		Vector cross = line1.direction().cross(line2.direction());
		double len2	 = cross.length_squared();

		Vector w				= line2.origin() - line1.origin();
		double skew_num = w.dot(cross); // = w · (d₁ × d₂)

		// Skew distance = |skew_num| / |cross|.  Threshold: k_rel_eps × |cross|.
		if (std::abs(skew_num) > utils::k_rel_eps * std::sqrt(len2))
			return std::nullopt; // lines are skew

		double s = w.cross(line2.direction()).dot(cross) / len2;

		return line1.at(s);
	}

} // namespace Geometry
