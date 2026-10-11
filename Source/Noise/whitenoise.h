/*
Copyright (c) 2020 Electrosmith, Corp

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/

#pragma once
#ifndef DSY_WHITENOISE_H
#define DSY_WHITENOISE_H
#include <stdint.h>
#ifdef __cplusplus
namespace daisysp
{
/** fast white noise generator

    I think this came from musicdsp.org at some point
*/
class WhiteNoise
{
  public:
    WhiteNoise() {}
    ~WhiteNoise() {}
    /** Initializes the WhiteNoise object
    */
    void Init()
    {
        amp_      = 1.0f;
        randseed_ = 1;
    }

    /** sets the amplitude of the noise output
    */
    inline void SetAmp(float a) { amp_ = a; }
    /** returns a new sample of noise in the range of -amp_ to amp_
    */
    inline float Process()
    {
        randseed_ *= uint32_t{16807};
        const int64_t signed_value = randseed_ < uint32_t{0x80000000}
            ? static_cast<int64_t>(randseed_)
            : static_cast<int64_t>(randseed_) - int64_t{0x100000000};
        return (static_cast<float>(signed_value) * coeff_) * amp_;
    }

    /** sets the seed (and corrects a seed of 0 to 1) */
    inline void SetSeed(int32_t s) { randseed_ = s == 0 ? 1u : static_cast<uint32_t>(s); }

  private:
    static constexpr float coeff_ = 4.6566129e-010f;
    float                  amp_;
    uint32_t               randseed_;
};
} // namespace daisysp
#endif
#endif
