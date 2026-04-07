#include "Geometry/Shapes/segment.hpp"

#include <algorithm>
#include <stdexcept>

#include "utils/assert_utils.hpp"
#include "utils/numerical_utils.hpp"

namespace Geometry
{
	Segment::Segment(Point const& start, Point const& end)
	: m_start(start),
		m_end(end)
	{
		bool const valid = (m_end - m_start).isValidDirection();
		GEOM_ASSERT(valid);
		if (!valid)
			throw std::invalid_argument("Segment endpoints must be distinct");
	}

	Point const& Segment::start() const noexcept
	{
		return m_start;
	}

	Point const& Segment::end() const noexcept
	{
		return m_end;
	}

	double Segment::length() const noexcept
	{
		return m_start.distance(m_end);
	}

	double Segment::length_squared() const noexcept
	{
		return m_start.distance_squared(m_end);
	}

	Point Segment::midpoint() const noexcept
	{
		return m_start + 0.5 * (m_end - m_start);
	}

	Vector Segment::direction() const
	{
		return (m_end - m_start).normalized();
	}

	// ── at(t) ─────────────────────────────────────────────────────────────────
	//
	// Returns start + clamp(t, 0, 1) * (end − start).
	// Clamping keeps the result on the segment for any t value.
	//
	Point Segment::at(double t) const noexcept
	{
		double const tc = std::clamp(t, 0.0, 1.0);
		return m_start + tc * (m_end - m_start);
	}

	// ── contains(Point) ───────────────────────────────────────────────────────
	//
	// A point P lies on the segment if and only if:
	//   dist(start, P) + dist(P, end) ≈ dist(start, end)
	//
	// This avoids cross-product or parameterisation logic and handles
	// degenerate near-zero-length segments via the constructor invariant.
	//
	bool Segment::contains(Point const& p, double abs_eps) const noexcept
	{
		double const len = length();
		double const sum = m_start.distance(p) + p.distance(m_end);
		return (sum - len) <= abs_eps;
	}

	bool Segment::operator==(Segment const& other) const noexcept
	{
		return m_start == other.m_start && m_end == other.m_end;
	}

	bool Segment::operator!=(Segment const& other) const noexcept
	{
		return !(*this == other);
	}

} // namespace Geometry
