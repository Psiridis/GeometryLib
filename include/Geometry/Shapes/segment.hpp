#ifndef GEOMETRY_SEGMENT_HPP
#define GEOMETRY_SEGMENT_HPP

#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"
#include "Geometry/Settings/tolerance.hpp"

namespace Geometry
{
	/// A finite line segment defined by two distinct endpoints.
	/// The segment spans the set of points p(t) = start + t*(end−start) for t ∈
	/// [0, 1]. Throws std::invalid_argument if the two endpoints are coincident.
	class Segment
	{
		public:
			Segment(Point const& start, Point const& end);

			[[nodiscard]] Point const& start() const noexcept;
			[[nodiscard]] Point const& end() const noexcept;

			/// Length of the segment.
			[[nodiscard]] double length() const noexcept;

			/// Squared length — cheaper than length() when only comparison is needed.
			[[nodiscard]] double length_squared() const noexcept;

			/// Midpoint of the segment.
			[[nodiscard]] Point midpoint() const noexcept;

			/// Unit vector from start to end.
			[[nodiscard]] Vector direction() const;

			/// Parametric point on the segment.  t is clamped to [0, 1].
			[[nodiscard]] Point at(double t) const noexcept;

			/// Returns true if the point lies on the segment within tolerance.
			/// Tests that distance(P, segment) ≈ 0 by checking that the sum of
			/// distances from P to each endpoint equals the segment length.
			[[nodiscard]] bool contains(Point const& p, double tol = tolerance::k_rel_eps) const noexcept;

			[[nodiscard]] bool operator==(Segment const& other) const noexcept;
			[[nodiscard]] bool operator!=(Segment const& other) const noexcept;

		private:
			Point m_start{};
			Point m_end{};
	};
} // namespace Geometry

#endif // GEOMETRY_SEGMENT_HPP
