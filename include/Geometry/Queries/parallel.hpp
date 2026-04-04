#ifndef GEOMETRY_PARALLEL_HPP
#define GEOMETRY_PARALLEL_HPP

#include "Geometry/Primitives/line.hpp"
#include "Geometry/Primitives/plane.hpp"

namespace Geometry
{
	/// Returns true if the two lines are parallel (or anti-parallel).
	/// Uses a scale-independent sine test: |d₁ × d₂|² / (|d₁|²|d₂|²) < k_abs_eps².
	[[nodiscard]] bool is_parallel(Line const& a, Line const& b) noexcept;

	/// Returns true if the two planes are parallel (or coincident).
	/// Since plane normals are unit vectors: |n₁ × n₂|² < k_abs_eps².
	[[nodiscard]] bool is_parallel(Plane const& a, Plane const& b) noexcept;

} // namespace Geometry

#endif // GEOMETRY_PARALLEL_HPP
