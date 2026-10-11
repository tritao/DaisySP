cmake_minimum_required(VERSION 3.14)

project(DAISYSP VERSION 0.0.1)

add_library(DaisySP STATIC
Source/Control/adenv.cpp
Source/Control/adsr.cpp
Source/Control/phasor.cpp
Source/Drums/analogbassdrum.cpp
Source/Drums/analogsnaredrum.cpp
Source/Drums/hihat.cpp
Source/Drums/synthbassdrum.cpp
Source/Drums/synthsnaredrum.cpp
Source/Dynamics/crossfade.cpp
Source/Dynamics/limiter.cpp
Source/Effects/autowah.cpp
Source/Effects/chorus.cpp
Source/Effects/decimator.cpp
Source/Effects/flanger.cpp
Source/Effects/overdrive.cpp
Source/Effects/phaser.cpp
Source/Effects/sampleratereducer.cpp
Source/Effects/tremolo.cpp
Source/Effects/wavefolder.cpp
Source/Filters/ladder.cpp
Source/Filters/svf.cpp
Source/Filters/soap.cpp
Source/Noise/clockednoise.cpp
Source/Noise/grainlet.cpp
Source/Noise/particle.cpp
Source/PhysicalModeling/drip.cpp
Source/PhysicalModeling/modalvoice.cpp
Source/PhysicalModeling/resonator.cpp
Source/PhysicalModeling/KarplusString.cpp
Source/PhysicalModeling/stringvoice.cpp
Source/Sampling/granularplayer.cpp
Source/Synthesis/fm2.cpp
Source/Synthesis/formantosc.cpp
Source/Synthesis/oscillator.cpp
Source/Synthesis/wavetable_oscillator.cpp
Source/Synthesis/oscillatorbank.cpp
Source/Synthesis/variablesawosc.cpp
Source/Synthesis/variableshapeosc.cpp
Source/Synthesis/vosim.cpp
Source/Synthesis/zoscillator.cpp
Source/Utility/dcblock.cpp
Source/Utility/metro.cpp
)


set_target_properties(DaisySP PROPERTIES PUBLIC
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED
  )


target_include_directories(DaisySP PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Source
  PRIVATE
  "Source"
  "Source/Control"
  "Source/Drums"
  "Source/Dynamics"
  "Source/Effects"
  "Source/Filters"
  "Source/Noise"
  "Source/PhysicalModeling"
  "Source/Synthesis"
  "Source/Utility"
  )

# Scoped host randomness is opt-in; embedded/libc users retain their behavior.
option(DAISYSP_SCOPED_RANDOM "Enable scoped thread-local random callbacks" OFF)
if(DAISYSP_SCOPED_RANDOM)
  target_compile_definitions(DaisySP PUBLIC DAISYSP_SCOPED_RANDOM)
endif()
option(DAISYSP_BUILD_TESTS "Build standalone DSP regression tests" OFF)
if(DAISYSP_BUILD_TESTS)
  enable_testing()
  add_executable(daisysp_initialization_tests tests/initialization.cpp)
  target_link_libraries(daisysp_initialization_tests PRIVATE DaisySP)
  if(NOT DAISYSP_SCOPED_RANDOM)
    message(FATAL_ERROR "DSP regression tests require DAISYSP_SCOPED_RANDOM=ON")
  endif()
  add_test(NAME daisysp_initialization COMMAND daisysp_initialization_tests)
  find_package(Threads REQUIRED)
  add_executable(daisysp_random_tests tests/random.cpp)
  target_link_libraries(daisysp_random_tests PRIVATE DaisySP Threads::Threads)
  add_test(NAME daisysp_random COMMAND daisysp_random_tests)
endif()
