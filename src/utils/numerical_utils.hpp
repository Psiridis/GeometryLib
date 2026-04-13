#ifndef NUMERICAL_UTILS_HPP
#define NUMERICAL_UTILS_HPP

#include "Geometry/Settings/tolerance.hpp"

namespace Geometry::utils
{
	/// Implementation-private aliases for the public tolerance constants.
	/// All threshold comparisons in .cpp files must use these names;
	/// the shared values are defined in Geometry/Settings/tolerance.hpp.
	inline constexpr double k_abs_eps = Geometry::tolerance::k_abs_eps;
	inline constexpr double k_rel_eps = Geometry::tolerance::k_rel_eps;

	[[nodiscard]] bool
	almost_equal(double a, double b, double abs_eps = k_abs_eps, double rel_eps = k_rel_eps);

	/// Clamps x to [-1, 1] before calling std::acos to prevent NaN from
	/// floating-point rounding that pushes the value just outside the valid domain.
	[[nodiscard]] double safe_acos(double x) noexcept;
} // namespace Geometry::utils

#endif // NUMERICAL_UTILS_HPP