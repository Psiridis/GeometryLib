#ifndef NUMERICAL_UTILS_HPP
#define NUMERICAL_UTILS_HPP

namespace Geometry::utils
{
	/// Default absolute and relative tolerances used throughout the library.
	/// All threshold constants in other modules must be derived from these.
	inline constexpr double k_abs_eps = 1e-12;
	inline constexpr double k_rel_eps = 1e-9;

	[[nodiscard]] bool
	almost_equal(double a, double b, double abs_eps = k_abs_eps, double rel_eps = k_rel_eps);

	/// Clamps x to [-1, 1] before calling std::acos to prevent NaN from
	/// floating-point rounding that pushes the value just outside the valid domain.
	[[nodiscard]] double safe_acos(double x) noexcept;
} // namespace Geometry::utils

#endif // NUMERICAL_UTILS_HPP