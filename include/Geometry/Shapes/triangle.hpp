#ifndef GEOMETRY_TRIANGLE_HPP
#define GEOMETRY_TRIANGLE_HPP

#include "Geometry/Primitives/plane.hpp"
#include "Geometry/Primitives/point.hpp"
#include "Geometry/Primitives/vector.hpp"

namespace Geometry
{
	/// A finite triangle defined by three non-collinear vertices (p0, p1, p2).
	/// The winding order p0 → p1 → p2 determines the outward normal via
	/// the right-hand rule:  n = (p1 − p0) × (p2 − p0), normalised.
	/// Throws std::invalid_argument if the three points are collinear.
	class Triangle
	{
		public:
			Triangle(Point const& p0, Point const& p1, Point const& p2);

			[[nodiscard]] Point const& p0() const noexcept;
			[[nodiscard]] Point const& p1() const noexcept;
			[[nodiscard]] Point const& p2() const noexcept;

			/// Unit normal consistent with the p0→p1→p2 winding order.
			[[nodiscard]] Vector const& normal() const noexcept;

			/// Area of the triangle:  ½ |( p1−p0) × (p2−p0)|
			[[nodiscard]] double area() const noexcept;

			/// Centroid (arithmetic mean of the three vertices).
			[[nodiscard]] Point centroid() const noexcept;

			/// Sum of edge lengths.
			[[nodiscard]] double perimeter() const noexcept;

			/// The infinite plane that contains the triangle (same normal and winding).
			[[nodiscard]] Plane plane() const;

			/// Returns true if the point lies inside or on the boundary of the triangle.
			/// Uses a barycentric coordinate test; the point is first assumed to be
			/// coplanar — results are undefined if it lies significantly off the plane.
			[[nodiscard]] bool contains(Point const& p) const noexcept;

			[[nodiscard]] bool operator==(Triangle const& other) const noexcept;
			[[nodiscard]] bool operator!=(Triangle const& other) const noexcept;

		private:
			Point m_p0{};
			Point m_p1{};
			Point m_p2{};

			// m_normal and m_area are derived from the three vertices but are cached
			// for two concrete reasons:
			//
			// 1. noexcept guarantees — computing the normal on demand requires
			//    normalized(), which can throw on a zero-length vector.  With the
			//    cached value, normal() and area() are unconditionally noexcept.
			//
			// 2. contains() reuse — the cross-product sign test in contains() depends
			//    on m_normal.  In mesh or BVH workloads a single triangle may be
			//    tested against thousands of query points; recomputing the normal
			//    each time would add a cross product + sqrt + division per call.
			//
			// Both values are computed once during construction (cost paid upfront)
			// and are read-only thereafter.  The added storage cost is 32 bytes per
			// Triangle instance (one Vector + one double).
			Vector m_normal{};
			double m_area{};
	};
} // namespace Geometry

#endif // GEOMETRY_TRIANGLE_HPP
