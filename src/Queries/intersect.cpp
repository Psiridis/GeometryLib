#include "Geometry/Queries/intersect.hpp"

#include "Geometry/Queries/parallel.hpp"

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

		// Scale-independent: sin²(angle) = (n·d)² / |d|² < k_rel_eps²
		// Since n is unit this reduces to: denom² < k_rel_eps² · |d|²
		if (denom * denom < utils::k_rel_eps * utils::k_rel_eps * ray.direction().length_squared())
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

		// Scale-independent: sin²(angle) = (n·d)² / |d|² < k_rel_eps²
		// Since n is unit this reduces to: denom² < k_rel_eps² · |d|²
		if (denom * denom < utils::k_rel_eps * utils::k_rel_eps * line.direction().length_squared())
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

		Vector const w				= line2.origin() - line1.origin();
		double const skew_num = w.dot(cross); // = w · (d₁ × d₂)

		// Scale-independent skew test:
		//   skew_distance = |skew_num| / |cross|
		//   Normalise by |w| to get a dimensionless ratio:
		//     (skew_distance / |w|)² < k_rel_eps²
		//   ↔ skew_num² < k_rel_eps² · |cross|² · |w|²
		// When |w| ≈ 0 (coincident origins) both sides → 0, test passes correctly.
		double const w_len2 = w.length_squared();
		if (skew_num * skew_num > utils::k_rel_eps * utils::k_rel_eps * len2 * w_len2)
			return std::nullopt; // lines are skew

		double s = w.cross(line2.direction()).dot(cross) / len2;

		return line1.at(s);
	}

	// ── Ray ∩ Triangle (Möller–Trumbore) ──────────────────────────────────────
	//
	// Reference:
	//   Möller, T. & Trumbore, B. (1997). "Fast, Minimum Storage Ray/Triangle
	//   Intersection." Journal of Graphics Tools, 2(1), 21–28.
	//   https://doi.org/10.1080/10867651.1997.10487468
	//
	// Ray:      P(t) = O + t D,    t ≥ 0
	// Triangle: vertices V0, V1, V2;  edges E1 = V1−V0, E2 = V2−V0
	//
	// Derivation (solve O + tD = V0 + u E1 + v E2):
	//   h  = D × E2
	//   a  = E1 · h            ← denominator; ≈ 0 → ray parallel to triangle
	//   f  = 1 / a
	//   s  = O − V0
	//   u  = f (s · h)         ← 1st barycentric coord; must be in [0, 1]
	//   q  = s × E1
	//   v  = f (D · q)         ← 2nd barycentric coord; must be in [0, 1−u]
	//   t  = f (E2 · q)        ← ray parameter; must be ≥ 0
	//
	// Both front-face and back-face hits are reported (two-sided test).
	//
	std::optional<Point> intersect(Ray const& ray, Triangle const& triangle)
	{
		Vector const e1 = triangle.p1() - triangle.p0();
		Vector const e2 = triangle.p2() - triangle.p0();

		Vector const h = ray.direction().cross(e2);
		double const a = e1.dot(h);

		// Parallel test: |a| < eps * |E1| * |D| (scale-independent)
		if (a * a < utils::k_rel_eps * utils::k_rel_eps * e1.length_squared() *
				ray.direction().length_squared())
			return std::nullopt;

		double const f = 1.0 / a;

		Vector const s = ray.origin() - triangle.p0();
		double const u = f * s.dot(h);
		if (u < 0.0 || u > 1.0)
			return std::nullopt;

		Vector const q = s.cross(e1);
		double const v = f * ray.direction().dot(q);
		if (v < 0.0 || u + v > 1.0)
			return std::nullopt;

		double const t = f * e2.dot(q);
		if (t < 0.0)
			return std::nullopt;

		return ray.origin() + t * ray.direction();
	}

} // namespace Geometry
