#include "Utility/random.h"
#include "Noise/dust.h"
#include "Noise/clockednoise.h"
#include "Noise/whitenoise.h"
#include <cassert>
#include <cstdint>
#include <thread>
#include <array>
struct State { uint32_t value; unsigned calls = 0; };
static int next(void *context) {
    auto &s = *static_cast<State *>(context);
    ++s.calls;
    s.value ^= s.value << 13; s.value ^= s.value >> 17; s.value ^= s.value << 5;
    return static_cast<int>(s.value & static_cast<uint32_t>(RAND_MAX));
}
static std::array<int, 1024> sequence(uint32_t seed) {
    State s{seed}; daisysp::ScopedRandomSource scope(next, &s);
    std::array<int, 1024> result;
    for(auto &v : result) v = daisysp::RandomInt();
    return result;
}
int main() {
    std::srand(42); int expected = std::rand(); std::srand(42);
    assert(daisysp::RandomInt() == expected);
    State outer{123}, reference{123}, inner{321};
    {
        daisysp::ScopedRandomSource scope(next, &outer);
        assert(daisysp::RandomInt() == next(&reference));
        { daisysp::ScopedRandomSource nested(next, &inner); daisysp::RandomInt(); }
        assert(daisysp::RandomInt() == next(&reference));
        unsigned calls = outer.calls;
        daisysp::Dust dust; dust.Init(); dust.Process();
        assert(outer.calls > calls);
        calls = outer.calls;
        daisysp::ClockedNoise noise; noise.Init(48000); noise.SetFreq(24000);
        for(int i=0;i<8;++i) noise.Process();
        assert(outer.calls > calls); // Compiled source uses the same header binding.
    }
    std::array<int,1024> a,b;
    std::thread ta([&]{ a=sequence(123); }); std::thread tb([&]{ b=sequence(321); });
    ta.join(); tb.join(); assert(a==sequence(123)); assert(b==sequence(321));
    for(int32_t seed : {0,1,-1,INT32_MIN,INT32_MAX}) {
        daisysp::WhiteNoise noise; noise.Init(); noise.SetSeed(seed); noise.SetAmp(.75f);
        uint32_t state=seed ? static_cast<uint32_t>(seed) : 1u;
        for(int i=0;i<10000;++i) {
            state *= uint32_t{16807};
            int64_t signed_value=state<0x80000000u ? int64_t(state) : int64_t(state)-int64_t{0x100000000};
            float value=float(signed_value)*4.6566129e-010f*.75f;
            assert(noise.Process()==value);
        }
    }
}
