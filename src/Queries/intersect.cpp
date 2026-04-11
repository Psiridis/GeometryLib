#include "Geometry/Queries/intersect.hpp"

#include "Geometry/Queries/parallel.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

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
		if (is_parallel(line, plane))
			return std::nullopt; // line is parallel to (or lies in) the plane

		double const denom = plane.normal().dot(line.direction());
		double const t		 = -plane.signed_distance(line.origin()) / denom;

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

	// ── Ray ∩ BoundingBox (slab method) ───────────────────────────────────────
	//
	// Reference:
	//   Williams, A., Barrus, S., Morley, R. K., & Shirley, P. (2005).
	//   "An efficient and robust ray-box intersection algorithm."
	//   Journal of Graphics Tools, 10(1), 49–54.
	//   https://doi.org/10.1080/2151237X.2005.10129188
	//
	// For each axis i ∈ {x, y, z} the ray intersects two parallel planes
	// ("slabs") at:
	//   t_near_i = (box.min[i] − origin[i]) / direction[i]
	//   t_far_i  = (box.max[i] − origin[i]) / direction[i]
	//
	// The ray is inside the slab when t ∈ [t_near_i, t_far_i].
	// The ray is inside all three slabs simultaneously when
	//   t_entry = max(t_near_x, t_near_y, t_near_z) ≤
	//   t_exit  = min(t_far_x,  t_far_y,  t_far_z)
	//
	// Special cases:
	//   - Direction component == 0: ray is parallel to that pair of slabs.
	//     If the origin is outside them → miss (t_near = +∞, t_exit → -∞).
	//     Use IEEE 754 ±∞ from division-by-zero: handled correctly by
	//     std::min/max because ∞ comparisons are well-defined.
	//   - t_exit < 0: box is entirely behind the ray origin → nullopt.
	//   - t_entry < 0 ≤ t_exit: origin is inside the box → return origin (t=0).
	//
	std::optional<Point> intersect(Ray const& ray, BoundingBox const& box)
	{
		double t_entry = 0.0; // clamped: never return a point behind origin
		double t_exit	 = std::numeric_limits<double>::infinity();

		auto const& o = ray.origin();
		auto const& d = ray.direction();

		// X slab
		{
			double const inv = 1.0 / d.dx();
			double t1				 = (box.min().x() - o.x()) * inv;
			double t2				 = (box.max().x() - o.x()) * inv;
			if (t1 > t2)
				std::swap(t1, t2);
			t_entry = std::max(t_entry, t1);
			t_exit	= std::min(t_exit, t2);
		}
		if (t_entry > t_exit)
			return std::nullopt;

		// Y slab
		{
			double const inv = 1.0 / d.dy();
			double t1				 = (box.min().y() - o.y()) * inv;
			double t2				 = (box.max().y() - o.y()) * inv;
			if (t1 > t2)
				std::swap(t1, t2);
			t_entry = std::max(t_entry, t1);
			t_exit	= std::min(t_exit, t2);
		}
		if (t_entry > t_exit)
			return std::nullopt;

		// Z slab
		{
			double const inv = 1.0 / d.dz();
			double t1				 = (box.min().z() - o.z()) * inv;
			double t2				 = (box.max().z() - o.z()) * inv;
			if (t1 > t2)
				std::swap(t1, t2);
			t_entry = std::max(t_entry, t1);
			t_exit	= std::min(t_exit, t2);
		}
		if (t_entry > t_exit)
			return std::nullopt;

		// t_exit < 0 means the box is entirely behind the ray — already excluded
		// by the t_entry = 0 clamp.  t_exit == 0 is a grazing touch on a face
		// at the ray origin: still a valid hit.
		return ray.origin() + t_entry * ray.direction();
	}

	// ── Segment ∩ Plane ────────────────────────────────────────────────────────
	//
	// Parameterise the segment as P(t) = start + t*(end − start), t ∈ [0, 1].
	// Intersect with the plane exactly as Ray ∩ Plane, but reject if t ∉ [0, 1].
	//
	std::optional<Point> intersect(Segment const& segment, Plane const& plane)
	{
		if (is_parallel(segment, plane))
			return std::nullopt;

		Vector const d		 = segment.end() - segment.start();
		double const denom = plane.normal().dot(d);
		double const t		 = -plane.signed_distance(segment.start()) / denom;

		if (t < 0.0 || t > 1.0)
			return std::nullopt;

		return segment.start() + t * d;
	}

	// ── Segment ∩ Triangle ────────────────────────────────────────────────────
	//
	// Clip the segment to the triangle plane to find the crossing parameter t,
	// then verify the resulting point lies inside the triangle.
	//
	std::optional<Point> intersect(Segment const& segment, Triangle const& triangle)
	{
		auto hit = intersect(segment, triangle.plane());
		if (!hit)
			return std::nullopt;

		if (!triangle.contains(*hit))
			return std::nullopt;

		return hit;
	}

	// ── Segment ∩ BoundingBox (slab method, t ∈ [0, 1]) ──────────────────────
	//
	// Identical to Ray ∩ BoundingBox but the segment has a finite length.
	// The slab intervals are computed in terms of the unnormalised direction
	// d = end − start; t=0 is the start, t=1 is the end.
	// A hit requires t_entry ≤ t_exit and both within [0, 1].
	//
	std::optional<Point> intersect(Segment const& segment, BoundingBox const& box)
	{
		Vector const d = segment.end() - segment.start();
		auto const& o	 = segment.start();

		double t_entry = 0.0;
		double t_exit	 = 1.0;

		// X slab
		{
			double const inv = 1.0 / d.dx();
			double t1				 = (box.min().x() - o.x()) * inv;
			double t2				 = (box.max().x() - o.x()) * inv;
			if (t1 > t2)
				std::swap(t1, t2);
			t_entry = std::max(t_entry, t1);
			t_exit	= std::min(t_exit, t2);
		}
		if (t_entry > t_exit)
			return std::nullopt;

		// Y slab
		{
			double const inv = 1.0 / d.dy();
			double t1				 = (box.min().y() - o.y()) * inv;
			double t2				 = (box.max().y() - o.y()) * inv;
			if (t1 > t2)
				std::swap(t1, t2);
			t_entry = std::max(t_entry, t1);
			t_exit	= std::min(t_exit, t2);
		}
		if (t_entry > t_exit)
			return std::nullopt;

		// Z slab
		{
			double const inv = 1.0 / d.dz();
			double t1				 = (box.min().z() - o.z()) * inv;
			double t2				 = (box.max().z() - o.z()) * inv;
			if (t1 > t2)
				std::swap(t1, t2);
			t_entry = std::max(t_entry, t1);
			t_exit	= std::min(t_exit, t2);
		}
		if (t_entry > t_exit)
			return std::nullopt;

		return segment.start() + t_entry * d;
	}

} // namespace Geometry
