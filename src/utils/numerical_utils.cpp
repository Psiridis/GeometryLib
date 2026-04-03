#include "utils/numerical_utils.hpp"

#include <algorithm>
#include <cmath>

namespace Geometry::utils
{

	bool almost_equal (double a, double b, double abs_eps, double rel_eps)
	{
		double diff = std::abs (a - b);

		if (diff <= abs_eps)
			return true;

		return diff <= rel_eps * std::max (std::abs (a), std::abs (b));
	}

} // namespace Geometry::utils