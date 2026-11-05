#pragma once

#include <kit/HalfBand.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

namespace kit
{

// Input gain past 0 dB, on every plug-in in the range: an old, dusty tape machine rather than a harder peak. The signal rounds
// over into a soft ceiling, leaning to one side for some second harmonic, at twice the sample
// rate so it doesn't fold back. The harder it's pushed, the more bass the heads pick up, the
// less top the worn tape keeps, and the more hiss follows the sound. Only ever an effect above
// 0 dB: at or below it the signal passes bit for bit.
//
// Audio thread only, after prepare().
class TapeInput
{
public:
    void prepare (double rate) noexcept
    {
        sampleRate = rate;
        lowCoefficient = 1.0 - std::exp (-2.0 * pi * 90.0 / rate);
        dcCoefficient = std::exp (-2.0 * pi * 5.0 / rate);
        reset();
    }

    void reset() noexcept
    {
        channels = {};
        level = 0.0;
    }

    // In place. The gain has already been applied; these are its dB across the block.
    void process (float* left, float* right, int numSamples, float dbFrom, float dbTo) noexcept
    {
        if (dbFrom <= 0.0f && dbTo <= 0.0f)
            return;
        const double levelCoefficient = 1.0 - std::exp (-1.0 / (0.02 * sampleRate));
        float* signal[2] { left, right };
        for (int i = 0; i < numSamples; ++i)
        {
            const double db = dbFrom + (dbTo - dbFrom) * (double) (i + 1) / numSamples;
            const double engaged = std::clamp (db, 0.0, 1.0);          // fully on tape by +1 dB
            const double worn = std::clamp (db / 24.0, 0.0, 1.0);
            const double lean = 0.15 * worn;
            const double offset = std::tanh (lean);
            const double loss = 1.0 - std::exp (-2.0 * pi * (20000.0 * std::pow (0.25, worn)) / sampleRate);
            const auto saturate = [&] (double x) { return ceiling * (std::tanh (x + lean) - offset); };

            level += levelCoefficient * (std::max (std::abs ((double) left[i]), std::abs ((double) right[i])) - level);
            for (int ch = 0; ch < 2; ++ch)
            {
                auto& c = channels[(size_t) ch];
                const double in = signal[ch][i];
                c.low += lowCoefficient * (in - c.low);
                const double driven = in + 0.3 * worn * c.low;   // the head bump

                const double older = saturate (c.upA.run (driven, HalfBandPath::a));
                const double newer = saturate (c.upB.run (driven, HalfBandPath::b));
                const double out = 0.5 * (c.downA.run (newer, HalfBandPath::a) + c.downB.run (older, HalfBandPath::b));

                c.dcOut = out - c.dcIn + dcCoefficient * c.dcOut;
                c.dcIn = out;
                c.top += loss * (c.dcOut - c.top);
                c.duller += loss * (c.top - c.duller);
                noise = noise * 1664525u + 1013904223u;
                const double hiss = 0.0015 * worn * std::min (1.0, level * 4.0) * ((double) noise / 2147483648.0 - 1.0);

                signal[ch][i] = (float) (in + engaged * (c.duller + hiss - in));
            }
        }
    }

private:
    static constexpr double pi = 3.14159265358979323846, ceiling = 0.92;
    struct Channel
    {
        HalfBandPath upA, upB, downA, downB;
        double low = 0.0, dcIn = 0.0, dcOut = 0.0, top = 0.0, duller = 0.0;
    };
    double sampleRate = 48000.0, lowCoefficient = 0.0, dcCoefficient = 0.999, level = 0.0;
    std::array<Channel, 2> channels {};
    std::uint32_t noise = 0x7a9e;
};

} // namespace kit
