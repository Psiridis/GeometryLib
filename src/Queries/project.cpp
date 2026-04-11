#include "Geometry/Queries/project.hpp"

#include <algorithm>

#include "utils/numerical_utils.hpp"

namespace Geometry
{
	// ── project(Point, Line) ──────────────────────────────────────────────────
	//
	// The projection of P onto a line is the foot of the perpendicular from P:
	//
	//   t  = (P − origin) · direction / |direction|²
	//   P' = origin + t * direction
	//
	// Dividing by |direction|² rather than normalizing keeps the direction
	// un-modified (Line does not require a unit direction).
	//
	Point project(Point const& point, Line const& line) noexcept
	{
		Vector const w = point - line.origin();
		double const t = w.dot(line.direction()) / line.direction().length_squared();
		return line.at(t);
	}

	// ── project(Point, Ray) ───────────────────────────────────────────────────────
	//
	// Same as the line projection but t is clamped to [0, ∞).
	// When t < 0 the perpendicular foot lies behind the ray origin,
	// so the closest point on the ray is the origin itself.
	//
	//   t  = max(0, (P − origin) · direction / |direction|²)
	//   P' = origin + t * direction
	//
	Point project(Point const& point, Ray const& ray) noexcept
	{
		Vector const w = point - ray.origin();
		double const t =
		std::max(0.0, w.dot(ray.direction()) / ray.direction().length_squared());
		return ray.origin() + t * ray.direction();
	}

	// ── project(Point, Plane) ─────────────────────────────────────────────────
	//
	// The projection of P onto a plane is obtained by moving P along the unit
	// normal by its signed distance from the plane:
	//
	//   P' = P − signed_distance(P) * n
	//
	Point project(Point const& point, Plane const& plane) noexcept
	{
		return point - plane.signed_distance(point) * plane.normal();
	}

	// ── project(Vector, Plane) ─────────────────────────────────────────────────
	//
	// Remove the component of v along the plane normal, keeping only the
	// tangential part:
	//
	//   v' = v − (v · n) * n
	//
	// Returns the zero vector when v is parallel to the normal (the vector has
	// no component within the plane).
	//
	Vector project(Vector const& vec, Plane const& plane) noexcept
	{
		return vec - vec.dot(plane.normal()) * plane.normal();
	}

	// ── project(Line, Plane) ──────────────────────────────────────────────────
	//
	// Project the origin point onto the plane, then remove the normal component
	// from the direction so it lies flat in the plane:
	//
	//   origin' = project(origin, plane)
	//   d'      = d − (d · n) * n
	//
	// Degenerate: if |d'|² ≈ 0, the line is perpendicular to the plane and
	// collapses to a single point → return std::nullopt.
	//
	std::optional<Line> project(Line const& line, Plane const& plane)
	{
		Vector const d_proj =
		line.direction() - line.direction().dot(plane.normal()) * plane.normal();

		if (d_proj.length_squared() < utils::k_abs_eps * utils::k_abs_eps)
			return std::nullopt; // line is perpendicular to the plane

		Point const origin_proj = project(line.origin(), plane);

		return Line(origin_proj, d_proj);
	}

	// ── project(Point, Segment) ───────────────────────────────────────────────
	//
	// Project P onto the infinite line through the segment, then clamp t to [0, 1]:
	//
	//   d  = end − start
	//   t  = clamp((P − start) · d / |d|², 0, 1)
	//   P' = start + t * d
	//
	Point project(Point const& point, Segment const& segment) noexcept
	{
		Vector const d = segment.end() - segment.start();
		double const t =
		std::clamp((point - segment.start()).dot(d) / d.length_squared(), 0.0, 1.0);
		return segment.start() + t * d;
	}

} // namespace Geometry
