#include "Geometry/Queries/parallel.hpp"

#include "utils/numerical_utils.hpp"

namespace Geometry
{
	bool is_parallel(Line const& a, Line const& b) noexcept
	{
		Vector cross = a.direction().cross(b.direction());

		// Scale-independent sine test:
		//   sin²(θ) = |d₁ × d₂|² / (|d₁|²|d₂|²) < k_abs_eps²
		return cross.length_squared() < utils::k_abs_eps * utils::k_abs_eps *
		a.direction().length_squared() * b.direction().length_squared();
	}

	bool is_parallel(Plane const& a, Plane const& b) noexcept
	{
		// Plane normals are always unit vectors, so:
		//   |n₁ × n₂|² = sin²(θ) < k_abs_eps²
		Vector cross = a.normal().cross(b.normal());
		return cross.length_squared() < utils::k_abs_eps * utils::k_abs_eps;
	}

	bool is_parallel(Ray const& a, Ray const& b) noexcept
	{
		Vector cross = a.direction().cross(b.direction());
		return cross.length_squared() < utils::k_abs_eps * utils::k_abs_eps *
		a.direction().length_squared() * b.direction().length_squared();
	}

	bool is_parallel(Ray const& ray, Plane const& plane) noexcept
	{
		// The ray is parallel to the plane when its direction is perpendicular
		// to the plane normal (dot product ≈ 0).
		// Scale-independent test: denom² < k_rel_eps² · |d|²
		double const denom = plane.normal().dot(ray.direction());
		return denom * denom <
		utils::k_rel_eps * utils::k_rel_eps * ray.direction().length_squared();
	}

} // namespace Geometry
