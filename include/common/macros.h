/* Copyright 2018 Hobum (Vincent) Kwon. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef VTZ_COMMON_MACROS_H_
#define VTZ_COMMON_MACROS_H_

// Compiler attributes
#if (defined(__GNUC__) || defined(__APPLE__)) && !defined(SWIG)
// Compiler supports GCC-style attributes
#define VTZ_ATTRIBUTE_NORETURN __attribute__((noreturn))
#define VTZ_ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#define VTZ_ATTRIBUTE_NOINLINE __attribute__((noinline))
#define VTZ_ATTRIBUTE_UNUSED __attribute__((unused))
#define VTZ_ATTRIBUTE_COLD __attribute__((cold))
#define VTZ_ATTRIBUTE_WEAK __attribute__((weak))
#define VTZ_PACKED __attribute__((packed))
#define VTZ_MUST_USE_RESULT __attribute__((warn_unused_result))
#define VTZ_PRINTF_ATTRIBUTE(string_index, first_to_check) \
  __attribute__((__format__(__printf__, string_index, first_to_check)))
#define VTZ_SCANF_ATTRIBUTE(string_index, first_to_check) \
  __attribute__((__format__(__scanf__, string_index, first_to_check)))
#elif defined(_MSC_VER)
// Non-GCC equivalents
#define VTZ_ATTRIBUTE_NORETURN __declspec(noreturn)
#define VTZ_ATTRIBUTE_ALWAYS_INLINE __forceinline
#define VTZ_ATTRIBUTE_NOINLINE
#define VTZ_ATTRIBUTE_UNUSED
#define VTZ_ATTRIBUTE_COLD
#define VTZ_ATTRIBUTE_WEAK
#define VTZ_MUST_USE_RESULT
#define VTZ_PACKED
#define VTZ_PRINTF_ATTRIBUTE(string_index, first_to_check)
#define VTZ_SCANF_ATTRIBUTE(string_index, first_to_check)
#else
// Non-GCC equivalents
#define VTZ_ATTRIBUTE_NORETURN
#define VTZ_ATTRIBUTE_ALWAYS_INLINE
#define VTZ_ATTRIBUTE_NOINLINE
#define VTZ_ATTRIBUTE_UNUSED
#define VTZ_ATTRIBUTE_COLD
#define VTZ_ATTRIBUTE_WEAK
#define VTZ_MUST_USE_RESULT
#define VTZ_PACKED
#define VTZ_PRINTF_ATTRIBUTE(string_index, first_to_check)
#define VTZ_SCANF_ATTRIBUTE(string_index, first_to_check)
#endif


// Control visiblity outside .so
#if defined(_WIN32)
#ifdef VTZ_COMPILE_LIBRARY
#define VTZ_EXPORT __declspec(dllexport)
#else
#define VTZ_EXPORT __declspec(dllimport)
#endif  // VTZ_COMPILE_LIBRARY
#else
#define VTZ_EXPORT __attribute__((visibility("default")))
#endif  // _WIN32

#ifdef __has_builtin
#define VTZ_HAS_BUILTIN(x) __has_builtin(x)
#else
#define VTZ_HAS_BUILTIN(x) 0
#endif

// Compilers can be told that a certain branch is not likely to be taken
// (for instance, a CHECK failure), and use that information in static
// analysis. Giving it this information can help it optimize for the
// common case in the absence of better information (ie.
// -fprofile-arcs).
//
// We need to disable this for GPU builds, though, since nvcc8 and older
// don't recognize `__builtin_expect` as a builtin, and fail compilation.
#if (!defined(__NVCC__)) && \
    (VTZ_HAS_BUILTIN(__builtin_expect) || (defined(__GNUC__) && __GNUC__ >= 3))
#define VTZ_PREDICT_FALSE(x) (__builtin_expect(x, 0))
#define VTZ_PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#else
#define VTZ_PREDICT_FALSE(x) (x)
#define VTZ_PREDICT_TRUE(x) (x)
#endif

// A macro to disallow the copy constructor and operator= functions
// This is usually placed in the private: declarations for a class.
#define VTZ_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;         \
  void operator=(const TypeName&) = delete

// The VTZ_ARRAYSIZE(arr) macro returns the # of elements in an array arr.
//
// The expression VTZ_ARRAYSIZE(a) is a compile-time constant of type
// size_t.
#define VTZ_ARRAYSIZE(a)         \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#endif  // VTZ_COMMON_MACROS_H_

