#ifndef GEOMETRY_BOUNDS_BOUNDING_BOX_HPP
#define GEOMETRY_BOUNDS_BOUNDING_BOX_HPP

#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

#include <span>

namespace Geometry
{
	/// An axis-aligned bounding box (AABB) in 3D space defined by a minimum
	/// corner point and a maximum corner point.
	///
	/// The box is the closed rectangular region:
	///   { P : min.x ≤ P.x ≤ max.x,  min.y ≤ P.y ≤ max.y,  min.z ≤ P.z ≤ max.z }
	///
	/// Degenerate boxes (zero thickness in one or more axes) are permitted.
	///
	/// **Coordinate system responsibility (caller's contract)**
	/// All points supplied to this class — whether via the constructor,
	/// `from_points()`, or `expand()` — must be expressed in the **same
	/// coordinate system**.  The class stores raw coordinate values and has no
	/// knowledge of local frames or transforms.  Mixing points from different
	/// coordinate spaces (e.g. one object's local frame vs. world space) will
	/// produce a geometrically meaningless box without any error or warning.
	/// It is the caller's responsibility to transform all input points into a
	/// common frame before constructing or comparing bounding boxes.
	/// Throws std::invalid_argument if any component of max is less than the
	/// corresponding component of min (inverted / empty box).
	class BoundingBox
	{
		public:
			BoundingBox(Point const& min, Point const& max);

			/// Builds the tightest AABB that contains all points in the span.
			/// Throws std::invalid_argument if the span is empty.
			[[nodiscard]] static BoundingBox from_points(std::span<Point const> points);

			[[nodiscard]] Point const& min() const noexcept;
			[[nodiscard]] Point const& max() const noexcept;

			/// Centre point: (min + max) / 2.
			[[nodiscard]] Point center() const noexcept;

			/// Diagonal vector from min to max.
			[[nodiscard]] Vector diagonal() const noexcept;

			/// Volume of the box: width × height × depth.
			[[nodiscard]] double volume() const noexcept;

			/// Total surface area: 2(wx·wy + wy·wz + wz·wx).
			[[nodiscard]] double surface_area() const noexcept;

			/// Returns true if the point lies inside or on the boundary of the box.
			[[nodiscard]] bool contains(Point const& p) const noexcept;

			/// Returns true if the other box is fully enclosed by (or coincides with) this box.
			[[nodiscard]] bool contains(BoundingBox const& other) const noexcept;

			/// Returns true if this box and the other box overlap (touching counts as overlap).
			[[nodiscard]] bool intersects(BoundingBox const& other) const noexcept;

			/// Grows this box in-place to include the point.
			BoundingBox& expand(Point const& p) noexcept;

			/// Grows this box in-place to include the other box.
			BoundingBox& expand(BoundingBox const& other) noexcept;

			[[nodiscard]] bool operator==(BoundingBox const& other) const noexcept;
			[[nodiscard]] bool operator!=(BoundingBox const& other) const noexcept;

		private:
			Point m_min{};
			Point m_max{};
	};

	/// Alias for users familiar with the standard acronym.
	using AABB = BoundingBox;
} // namespace Geometry

#endif // GEOMETRY_BOUNDS_BOUNDING_BOX_HPP
