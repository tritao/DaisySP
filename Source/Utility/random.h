/*
Copyright (c) 2026 DaisySP contributors
Use of this source code is governed by the MIT license in LICENSE.
*/
#pragma once
#include <cstdlib>

namespace daisysp {
/** Integer randomness compatible with rand(): return values in [0, RAND_MAX].
 * Embedded builds retain libc behavior by default. Host builds can enable
 * DAISYSP_SCOPED_RANDOM to provide a callback for each processing scope.
 */
#ifdef DAISYSP_SCOPED_RANDOM
using RandomCallback = int (*)(void *);
namespace detail {
struct RandomBinding { RandomCallback callback; void *context; };
inline RandomBinding &RandomState() noexcept {
    static thread_local RandomBinding binding{nullptr, nullptr};
    return binding;
}
}
/** A non-owning thread-local binding. Nested scopes restore the previous source.
 * The callback/context must remain alive until the scope ends. A real-time host
 * supplies an allocation-free, nonblocking callback; the library does not lock.
 */
class ScopedRandomSource {
  public:
    ScopedRandomSource(RandomCallback callback, void *context) noexcept
        : previous_(detail::RandomState()) {
        detail::RandomState() = {callback, context};
    }
    ~ScopedRandomSource() { detail::RandomState() = previous_; }
    ScopedRandomSource(const ScopedRandomSource &) = delete;
    ScopedRandomSource &operator=(const ScopedRandomSource &) = delete;
  private:
    detail::RandomBinding previous_;
};
#endif
inline int RandomInt() {
#ifdef DAISYSP_SCOPED_RANDOM
    const auto binding = detail::RandomState();
    if (binding.callback) return binding.callback(binding.context);
#endif
    return std::rand();
}
}
