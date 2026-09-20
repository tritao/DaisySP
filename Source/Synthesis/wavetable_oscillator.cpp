/*
Copyright (c) 2026 NativeKit contributors

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#include "wavetable_oscillator.h"

#include <algorithm>
#include <cmath>

using namespace daisysp;

float WavetableOscillator::Process()
{
    if(!tables_ || table_count_ == 0)
        return 0.0f;

    const auto harmonic_limit = 0.5f * sample_rate_ / std::max(frequency_, 1.0e-6f);
    uint32_t table_index       = 0;
    for(uint32_t index = 1; index < table_count_; ++index)
    {
        if(static_cast<float>(tables_[index].maximum_harmonic) > harmonic_limit)
            break;
        table_index = index;
    }

    const auto& table = tables_[table_index];
    if(!table.samples || table.sample_count == 0)
        return 0.0f;
    const auto wrapped_phase = phase_ - std::floor(phase_);
    const auto position       = wrapped_phase * static_cast<float>(table.sample_count);
    const auto first          = static_cast<uint32_t>(position);
    const auto second         = first + 1 == table.sample_count ? 0 : first + 1;
    const auto fraction       = position - static_cast<float>(first);
    const auto value = table.samples[first]
                       + (table.samples[second] - table.samples[first]) * fraction;

    phase_ += phase_increment_;
    phase_ -= std::floor(phase_);
    return value * amplitude_;
}
