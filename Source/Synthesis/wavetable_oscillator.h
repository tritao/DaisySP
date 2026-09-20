/*
Copyright (c) 2026 NativeKit contributors

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_WAVETABLE_OSCILLATOR_H
#define DSY_WAVETABLE_OSCILLATOR_H

#include <stdint.h>

#ifdef __cplusplus

namespace daisysp
{
/** Plays an immutable bank of periodic, harmonic-limited wavetable bands.

    The caller owns the table descriptors and sample storage. They must remain
    valid until the oscillator is pointed at another bank or destroyed. Bands
    are ordered from lowest to highest maximum harmonic.
*/
class WavetableOscillator
{
  public:
    struct Table
    {
        const float* samples;
        uint32_t     sample_count;
        uint32_t     maximum_harmonic;
    };

    WavetableOscillator() {}
    ~WavetableOscillator() {}

    /** Initializes the oscillator for one Process call per sample period. */
    void Init(float sample_rate)
    {
        sample_rate_      = sample_rate;
        sample_rate_recip_ = 1.0f / sample_rate;
        frequency_        = 100.0f;
        amplitude_        = 0.5f;
        phase_            = 0.0f;
        phase_increment_  = frequency_ * sample_rate_recip_;
        tables_            = nullptr;
        table_count_      = 0;
    }

    /** Points the oscillator at an immutable table bank. */
    void SetTables(const Table* tables, uint32_t table_count)
    {
        tables_       = tables;
        table_count_  = table_count;
    }

    /** Changes the oscillator frequency in Hz. */
    void SetFreq(float frequency)
    {
        frequency_       = frequency;
        phase_increment_ = frequency * sample_rate_recip_;
    }

    /** Sets the linear output amplitude. */
    void SetAmp(float amplitude) { amplitude_ = amplitude; }

    /** Resets the normalized phase. */
    void Reset(float phase = 0.0f) { phase_ = phase; }

    /** Processes one sample with linear interpolation and band selection. */
    float Process();

  private:
    float              sample_rate_;
    float              sample_rate_recip_;
    float              frequency_;
    float              amplitude_;
    float              phase_;
    float              phase_increment_;
    const Table*       tables_;
    uint32_t           table_count_;
};
} // namespace daisysp

#endif
#endif
