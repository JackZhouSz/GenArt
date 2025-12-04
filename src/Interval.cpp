#include "Interval.h"

#include "MathHelpers.h"

#include <algorithm>
#include <limits>
#include <sstream>

interval::interval()
{
    // Use the convention that if upper < lower, it's empty.
    lower = infinity();
    upper = -infinity();
}

interval::interval(const float v) : lower(v), upper(v) {}

interval::interval(const float v0, const float v1) { set(v0, v1); }

void interval::set(const float v) { lower = upper = v; }

void interval::set(const float v1, const float v2)
{
    lower = std::min(v1, v2);
    upper = std::max(v1, v2);
}

void interval::extend(const float v)
{
    // These return different results in debug vs. release mode with NaN.
    // v can be Nan if it's the result of 0 * inf.
    lower = std::min(lower, v);
    upper = std::max(upper, v);
}

void interval::extend(const interval ivl)
{
    lower = std::min(lower, ivl.lower);
    upper = std::max(upper, ivl.upper);
}

void interval::set_infinite()
{
    lower = -infinity();
    upper = infinity();
}

bool interval::contains(const float v) const { return (v >= lower || lower == -infinity()) && (v <= upper || upper == infinity()); }

bool interval::contains(const interval ivl) const { return (ivl.lower >= lower || lower == -infinity()) && (ivl.upper <= upper || upper == infinity()); }

bool interval::overlaps(const interval ivl) const { return ivl.lower <= upper && ivl.upper >= lower; }

bool interval::empty() const { return upper < lower; }

bool interval::finite() const { return Finite(lower) && Finite(upper); }

bool interval::isnan() const { return IsNaN(lower) || IsNaN(upper); }

float interval::span() const { return fabsf(upper - lower); }

float interval::lower_finite() const
{
    return Finite(lower) ? lower : ((lower < 0.0f) ? -std::numeric_limits<float>::max() : std::numeric_limits<float>::max());
}

float interval::upper_finite() const
{
    return Finite(upper) ? upper : ((upper < 0.0f) ? -std::numeric_limits<float>::max() : std::numeric_limits<float>::max());
}

float interval::min_float() { return std::numeric_limits<float>::min(); }

float interval::infinity() { return std::numeric_limits<float>::infinity(); }

interval operator-(const interval& ivl)
{
    interval iout(-ivl.upper, -ivl.lower);

    return iout;
}

interval operator+(const interval& ivl, const interval& ivr)
{
    interval iout(ivr.lower + ivl.lower, ivr.upper + ivl.upper);

    return iout;
}

namespace {
float mymul(float a, float b)
{
    if (a == 0.f || b == 0.f) return 0.f;
    return a * b;
}
} // namespace

interval operator*(const interval& ivl, const interval& ivr)
{
    interval iout;
    iout.extend(mymul(ivr.lower, ivl.lower));
    iout.extend(mymul(ivr.lower, ivl.upper));
    iout.extend(mymul(ivr.upper, ivl.lower));
    iout.extend(mymul(ivr.upper, ivl.upper));

    return iout;
}

interval intersect(const interval& ivl, const interval& ivr)
{
    interval iout;
    iout.lower = std::max(ivl.lower, ivr.lower);
    iout.upper = std::min(ivl.upper, ivr.upper);

    return iout;
}

std::string tostring(const interval& ival)
{
    std::ostringstream oss;
    oss << '[' << ival.lower << ',' << ival.upper << ']';

    return oss.str();
}
