#pragma once

#include <array>

namespace kit
{

// One path of a half-band filter: all-pass sections in series. Two of them, A and B, go to
// twice the sample rate and back: flat to 0.42 of the rate, 69 dB down from 0.6.
struct HalfBandPath
{
    static constexpr std::array<double, 2> a { 0.07986642623635751, 0.5453536510711322 };
    static constexpr std::array<double, 2> b { 0.28382934487410993, 0.8344118914807379 };

    std::array<double, 2> x {}, y {};
    double run (double in, const std::array<double, 2>& coefficients) noexcept
    {
        for (size_t n = 0; n < coefficients.size(); ++n)
        {
            const double out = coefficients[n] * (in - y[n]) + x[n];
            x[n] = in;
            y[n] = out;
            in = out;
        }
        return in;
    }
};

} // namespace kit
