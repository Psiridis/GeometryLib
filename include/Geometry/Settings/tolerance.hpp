#ifndef GEOMETRY_SETTINGS_TOLERANCE_HPP
#define GEOMETRY_SETTINGS_TOLERANCE_HPP

namespace Geometry
{
	/// Floating-point tolerance constants used throughout the library.
	///
	/// Exposed in the public API so consumers can write tolerance-consistent
	/// comparisons without reimplementing or hard-coding the same values.
	namespace tolerance
	{
		/// Default absolute tolerance.
		/// Used where the threshold does not need to scale with input magnitude
		/// (e.g. cross products of unit vectors, angle tests on unit normals).
		inline constexpr double k_abs_eps = 1e-12;

		/// Default relative tolerance.
		/// Used for scale-independent comparisons where vector magnitudes are
		/// factored out of the threshold (e.g. sine/cosine parallelism tests,
		/// on directions that are not required to be unit length).
		inline constexpr double k_rel_eps = 1e-9;
	} // namespace tolerance
} // namespace Geometry

#endif // GEOMETRY_SETTINGS_TOLERANCE_HPP