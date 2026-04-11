#ifndef GEOMETRY_PARALLEL_HPP
#define GEOMETRY_PARALLEL_HPP

#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/ray.hpp"

namespace Geometry
{
	/// Returns true if the two lines are parallel (or anti-parallel).
	/// Uses a scale-independent sine test: |d₁ × d₂|² / (|d₁|²|d₂|²) < k_abs_eps².
	[[nodiscard]] bool is_parallel(Line const& a, Line const& b) noexcept;

	/// Returns true if the two planes are parallel (or coincident).
	/// Since plane normals are unit vectors: |n₁ × n₂|² < k_abs_eps².
	[[nodiscard]] bool is_parallel(Plane const& a, Plane const& b) noexcept;

	/// Returns true if the two rays are parallel (or anti-parallel).
	/// Uses the same scale-independent sine test as the Line overload.
	[[nodiscard]] bool is_parallel(Ray const& a, Ray const& b) noexcept;

	/// Returns true if the ray direction is parallel to the plane
	/// (i.e. the ray cannot intersect the plane).
	[[nodiscard]] bool is_parallel(Ray const& ray, Plane const& plane) noexcept;

} // namespace Geometry

#endif // GEOMETRY_PARALLEL_HPP
