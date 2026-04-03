#ifndef NUMERICAL_UTILS_HPP
#define NUMERICAL_UTILS_HPP

namespace Geometry::utils
{
	[[nodiscard]] bool
	almost_equal (double a, double b, double abs_eps = 1e-12, double rel_eps = 1e-9);
}

#endif // NUMERICAL_UTILS_HPP