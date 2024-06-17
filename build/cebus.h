/* MIT License

Copyright (c) 2024 Loris Kriyonas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/* DOCUMENTATION
In your C source file, include the library header and define the implementation
as follows:
```c
#define CEBUS_IMPLEMENTATION
#include "cebus.h"
```
*/

/* DOCUMENTATION
## Features

- **Platform Detection**: Identifies the operating system, such as Linux or
Windows.
- **Architecture Detection**: Determines the CPU architecture, such as x86_64 or
ARM.
- **Compiler Detection**: Identifies the compiler used, such as GCC, Clang, or
MSVC.
- **CPU Bitness**: Distinguishes between 32-bit and 64-bit environments.
- **Byte Order**: Defines the system's byte order (endianness).
*/

#ifndef __CEBUS_PLATFORM_H__
#define __CEBUS_PLATFORM_H__

////////////////////////////////////////////////////////////////////////////
/* System */
#if defined(__linux__)
#define LINUX
#define CEBUS_SYSTEM "Linux"
#elif defined(_WIN32) || defined(_WIN64)
#define WINDOWS
#define CEBUS_SYSTEM "Windows"
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#else
#error "Platform not supported!"
#endif
/* !System */

////////////////////////////////////////////////////////////////////////////
/* Architecture */
#if defined(__x86_64__) || defined(_M_X64)
#define x86_64
#define CEBUS_ARCHITECTURE "x86_64"
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define x86_32
#define CEBUS_ARCHITECTURE "x86_32"
#elif defined(__ARM_ARCH_2__)
#define ARM2
#define CEBUS_ARCHITECTURE "ARM2"
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define ARM3
#define CEBUS_ARCHITECTURE "ARM3"
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define ARM4T
#define CEBUS_ARCHITECTURE "ARM4T"
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
#define ARM5
#define CEBUS_ARCHITECTURE "ARM5"
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define ARM6T2
#define CEBUS_ARCHITECTURE "ARM6T2"
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) ||                   \
    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) ||                    \
    defined(__ARM_ARCH_6ZK__)
#define ARM6
#define CEBUS_ARCHITECTURE "ARM6"
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) ||                   \
    defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||                    \
    defined(__ARM_ARCH_7S__)
#define ARM7
#define CEBUS_ARCHITECTURE "ARM7"
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||                  \
    defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARM7A
#define CEBUS_ARCHITECTURE "ARM7A"
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) ||                  \
    defined(__ARM_ARCH_7S__)
#define ARM7R
#define CEBUS_ARCHITECTURE "ARM7R"
#elif defined(__ARM_ARCH_7M__)
#define ARM7M
#define CEBUS_ARCHITECTURE "ARM7M"
#elif defined(__ARM_ARCH_7S__)
#define ARM7S
#define CEBUS_ARCHITECTURE "ARM7S"
#elif defined(__aarch64__) || defined(_M_ARM64)
#define ARM64
#define CEBUS_ARCHITECTURE "ARM64"
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define MIPS
#define CEBUS_ARCHITECTURE "MIPS"
#elif defined(__sh__)
#define SUPERH
#define CEBUS_ARCHITECTURE "SUPERH"
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) ||  \
    defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) ||            \
    defined(_ARCH_PPC)
#define POWERPC
#define CEBUS_ARCHITECTURE "POWERPC"
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define POWERPC64
#define CEBUS_ARCHITECTURE "POWERPC64"
#elif defined(__sparc__) || defined(__sparc)
#define SPARC
#define CEBUS_ARCHITECTURE "SPARC"
#elif defined(__m68k__)
#define M68K
#define CEBUS_ARCHITECTURE "M68K"
#else
#define ARCHITECTURE_UNKNOWN 0
#define CEBUS_ARCHITECTURE "ARCHITECTURE UNKNOWN"
#endif
/* !Architecture */

////////////////////////////////////////////////////////////////////////////
/* Compiler */
#if defined(__GNUC__) && !defined(__clang__)
#define GCC
#define CEBUS_COMPILER "GCC"
#elif defined(__clang__)
#define CLANG
#define CEBUS_COMPILER "Clang"
#elif defined(__TINYC__)
#define TINYC
#define CEBUS_COMPILER "TinyC"
#elif defined(_MSC_VER)
#define MSVC
#define CEBUS_COMPILER "MSVC"
#elif defined(__MINGW32__)
#define MINGW32
#define CEBUS_COMPILER "MinGW32"
#elif defined(__MINGW64__)
#define MINGW64
#define CEBUS_COMPILER "MinGW64"
#elif defined(__INTEL_COMPILER)
#define INTEL_COMPILER
#define CEBUS_COMPILER "Intel Compiler"
#else
#define COMPILER_UNKOWN
#define CEBUS_COMPILER "COMPILER UNKOWN"
#endif
/* !Compiler */

////////////////////////////////////////////////////////////////////////////
/* CPU */
#if defined(__LP64__) || defined(_LP64)
#define CEBUS_64BIT
#else
#define CEBUS_32BIT
#endif
/* !CPU */

////////////////////////////////////////////////////////////////////////////
/* Byte-Order */
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) &&                \
    defined(__ORDER_LITTLE_ENDIAN__)

#define ENDIAN_LITTLE __ORDER_LITTLE_ENDIAN__
#define ENDIAN_BIG __ORDER_BIG_ENDIAN__
#define CEBUS_BYTE_ORDER __BYTE_ORDER__

#elif defined(_MSC_VER)
#define ENDIAN_LITTLE 1234
#define ENDIAN_BIG 4321
#define CEBUS_BYTE_ORDER ENDIAN_LITTLE
#else
#error "No Byte Order detected!"
#endif
/* !Byte-Order */

////////////////////////////////////////////////////////////////////////////

#ifdef __cross__
#define CEBUS_CROSS_COMPILATION
#endif

////////////////////////////////////////////////////////////////////////////

#endif /*  !__CEBUS_PLATFORM_H__ */

/* DOCUMENTATION
## Key Definitions and Macros

- **Data Types**: Defines types such as `u8`, `i8`, `u32`, `i32`,
`usize`, `f32`, `f64`, `Bytes`, `Str`, and `Utf8`.
- **Mathematical Constants**: Defines `F64_PI` and `F64_E` for mathematical
operations.
- **Memory Units**: Macros for `KILOBYTES`, `MEGABYTES`, and `GIGABYTES` to
easily specify memory sizes.
- **Array Length**: `ARRAY_LEN(A)` calculates the number of elements in an
array.
- **Comparison Ordering**: Enum `CmpOrdering` for less than, equal, and greater
than comparisons.
- **Compiler Attributes**: Macros such as `EXPORT`, `NORETURN`, `UNUSED`,
`PURE_FN`, `CONST_FN` for compiler-specific attributes.
- **Likely and Unlikely**: `LIKELY` and `UNLIKELY` macros to hint the compiler
about branch prediction.
- **Format Attribute**: `FMT` macro to specify format strings for functions,
enhancing type safety with `printf`-like functions.

*/

#ifndef __CEBUS_DEFINES_H__
#define __CEBUS_DEFINES_H__

// #include "platform.h"

#include <float.h>
#include <stdbool.h>
#include <stddef.h> // IWYU pragma: export
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////

#define KILOBYTES(s) ((usize)(s) * (usize)1e+3)
#define MEGABYTES(s) ((usize)(s) * (usize)1e+6)
#define GIGABYTES(s) ((usize)(s) * (usize)1e+9)

#define ARRAY_LEN(...) (sizeof((__VA_ARGS__)) / sizeof((__VA_ARGS__)[0]))

////////////////////////////////////////////////////////////////////////////

typedef struct {
  const char *file;
  int line;
  const char *function;
} FileLocation;

#define FILE_LOCATION_FMT "%s:%d: %s()"
#define FILE_LOCATION_ARG(fl) (fl).file, (fl).line, (fl).function
#define FILE_LOCATION_ARG_CURRENT __FILE__, __LINE__, __func__
#define FILE_LOCATION_CURRENT                                                  \
  (FileLocation) { __FILE__, __LINE__, __func__ }

////////////////////////////////////////////////////////////////////////////

typedef enum {
  CMP_LESS = -1,
  CMP_EQUAL = 0,
  CMP_GREATER = 1,
} CmpOrdering;

typedef CmpOrdering (*CompareFn)(const void *, const void *);
typedef CmpOrdering (*CompareCtxFn)(const void *, const void *, const void *);

////////////////////////////////////////////////////////////////////////////

typedef uint8_t u8;
#define U8_MAX UINT8_MAX
#define U8_MIN 0
#define U8_BITS 8
#define U8_FMT "hhu"
#define U8_HEX "hhx"

typedef int8_t i8;
#define I8_MAX INT8_MAX
#define I8_MIN INT8_MIN
#define I8_BITS 8
#define I8_FMT "hhd"
#define I8_HEX "hhx"

typedef uint16_t u16;
#define U16_MAX UINT16_MAX
#define U16_MIN 0
#define U16_BITS 16
#define U16_FMT "hd"
#define U16_HEX "hx"

typedef int16_t i16;
#define I16_MAX INT16_MAX
#define I16_MIN INT16_MIN
#define I16_BITS 16

typedef uint32_t u32;
#define U32_MAX UINT32_MAX
#define U32_MIN 0
#define U32_BITS 32

typedef int32_t i32;
#define I32_MAX INT32_MAX
#define I32_MIN INT32_MIN
#define I32_BITS 32

typedef uint64_t u64;
#define U64_MAX UINT64_MAX
#define U64_MIN 0
#define U64_BITS 64
#if defined(LINUX)
#define U64_FMT "lu"
#define U64_HEX "lx"
#elif defined(WINDOWS)
#define U64_FMT "llu"
#define U64_HEX "llx"
#else
#define U64_FMT "lu"
#define U64_HEX "lx"
#endif

typedef int64_t i64;
#define I64_MAX INT64_MAX
#define I64_MIN INT64_MIN
#define I64_BITS 64
#if defined(LINUX)
#define I64_FMT "lu"
#define I64_HEX "lx"
#elif defined(WINDOWS)
#define I64_FMT "llu"
#define I64_HEX "llx"
#else
#define I64_FMT "lu"
#define I32_FMT "lx"
#endif

typedef size_t usize;
#define USIZE_MAX SIZE_MAX
#define USIZE_MIN 0
#define USIZE_BITS (sizeof(usize) * 8)
#if defined(WINDOWS) && defined(GCC)
#define USIZE_FMT "llu"
#else
#define USIZE_FMT "zu"
#endif

typedef float f32;
#define F32_MAX FLT_MAX
#define F32_MIN FLT_MIN
#define F32_EPSILON FLT_EPSILON

typedef double f64;
#define F64_MAX DBL_MAX
#define F64_MIN DBL_MIN
#define F64_EPSILON DBL_EPSILON

#define F32_INF (*(const f32 *)(const u32[]){0x7F800000})
#define F64_INF (*(const f64 *)(const u64[]){0x7FF0000000000000})
#define F32_NAN (0.0f / 0.0f)
#define F64_NAN (0.0 / 0.0)
#define F64_PI 3.14159265358979323846
#define F32_PI 3.1415926535f
#define F64_E 2.71828182845904523536
#define F32_E 2.7182818284f

#define BOOL_FMT "%s"
#define BOOL_ARG(b) (b ? "true" : "false")

////////////////////////////////////////////////////////////////////////////

#define BYTES(...)                                                             \
  (Bytes) {                                                                    \
    sizeof((const u8[]){__VA_ARGS__}), (const u8[]) { __VA_ARGS__ }            \
  }

#define BYTES_STR(s)                                                           \
  (Bytes) { sizeof(s) - 1, (const u8 *)(s) }

typedef struct {
  usize size;
  const u8 *data;
} Bytes;

////////////////////////////////////////////////////////////////////////////

#define STR(str) ((Str){.len = sizeof(str) - 1, .data = (str)})
#define STR_STATIC(str)                                                        \
  { .len = sizeof(str) - 1, .data = (str) }
#define STR_FMT "%.*s"
#define STR_ARG(str) (i32)(str).len, (str).data

typedef struct {
  usize len;
  const char *data;
} Str;

////////////////////////////////////////////////////////////////////////////

#define UTF8(s) utf8_decode(BYTES_STR(s), ErrDefault)
#define UTF8_FMT "%.*s"
#define UTF8_ARG(s) (i32)(s).size, (s).data

typedef struct {
  usize size;
  const char *data;
  usize len;
} Utf8;

////////////////////////////////////////////////////////////////////////////

#if defined(LINUX)
#include <bits/types/FILE.h>
#elif defined(WINDOWS)
#define _FILE_DEFINED
typedef struct _iobuf FILE;
#endif

////////////////////////////////////////////////////////////////////////////

#if defined(GCC) || defined(CLANG) || defined(MINGW32) || defined(MINGW64)

#define EXPORT __attribute__((used))
#define NORETURN __attribute__((noreturn))
#define UNUSED __attribute__((unused))
#define PURE_FN __attribute__((pure)) __attribute__((warn_unused_result))
#define CONST_FN __attribute__((const)) __attribute__((warn_unused_result))
#define LIKELY(exp) __builtin_expect(((exp) != 0), 1)
#define UNLIKELY(exp) __builtin_expect(((exp) != 0), 0)
#define FMT(__fmt_arg) __attribute__((format(printf, __fmt_arg, __fmt_arg + 1)))

#elif defined(MSVC)

#include <sal.h>
#define EXPORT __declspec(dllexport)
#define NORETURN __declspec(noreturn)
#define UNUSED __pragma(warning(suppress : 4100))
#define PURE_FN _Check_return_
#define CONST_FN _Check_return_

#endif

#ifndef EXPORT
#define EXPORT
#endif

#ifndef NORETURN
#define NORETURN
#endif

#ifndef UNUSED
#define UNUSED
#endif

#ifndef PURE_FN
#define PURE_FN
#endif

#ifndef CONST_FN
#define CONST_FN
#endif

#ifndef LIKELY
#define LIKELY(...) (__VA_ARGS__)
#endif

#ifndef UNLIKELY
#define UNLIKELY(...) (__VA_ARGS__)
#endif

#ifndef FMT
#define FMT(...)
#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_DEFINES_H__ */

/* DOCUMENTATION
## Initialization

To start using the library, initialize an `Arena` struct:

```c
Arena arena = {0};
```

## Memory Allocation

Allocate memory from the arena using `arena_alloc` or `arena_calloc` for
uninitialized or zero-initialized memory, respectively:

```c
int* i1 = arena_alloc(&arena, sizeof(int));
int* i2 = arena_alloc(&arena, sizeof(int));
int* i3 = arena_alloc(&arena, sizeof(int));
```

## Memory Deallocation

Deallocate all memory associated with an arena at once using `arena_free`. This
operation frees all memory chunks allocated from the arena, providing a
significant performance advantage over individual deallocation:

```c
arena_free(&arena);
```

## Chunk

The library also provides functions for more granular control over memory chunks
within an arena:

- `arena_alloc_chunk`: Allocate a new chunk of memory.
- `arena_calloc_chunk`: Allocates a new, zero initialized, chunk of memory.
- `arena_realloc_chunk`: Reallocate a previously allocated chunk to a new size.
- `arena_free_chunk`: Free a specific chunk of memory (advanced use cases).
*/

#ifndef __CEBUS_ARENA_H__
#define __CEBUS_ARENA_H__

// #include "cebus/core/defines.h"

typedef struct Chunk Chunk;

typedef struct {
  Chunk *begin;
} Arena;

////////////////////////////////////////////////////////////////////////////

void arena_free(Arena *arena);

void *arena_alloc(Arena *arena, usize size);
void *arena_calloc(Arena *arena, usize size);
void arena_reset(Arena *arena);

////////////////////////////////////////////////////////////////////////////

void *arena_alloc_chunk(Arena *arena, usize size);
void *arena_calloc_chunk(Arena *arena, usize size);
void *arena_realloc_chunk(Arena *arena, void *ptr, usize size);
void arena_free_chunk(Arena *arena, void *ptr);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_ARENA_H__ */

/* DOCUMENTATION
## Initialization

To start using the dynamic array, you first need to create an instance of
`Arena` for memory management and then initialize the dynamic array with the
specific type.

```c
Arena arena = {0};
DA(int) vec = da_new(&arena);
```

## Adding Elements

Elements can be added to the dynamic array using `da_push`, which automatically
resizes the array if additional space is required.

```c
da_push(&vec, 69);
da_push(&vec, 420);
```

## Accessing Elements

Access the first and last elements with `da_first` and `da_last`. Remove the
last element with `da_pop`. Get any element with `da_get`.

> :warning: These operations do not perform any bounds checks. So make sure your
dynamic array has at least one element in it.

```c
int first = da_first(&vec);
int last = da_last(&vec);
int nth = da_get(&vec, 3);
int popped = da_pop(&vec);
```

## Array Utils

- `da_empty`: Use to check if the array has no elements.
- `da_len`: Get the length of the dynamic array.
- `da_clear`: Reset the length of the array to zero.
- `da_init`:  :warning: depricated :warning: Initialize dynamic array.
- `da_init_list`: Initialize dynamic array from a array.
- `da_init_static`: Initialize dynamic array from a static array.
- `da_copy`: Duplicate the contents of one dynamic array into another.

## Removing and inserting
> :warning: These operations do not perform any bound checks.

- `da_insert`: Insert a value at a specified index.
- `da_remove`: Remove a value at a specified index.

## Resizing and Reserving Space

- `da_resize`: Adjust the capacity of the array to a specified size. Used for
preallocating space.
- `da_reserve`: Ensure there is enough space for additional elements. Used
before adding multiple elements.

## Manipulating the Array

- `da_extend`: Add multiple elements from another array or list.
- `da_map`: Transform elements of the array into another form and store them in
a destination array.
- `da_map_ctx`: Same but the mapping function also takes a context.
- `da_filter`: Filter dynamic array with a filter function and place it into a
destination.
- `da_filter_ctx`: Filter dynamic array with a filter function, that takes a
`void*` as a context, and place it into a destination.
- `da_sort`: Sort the array using a comparison function.
- `da_sort_ctx`: Sort the array using a comparison function that also takes a
`void*` as a context.
- `da_reverse`: Reverse the order of elements in the array.
*/

#ifndef __CEBUS_DA_H__
#define __CEBUS_DA_H__

// #include "cebus/core/arena.h"   // IWYU pragma: export
// #include "cebus/core/defines.h" // IWYU pragma: export
// #include "cebus/core/sorting.h" // IWYU pragma: export

///////////////////////////////////////////////////////////////////////////////

#define DA(T)                                                                  \
  struct {                                                                     \
    usize cap;                                                                 \
    usize len;                                                                 \
    Arena *arena;                                                              \
    T *items;                                                                  \
  }

#define da_first(list) (list)->items[0]
#define da_last(list) (list)->items[(list)->len - 1]
#define da_get(list, idx) (list)->items[idx]
#define da_pop(list) (list)->items[--(list)->len]
#define da_empty(list) (!(list)->len)
#define da_len(list) ((list)->len)

#define da_clear(list) ((list)->len = 0)

///////////////////////////////////////////////////////////////////////////////

#define da_new(_arena)                                                         \
  { .arena = (_arena), .items = NULL, }

// depricated
#define da_init(list, _arena)                                                  \
  do {                                                                         \
    (list)->len = 0;                                                           \
    (list)->cap = 0;                                                           \
    (list)->arena = _arena;                                                    \
    (list)->items = NULL;                                                      \
  } while (0)

#define da_init_list(list, _arena, count, array)                               \
  do {                                                                         \
    (list)->len = count;                                                       \
    (list)->cap = 0;                                                           \
    (list)->arena = _arena;                                                    \
    (list)->items = NULL;                                                      \
    da_resize(list, count);                                                    \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                          \
      (list)->items[__e_i] = (array)[__e_i];                                   \
    }                                                                          \
  } while (0)

#define da_init_static(list, _arena, ...)                                      \
  do {                                                                         \
    da_init_list(list, _arena, ARRAY_LEN((__VA_ARGS__)), (__VA_ARGS__));       \
  } while (0)

#define da_copy(src, dest)                                                     \
  do {                                                                         \
    da_resize((dest), (src)->len);                                             \
    for (usize __c_i = 0; __c_i < (src)->len; __c_i++) {                       \
      (dest)->items[__c_i] = (src)->items[__c_i];                              \
    }                                                                          \
    (dest)->len = (src)->len;                                                  \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

#define da_resize(list, size)                                                  \
  do {                                                                         \
    if (size < (list)->cap) {                                                  \
      break;                                                                   \
    }                                                                          \
    (list)->cap = size;                                                        \
    (list)->items = arena_realloc_chunk((list)->arena, (list)->items,          \
                                        (list)->cap * sizeof(*(list)->items)); \
  } while (0)

#define da_reserve(list, size)                                                 \
  do {                                                                         \
    const usize __rs = da_len(list) + size;                                    \
    if (__rs < (list)->cap) {                                                  \
      break;                                                                   \
    }                                                                          \
    usize __ns = (list)->cap == 0 ? 5 : (list)->cap;                           \
    while (__ns < __rs) {                                                      \
      __ns *= 2;                                                               \
    }                                                                          \
    da_resize(list, __ns);                                                     \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

#define da_push(list, ...)                                                     \
  do {                                                                         \
    da_reserve((list), 1);                                                     \
    da_get(list, da_len(list)++) = (__VA_ARGS__);                              \
  } while (0)

#define da_extend(list, count, ...)                                            \
  do {                                                                         \
    da_reserve((list), (count));                                               \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                          \
      da_get(list, da_len(list) + __e_i) = (__VA_ARGS__)[__e_i];               \
    }                                                                          \
    (list)->len += count;                                                      \
  } while (0)

#define da_insert(list, value, idx)                                            \
  do {                                                                         \
    da_reserve(list, 1);                                                       \
    for (usize __r_i = idx + 1; __r_i < da_len(list) + 1; __r_i++) {           \
      da_get(list, __r_i) = da_get(list, __r_i - 1);                           \
    }                                                                          \
    da_get(list, idx) = value;                                                 \
    da_len(list)++;                                                            \
  } while (0)

#define da_remove(list, idx)                                                   \
  do {                                                                         \
    for (usize __r_i = idx + 1; __r_i < da_len(list); __r_i++) {               \
      da_get(list, __r_i - 1) = da_get(list, __r_i);                           \
    }                                                                          \
    da_len(list)--;                                                            \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

#define da_map(src, dest, map)                                                 \
  do {                                                                         \
    da_reserve((dest), da_len(src));                                           \
    for (usize __m_i = 0; __m_i < da_len(src); __m_i++) {                      \
      da_get(dest, __m_i) = map(da_get(src, __m_i));                           \
    }                                                                          \
    da_len(dest) = da_len(src);                                                \
  } while (0)

#define da_map_ctx(src, dest, map, ctx)                                        \
  do {                                                                         \
    da_reserve((dest), da_len(src));                                           \
    for (usize __m_i = 0; __m_i < da_len(src); __m_i++) {                      \
      da_get(dest, __m_i) = map(ctx, da_get(src, __m_i));                      \
    }                                                                          \
    da_len(dest) = da_len(src);                                                \
  } while (0)

#define da_filter(src, dest, filter)                                           \
  do {                                                                         \
    da_reserve((dest), da_len(src));                                           \
    usize __f_count = 0;                                                       \
    for (usize __f_i = 0; __f_i < da_len(src); __f_i++) {                      \
      if (filter(da_get(src, __f_i))) {                                        \
        da_get(dest, __f_count++) = da_get(src, __f_i);                        \
      }                                                                        \
    }                                                                          \
    da_len(dest) = __f_count;                                                  \
  } while (0)

#define da_filter_ctx(src, dest, filter, ctx)                                  \
  do {                                                                         \
    da_reserve((dest), da_len(src));                                           \
    usize __f_count = 0;                                                       \
    for (usize __f_i = 0; __f_i < da_len(src); __f_i++) {                      \
      if (filter((ctx), da_get(src, __f_i))) {                                 \
        da_get(dest, __f_count++) = da_get(src, __f_i);                        \
      }                                                                        \
    }                                                                          \
    da_len(dest) = __f_count;                                                  \
  } while (0)

#define da_sort(src, dest, sort)                                               \
  do {                                                                         \
    da_reserve((dest), da_len(src));                                           \
    quicksort(&da_get(src, 0), &da_get(dest, 0), sizeof(da_get(src, 0)),       \
              da_len(src), sort);                                              \
  } while (0)

#define da_sort_ctx(src, dest, sort, ctx)                                      \
  do {                                                                         \
    da_reserve((dest), da_len(src));                                           \
    quicksort_ctx((src)->items, (dest)->items, sizeof(da_get(src, 0)),         \
                  da_len(src), sort, ctx);                                     \
  } while (0)

#define da_reverse(list)                                                       \
  do {                                                                         \
    da_reserve((list), 1);                                                     \
    for (usize __r_i = 0; __r_i < (list)->len - __r_i - 1; __r_i++) {          \
      da_get(list, da_len(list)) = da_get(list, __r_i);                        \
      da_get(list, __r_i) = da_get(list, da_len(list) - __r_i - 1);            \
      da_get(list, da_len(list) - __r_i - 1) = da_get(list, da_len(list));     \
    }                                                                          \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_DA_H__ */

/* DOCUMENTATION
My HashMap takes a unique approach: it stores only the hashes of keys, not the
keys themselves. Most of the time, you don’t really need the original keys
hanging around. If you find yourself in a situation where you do, just pair it
with a dynamic array to cover those bases. See
[this](https://github.com/Code-Nycticebus/cebus/blob/main/examples/word.c)
example.

As for the values, the HashMap is set up to work with simple, primitive
data types. You can use pointers to handle more complex values. But make sure
they have the same lifetime as the `HashMap`.

## Initialization

Creating a new `HashMap` involves initializing an `Arena`, then calling
`hm_create` or `hm_with_size` to initialize the hashmap with an optional initial
size:

```c
Arena arena = {0};
HashMap* hm = hm_create(&arena);
```

## HashMap Operations

Basic hashmap management includes clearing, copying, resizing, reserving
capacity, and updating from another hashmap:

- `hm_clear`: Clears the hashmap.
- `hm_copy`: Creates a copy of the hashmap.
- `hm_resize`: Resizes the hashmap. Used for preallocating space
- `hm_reserve`: Reserves space in the hashmap. Used before adding multiple
elements.
- `hm_update`: Merges another hashmap into the current one.

## Inserting Elements

Elements of various types can be inserted into the hashmap, including integers,
floating-point numbers, and pointers:

- `hm_insert_<T>`: Insert `u8`, `i8`, `u32`, `i32`, `u64`, `i64`, `usize`, `f32`
or `f64` values.
- `hm_insert_mut_ptr`, `hm_insert_ptr`: Insert mutable or constant pointers.

## Querying Elements

Retrieve pointers to the values stored in the hashmap by their key hashes,
allowing for mutable or immutable access. Returns `NULL` if key was not found:

> :warning: Avoid storing pointers from the hashmap for extended periods.
> Keeping these pointers beyond the immediate scope can lead to undefined
behavior, as the underlying storage may change.

- `hm_get_<T>`: Get `u8`, `i8`, `u32`, `i32`, `u64`, `i64`, `usize`, `f32`
or `f64` pointers.
- `hm_get_<T>_mut`: Get `u8`, `i8`, `u32`, `i32`, `u64`, `i64`, `usize`, `f32`
or `f64` pointers.
*/

#ifndef __CEBUS_HASHMAP_H__
#define __CEBUS_HASHMAP_H__

// #include "cebus/core/arena.h"
// #include "cebus/core/defines.h"

typedef struct HashMap HashMap;

///////////////////////////////////////////////////////////////////////////////

HashMap *hm_create(Arena *arena);
HashMap *hm_with_size(Arena *arena, usize size);

void hm_clear(HashMap *hm);

HashMap *hm_copy(HashMap *hm, Arena *arena);

void hm_resize(HashMap *hm, usize size);
void hm_reserve(HashMap *hm, usize size);

void hm_update(HashMap *hm, HashMap *other);

bool hm_remove(HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

bool hm_insert_f32(HashMap *hm, u64 hash, f32 value);
bool hm_insert_f64(HashMap *hm, u64 hash, f64 value);
bool hm_insert_i8(HashMap *hm, u64 hash, i8 value);
bool hm_insert_u8(HashMap *hm, u64 hash, u8 value);
bool hm_insert_i16(HashMap *hm, u64 hash, i16 value);
bool hm_insert_u16(HashMap *hm, u64 hash, u16 value);
bool hm_insert_i32(HashMap *hm, u64 hash, i32 value);
bool hm_insert_u32(HashMap *hm, u64 hash, u32 value);
bool hm_insert_i64(HashMap *hm, u64 hash, i64 value);
bool hm_insert_u64(HashMap *hm, u64 hash, u64 value);
bool hm_insert_usize(HashMap *hm, u64 hash, usize value);
bool hm_insert_mut_ptr(HashMap *hm, u64 hash, void *value);
bool hm_insert_ptr(HashMap *hm, u64 hash, const void *value);

///////////////////////////////////////////////////////////////////////////////

f32 *hm_get_f32_mut(const HashMap *hm, u64 hash);
f64 *hm_get_f64_mut(const HashMap *hm, u64 hash);
i8 *hm_get_i8_mut(const HashMap *hm, u64 hash);
u8 *hm_get_u8_mut(const HashMap *hm, u64 hash);
i16 *hm_get_i16_mut(const HashMap *hm, u64 hash);
u16 *hm_get_u16_mut(const HashMap *hm, u64 hash);
i32 *hm_get_i32_mut(const HashMap *hm, u64 hash);
u32 *hm_get_u32_mut(const HashMap *hm, u64 hash);
i64 *hm_get_i64_mut(const HashMap *hm, u64 hash);
u64 *hm_get_u64_mut(const HashMap *hm, u64 hash);
usize *hm_get_usize_mut(const HashMap *hm, u64 hash);
void *hm_get_ptr_mut(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

const f32 *hm_get_f32(const HashMap *hm, u64 hash);
const f64 *hm_get_f64(const HashMap *hm, u64 hash);
const i8 *hm_get_i8(const HashMap *hm, u64 hash);
const u8 *hm_get_u8(const HashMap *hm, u64 hash);
const i16 *hm_get_i16(const HashMap *hm, u64 hash);
const u16 *hm_get_u16(const HashMap *hm, u64 hash);
const i32 *hm_get_i32(const HashMap *hm, u64 hash);
const u32 *hm_get_u32(const HashMap *hm, u64 hash);
const i64 *hm_get_i64(const HashMap *hm, u64 hash);
const u64 *hm_get_u64(const HashMap *hm, u64 hash);
const usize *hm_get_usize(const HashMap *hm, u64 hash);
const void *hm_get_ptr(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_HASHMAP_H__ */

/* DOCUMENTATION
My `Set` implementation follows the same principle as my `HashMap`: it stores
only the hashes for lookup. This means you get efficient way to check
if something is in the set without the overhead of storing the actual elements.
It’s pretty handy for cases where you just need to keep track of existence
rather than the elements themselves.

## Initialization

To start using the set, you need to initialize an `Arena` and create a new set:

```c
Arena arena = {0};
Set set = set_create(&arena);
```

## Set Operations

Perform basic set operations such as adding, removing, and extending sets with
multiple elements:

- `set_add`: Add an element to the set. Returns false if the element already
exists.
- `set_remove`: Remove an element from the set.
- `set_extend`: Add multiple elements to the set at once.

Copy and resize sets for handling dynamic set sizes:

- `set_copy`: Create a duplicate of a set.
- `set_resize`: Adjust the capacity of the set. Used for preallocating
space.
- `set_reserve`: Reserves space in the set. Used before adding multiple
elements


## Set Query Operations

Query the set for the presence of elements, equality with another set, or subset
and disjoint relationships:

- `set_contains`: Check if an element is in the set.
- `set_eq`: Check if two sets are equal.
- `set_subset`: Determine if one set is a subset of another.
- `set_disjoint`: Determine if two sets have no elements in common.

## Set Algebraic Operations

Combine sets or find their differences using algebraic set operations:

- `set_intersection`: Find the intersection of two sets.
- `set_difference`: Find the difference between two sets.
- `set_union`: Combine two sets into a union.
*/

#ifndef __CEBUS_SET_H__
#define __CEBUS_SET_H__

// #include "cebus/core/arena.h"
// #include "cebus/core/defines.h"

//////////////////////////////////////////////////////////////////////////////

typedef struct {
  usize cap;
  usize count;
  usize deleted;
  Arena *arena;
  u64 *items;
} Set;

///////////////////////////////////////////////////////////////////////////////

Set set_create(Arena *arena);
Set set_with_size(Arena *arena, usize size);

void set_clear(Set *set);

Set set_copy(Arena *arena, Set *set);

void set_resize(Set *set, usize size);
void set_reserve(Set *set, usize size);

///////////////////////////////////////////////////////////////////////////////

bool set_add(Set *set, u64 hash);
void set_extend(Set *set, usize count, const u64 *hashes);
void set_update(Set *dest, const Set *set);

bool set_remove(Set *set, u64 hash);

//////////////////////////////////////////////////////////////////////////////

bool set_contains(const Set *set, u64 hash);
bool set_eq(const Set *set, const Set *other);
bool set_subset(const Set *set, const Set *other);
bool set_disjoint(const Set *set, const Set *other);

///////////////////////////////////////////////////////////////////////////////

Set set_intersection(const Set *set, const Set *other, Arena *arena);
Set set_difference(const Set *set, const Set *other, Arena *arena);
Set set_union(const Set *set, const Set *other, Arena *arena);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_SET_H__ */

/* DOCUMENTATION
The `StringBuilder` provides functionality for efficiently constructing
strings.

> :warning: StringBuilder does not construct '\0' terminated strings.

## Functions

- **`StringBuilder sb_init(Arena *arena);`**
  Initializes a new `StringBuilder` instance, allocating its buffer using the
provided memory `arena`.

- **`Str sb_to_str(StringBuilder *sb);`**
  Converts the contents of the `StringBuilder` to a `Str`, effectively
finalizing the string construction.

- **`void sb_append_parts(StringBuilder *sb, usize size, const char *s);`**
  Appends parts of a string to the `StringBuilder`, where `size` specifies the
number of characters to append, and `s` points to the string parts to be
appended.

- **`void sb_append_cstr(StringBuilder *sb, const char *cstr);`**
  Appends a C-style null-terminated string to the `StringBuilder`.

- **`void sb_append_str(StringBuilder *sb, Str str);`**
  Appends a `Str` type string to the `StringBuilder`.

- **`void sb_append_fmt(StringBuilder *sb, const char *fmt, ...);`**
  Appends a formatted string to the `StringBuilder`, similar to `printf` style
formatting.

- **`void sb_append_va(StringBuilder *sb, const char *fmt, va_list va);`**
  Appends a formatted string and va_list to the `StringBuilder`, similar to
`vprintf` style formatting.

*/

#ifndef __CEBUS_STRING_BUILDER_H__
#define __CEBUS_STRING_BUILDER_H__

// #include "cebus/core/defines.h"

// #include "cebus/collection/da.h"
// #include "cebus/core/arena.h"

#include <stdarg.h>

typedef DA(char) StringBuilder;

StringBuilder sb_init(Arena *arena);
void sb_clear(StringBuilder *sb);

Str sb_to_str(StringBuilder *sb);

void sb_append_parts(StringBuilder *sb, usize size, const char *s);
void sb_append_cstr(StringBuilder *sb, const char *cstr);
void sb_append_str(StringBuilder *sb, Str str);
void sb_append_c(StringBuilder *sb, char c);
FMT(2) usize sb_append_fmt(StringBuilder *sb, const char *fmt, ...);
usize sb_append_va(StringBuilder *sb, const char *fmt, va_list va);

#endif /* !__CEBUS_STRING_BUILDER_H__ */

#ifndef __CEBUS_ASSERTS_H__
#define __CEBUS_ASSERTS_H__

/* DOCUMENTATION
## Usage

Use the assertion macros to validate conditions and log failures:

```c
cebus_assert(1 == 1, "It needs a message");
cebus_assert(2 == 2, "It can even handle arguments: %d", 420);
```

## Assert Macros

- `cebus_assert(EXPR, FMT, ...)`: Asserts a condition and logs an error if the
condition is false.
- `cebus_assert_warn(EXPR, FMT, ...)`: Asserts a condition and logs a warning if
the condition is false.
- `cebus_assert_debug(EXPR, FMT, ...)`: Debug-only assert that logs a debug
message if the condition is false.
- `cebus_assert_return(EXPR, RETURN_VALUE)`: Asserts a condition and returns a
value if the condition is false.

## Debug Macros

- `DEBUGBREAK()`: Uses platform specific debug break.
- `UNREACHABLE()`: Prints error message in debug mode. In release mode it uses
compiler intrensics.
- `NOT_IMPLEMENTED()`: Prints error message
*/

// #include "cebus/core/platform.h" // IWYU pragma: export
// #include "logging.h"             // IWYU pragma: export

NORETURN void abort(void);

////////////////////////////////////////////////////////////////////////////

#if defined(LINUX)
#ifndef SIGTRAP
#define SIGTRAP 5
#endif
NORETURN int raise(int);
#define DEBUGBREAK() raise(SIGTRAP)
#elif defined(WINDOWS)
#include <intrin.h> // IWYU pragma: export
#define DEBUGBREAK() __debugbreak()
#else
#define DEBUGBREAK(...) abort()
#endif

////////////////////////////////////////////////////////////////////////////

#if defined(NDEBUG)
#if defined(GCC) || defined(CLANG) || defined(MINGW32) || defined(MINGW64)
#define UNREACHABLE()                                                          \
  __builtin_unreachable();                                                     \
  abort()
#elif defined(MSVC)
#define UNREACHABLE()                                                          \
  __assume(0);                                                                 \
  abort()
#else
#define UNREACHABLE() abort()
#endif
#else
#define UNREACHABLE()                                                          \
  cebus_log_error("UNREACHABLE: " FILE_LOCATION_FMT,                           \
                  FILE_LOCATION_ARG_CURRENT);                                  \
  abort()
#endif

////////////////////////////////////////////////////////////////////////////

#if defined(NDEBUG)
#define NOT_IMPLEMENTED() abort()
#else
#define NOT_IMPLEMENTED()                                                      \
  cebus_log_error("NOT IMPLEMENTED: " FILE_LOCATION_FMT,                       \
                  FILE_LOCATION_ARG_CURRENT);                                  \
  abort()
#endif

////////////////////////////////////////////////////////////////////////////

#define _cebus_assert_print(level, expr, ...)                                  \
  cebus_log_level(level, FILE_LOCATION_FMT, FILE_LOCATION_ARG_CURRENT);        \
  cebus_log_level(level, "  Assertion '%s' failed", expr);                     \
  cebus_log_level(level, "  Description: "__VA_ARGS__)

#define cebus_assert(expression, ...)                                          \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _cebus_assert_print(CEBUS_LOG_ERROR, #expression, __VA_ARGS__);          \
      DEBUGBREAK();                                                            \
    }                                                                          \
  } while (0)

#define cebus_assert_return(expression, ret)                                   \
  do {                                                                         \
    if (!(expression)) {                                                       \
      return ret;                                                              \
    }                                                                          \
  } while (0)

#define cebus_assert_warn(expression, ...)                                     \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _cebus_assert_print(CEBUS_LOG_WARNING, #expression, __VA_ARGS__);        \
    }                                                                          \
  } while (0)

#ifndef NDEBUG
#define cebus_assert_debug(expression, ...)                                    \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _cebus_assert_print(CEBUS_LOG_DEBUG, #expression, __VA_ARGS__);          \
      DEBUGBREAK();                                                            \
    }                                                                          \
  } while (0)
#else
#define cebus_assert_debug(...)
#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_ASSERTS_H__ */

/* DOCUMENTATION
### Initialization Macros
- `ErrNew`: Initializes a new Error instance.
- `ErrPanic`: Initializes an Error that will trigger a panic on `error_emit()`.
- `ErrDefault`: Empty Error that will panic on `error_emit()`.

### Error Emitting and Context
- `error_emit(E, code, fmt, ...)` initializes the passed in error.
```c
void function_that_fails(Error *error) {
  // ...
  if (failure_condition) {
    error_emit(error, error_code, "Error: %s", reason);
    return;
  }
}
```

- `error_context()` creates a context for you to handle the error. Panics if it
falls through
```c
Error error = ErrNew;
function_that_fails(&error);
error_context(&error, {
  error_raise();
});
```

- `error_propagate()` creates a context. Does not panic if it falls through but
also does not reset the error.
:warning: if the error is never handled there will be a memory leak.
```c
Error error = ErrNew;
function_that_fails(&error);
error_propagate(&error, {
  return;
});
```

### Error Handling
- `error_panic()`: Triggers a panic with the current error.
- `error_except()`: Resets the error state.
- `error_msg()`: Retrieves the error message.
- `error_code(T)`: Retrieves the error code and casts it to `T`.
- `error_set_code()`: Sets a new error code.
- `error_set_msg(fmt, ...)`: Sets a new error message and clears all notes.
- `error_add_location()`: Adds current file and line location.
- `error_add_note(fmt, ...)`: Adds a note to the error.

*/

#ifndef __CEBUS_ERROR_H__
#define __CEBUS_ERROR_H__

// #include "cebus/collection/da.h"
// #include "cebus/collection/string_builder.h"
// #include "cebus/core/arena.h"
// #include "cebus/core/defines.h"

////////////////////////////////////////////////////////////////////////////

#define ERROR_LOCATION_MAX 10

typedef struct {
  i64 code;
  Str msg;
  StringBuilder message;
  DA(FileLocation) locations;
} ErrorInfo;

typedef struct {
  Arena arena;
  bool failure;
  bool panic_on_emit;
  FileLocation location;
  ErrorInfo *info;
} Error;

#define ErrNew                                                                 \
  ((Error){                                                                    \
      .failure = false,                                                        \
      .panic_on_emit = false,                                                  \
      .location = FILE_LOCATION_CURRENT,                                       \
      .arena = {0},                                                            \
  })

#define ErrPanic                                                               \
  ((Error[]){{                                                                 \
      .failure = false,                                                        \
      .panic_on_emit = true,                                                   \
      .location = FILE_LOCATION_CURRENT,                                       \
      .arena = {0},                                                            \
  }})

#define ErrDefault ((Error *)NULL)

////////////////////////////////////////////////////////////////////////////

#define error_emit(E, code, ...)                                               \
  _error_internal_emit(E, code, FILE_LOCATION_CURRENT, __VA_ARGS__);

#define error_context(E, ...)                                                  \
  do {                                                                         \
    if (_error_internal_occured(E)) {                                          \
      Error *__error_context__ = (E);                                          \
      __VA_ARGS__                                                              \
      if ((E)->failure) {                                                      \
        _error_internal_panic(E);                                              \
      }                                                                        \
    }                                                                          \
  } while (0)

#define error_propagate(E, ...)                                                \
  do {                                                                         \
    if (_error_internal_occured(E)) {                                          \
      Error *__error_context__ = (E);                                          \
      error_add_location();                                                    \
      __VA_ARGS__                                                              \
    }                                                                          \
  } while (0)

#define error_panic() _error_internal_panic(__error_context__)
#define error_except() _error_internal_except(__error_context__)

#define error_msg() (__error_context__->info->msg)
#define error_code(T) ((T)__error_context__->info->code)

#define error_set_code(code)                                                   \
  _error_internal_set_code(__error_context__, (i64)code)
#define error_set_msg(...)                                                     \
  _error_internal_set_msg(__error_context__, __VA_ARGS__)

#define error_add_location(...)                                                \
  _error_internal_add_location(__error_context__, FILE_LOCATION_CURRENT)
#define error_add_note(...)                                                    \
  _error_internal_add_note(__error_context__, __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////

void FMT(4) _error_internal_emit(Error *err, i32 code, FileLocation location,
                                 const char *fmt, ...);
bool _error_internal_occured(Error *err);
void NORETURN _error_internal_panic(Error *err);
void _error_internal_except(Error *err);
void _error_internal_set_code(Error *err, i32 code);
void FMT(2) _error_internal_set_msg(Error *err, const char *fmt, ...);
void _error_internal_add_location(Error *err, FileLocation location);
void FMT(2) _error_internal_add_note(Error *err, const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_ERROR_H__ */

/* DOCUMENTATION
## Usage

Call the function with the message format and arguments:

```c
cebus_log_error("This is an error message: %s", error_details);
```

## Functions

- `cebus_log_level(log_level, fmt, ...)`: Logs a message with the specified log
level.
- `cebus_log(fmt, ...)`: Logs a message.
- `cebus_log_fatal(fmt, ...)`: Logs a fatal message.
- `cebus_log_error(fmt, ...)`: Logs an error message.
- `cebus_log_warning(fmt, ...)`: Logs a warning message.
- `cebus_log_info(fmt, ...)`: Logs an info message.
- `cebus_log_debug(fmt, ...)`: Logs a debug message (only in debug builds).
- `cebus_log_trace(fmt, ...)`: Logs a trace message (only in debug builds).
*/

#ifndef __CEBUS_LOGGING_H__
#define __CEBUS_LOGGING_H__

// #include "cebus/core/defines.h"

////////////////////////////////////////////////////////////////////////////

typedef enum {
  CEBUS_LOG = 0,
  CEBUS_LOG_FATAL,
  CEBUS_LOG_ERROR,
  CEBUS_LOG_WARNING,
  CEBUS_LOG_INFO,
  CEBUS_LOG_DEBUG,
  CEBUS_LOG_TRACE,
} LogLevel;

////////////////////////////////////////////////////////////////////////////

void FMT(2) cebus_log_level(LogLevel log_level, const char *fmt, ...);
void FMT(1) cebus_log(const char *fmt, ...);
void FMT(1) cebus_log_fatal(const char *fmt, ...);
void FMT(1) cebus_log_error(const char *fmt, ...);
void FMT(1) cebus_log_warning(const char *fmt, ...);
void FMT(1) cebus_log_info(const char *fmt, ...);

////////////////////////////////////////////////////////////////////////////

void FMT(1) _cebus_log_debug(const char *fmt, ...);
void FMT(1) _cebus_log_trace(const char *fmt, ...);

#ifndef NDEBUG
#define cebus_log_debug(...) _cebus_log_debug(__VA_ARGS__)
#define cebus_log_trace(...) _cebus_log_trace(__VA_ARGS__)
#else
#define cebus_log_debug(...)
#define cebus_log_trace(...)
#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_LOGGING_H__ */

/* DOCUMENTATION
## Usage

Sort an array by providing the array, its size, the number of elements, and a
comparison function:

```c
int array[5] = {5, 4, 3, 2, 1};
quicksort(array, array, sizeof(int), 5, i32_compare_qsort(CMP_LESS));
```

For context-aware comparisons, use `quicksort_ctx` with a comparison function
that takes an additional context parameter.

*/

#ifndef __CEBUS_SORTING_H__
#define __CEBUS_SORTING_H__

// #include "cebus/core/defines.h"

////////////////////////////////////////////////////////////////////////////

/* Sort an array with a function */
void quicksort(const void *src, void *dest, usize size, usize nelem,
               CompareFn compare);

////////////////////////////////////////////////////////////////////////////

/* Sort an array with a function that also takes a context */
void quicksort_ctx(const void *src, void *dest, usize size, usize nelem,
                   CompareCtxFn compare, const void *ctx);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_SORTING_H__ */

/* DOCUMENTATION
## Functions

- **`cmd_exec(error, argc, argv)`**: Executes a system command.
- **`cmd_exec_da(error, da)`**: Executes a with a dynamic array.

## Construction a dynamic array

```c
Arena arena = {0};

Cmd cmd = cmd_new(&arena);

cmd_push(&cmd, STR("gcc"), STR("-o"), STR("main"));

Str cflags[] = {STR("-Wall"), STR("-Wextra")};
cmd_extend(&cmd, words);

DA(Str) files = da_new(&arena);
// collect files...
cmd_extend_da(&cmd, &files);

cmd_exec_da(ErrPanic, &cmd);

arena_free(&arena);
```

## Error Handling

Defines `CmdError` enum for different command execution outcomes:
- `CMD_OK`: Command executed successfully.
- `CMD_FORK`: Error occurred while forking the process.
- `CMD_NOT_FOUND`: Command not found, typically returns `127`.

## Usage Example

```c
Error error = ErrNew;
Str args[] = {STR("/bin/echo"), STR("Hello, world!")};
cmd_exec(&error, 2, args);
error_context(&error, {
    // Handle error
});
```
*/

#ifndef __CEBUS_CMD_H__
#define __CEBUS_CMD_H__

// #include "cebus/collection/da.h"
// #include "cebus/core/defines.h"
// #include "cebus/core/error.h"

typedef enum {
  CMD_OK,
  CMD_FORK,
  CMD_NOT_FOUND = 127,
} CmdError;

void cmd_exec(Error *error, size_t argc, Str *argv);

typedef DA(Str) Cmd;

#define cmd_new(arena) da_new(arena)
#define cmd_push(cmd, ...)                                                     \
  da_extend(cmd, ARRAY_LEN((Str[]){__VA_ARGS__}), (Str[]){__VA_ARGS__})
#define cmd_extend(cmd, ...)                                                   \
  da_extend(cmd, ARRAY_LEN(__VA_ARGS__), (__VA_ARGS__))
#define cmd_extend_da(cmd, da) da_extend(cmd, (da)->len, (da)->items)

void cmd_exec_da(Error *error, const Cmd *cmd);

#endif // !__CEBUS_CMD_H__

/* DOCUMENTATION
## Functions

- **`dll_load(path, error)`**: Loads a dynamic link library.
- **`dll_close(handle)`**: Closes an opened dynamic link library.
- **`dll_symbol(handle, symbol, error)`**: Retrieves a symbol from the dynamic
link library.


## Usage Example

```c
Error error = ErrNew;
Dll *myLib = dll_load(STR("myLibrary.dll"), &error);
error_context(&error, {
  error_raise();
});
Function *myFunction = dll_symbol(myLib, "myFunctionName", &error);
// Use the function pointer as needed
dll_close(myLib);
```
*/

#ifndef __CEBUS_DLL_H__
#define __CEBUS_DLL_H__

// #include "cebus/core/defines.h"
// #include "cebus/core/error.h"

typedef void Dll;
typedef void (*Function)(void);

Dll *dll_load(Str path, Error *error);
void dll_close(Dll *handle);

Function dll_symbol(Dll *handle, Str symbol, Error *error);

#endif /*  !__CEBUS_DLL_H__ */

/* DOCUMENTATION
## Functions

- **Reading Files**:
  - `file_read_bytes(filename, arena, error)`: Reads the entire file into a byte
array.
  - `file_read_str(filename, arena, error)`: Reads the entire file into a
string.
  - `file_read_utf8(filename, arena, error)`: Reads the entire file into UTF-8
format.

- **Writing Files**:
  - `file_write_bytes(filename, bytes, error)`: Writes byte data to a file.
  - `file_write_str(filename, content, error)`: Writes a string to a file.
  - `file_write_utf8(filename, content, error)`: Writes UTF-8 formatted data to
a file.

- **File Management**:
  - `file_open(filename, mode, error)`: Opens a file with the specified mode.
  - `file_close(file, error)`: Closes an open file.
  - `file_rename(old_name, new_name, error)`: Renames a file.
  - `file_remove(filename, error)`: Removes a file.
  - `file_exists(filename)`: Checks if a file exists.

## Usage Example

```c
Arena arena = {0};
Error error = ErrNew;
Str content = file_read_str(STR("filename.txt"), &arena, &error);
error_context(&error, {
    error_raise();
});
arena_free(&arena);
```
*/

#ifndef __CEBUS_FS_H__
#define __CEBUS_FS_H__

// #include "cebus/core/arena.h"
// #include "cebus/core/defines.h"
// #include "cebus/core/error.h"

#include <errno.h>

////////////////////////////////////////////////////////////////////////////

typedef enum {
  FILE_OK,
  FILE_PERMISSION = EACCES,
  FILE_NOT_FOUND = ENOENT,
  FILE_INVALID = EBADF,
} FileError;

////////////////////////////////////////////////////////////////////////////

FILE *file_open(Str filename, const char *mode, Error *error);
void file_close(FILE *file, Error *error);

Bytes file_read_bytes(Str filename, Arena *arena, Error *error);
Str file_read_str(Str filename, Arena *arena, Error *error);
Utf8 file_read_utf8(Str filename, Arena *arena, Error *error);

void file_write_bytes(Str filename, Bytes bytes, Error *error);
void file_write_str(Str filename, Str content, Error *error);
void file_write_utf8(Str filename, Utf8 content, Error *error);

void file_rename(Str old_name, Str new_name, Error *error);
void file_remove(Str filename, Error *error);
bool file_exists(Str filename);

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_FS_H__ */

/* DOCUMENTATION
## Functions

- **Output**:
  - `io_write(file, fmt, ...)`: Writes a formated string into the file
  - `io_write_bytes(file, bytes, error)`: Writes byte data to a file or stream.

- **Input**:
  - `io_read_bytes(file, size, buffer, error)`: Reads a specified amount of byte
data from a file or stream into a buffer.
  - `io_read_line(file, size, buffer, error)`: Reads a line of text from a file
or stream into a buffer.
  - `input(prefix)`: Displays a prompt and reads a line of text from standard
input.

## Usage Example

```c
Error e = ErrNew;
io_write(stdout, BYTES_STR("Hello, World"), &e);
error_context(&e, { error_raise(); });

Str ret = input(STR(":> "));
printf("input: '"STR_FMT"'\n", STR_ARG(ret));
```

Output when running the example:
```console
:> name
input: 'name'
```

*/

#ifndef __CEBUS_IO_H__
#define __CEBUS_IO_H__

// #include "cebus/core/defines.h"
// #include "cebus/core/error.h"

////////////////////////////////////////////////////////////////////////////

FMT(2) usize io_write_fmt(FILE *file, const char *fmt, ...);
void io_write_bytes(FILE *file, Bytes bytes, Error *error);
void io_write_str(FILE *file, Str string, Error *error);

Bytes io_read_bytes(FILE *file, usize size, void *buffer, Error *error);
Str io_read_line(FILE *file, usize size, char *buffer, Error *error);

////////////////////////////////////////////////////////////////////////////

Str input(Str prefix);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_IO_H__ */

/* DOCUMENTATION
## Functions

- **Environment Variables**:
  - `os_getenv(env, error)`: Retrieves the value of an environment variable.

- **Current Working Directory**:
  - `os_getcwd(arena)`: Returns the current working directory, allocating memory
from the specified `Arena`.
  - `os_chdir(path)`: Changes the current working directory to the specified
path.

## Usage Example

```c
Error error = ErrNew;
Str cwd = os_getcwd(&arena);
Str home = os_getenv("HOME", &error);
error_context(&error, {
  error_raise();
});

printf("Current working directory: " STR_FMT "\n", STR_ARG(cwd));
printf("Home directory: " STR_FMT "\n", STR_ARG(home));
```
*/

#ifndef __CEBUS_OS_H__
#define __CEBUS_OS_H__

// #include "cebus/core/arena.h"
// #include "cebus/core/defines.h"
// #include "cebus/core/error.h"

////////////////////////////////////////////////////////////////////////////

Str os_getenv(const char *env, Error *error);
void os_chdir(Str path);
Str os_getcwd(Arena *arena);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_OS_H__ */

/* DOCUMENTATION
## Features and Functions

- **Creating Byte Arrays**:
  - `BYTES`: Create a byte array from byte literals.
  - `BYTES_STR`: Create a byte array from a string literal.

- **Creating from Pointer and Size**:
  - `bytes_from_parts(size, data)`: Creates a byte array from given size and
data pointer.

- **Copying Byte Arrays**:
  - `bytes_copy(bytes, arena)`: Performs a deep copy of a byte array, allocating
memory from the specified arena.

- **Slicing and Taking Bytes**:
  - `bytes_slice(bytes, idx1, idx2)`: Returns a new byte array that is a slice
from the original, from `idx1` to `idx2`.
  - `bytes_take(bytes, count)`: Modifies the original byte array to keep only
the first `count` bytes, returning the removed portion.

- **Checking Equality**:
  - `bytes_eq(b1, b2)`: Checks if two byte arrays are equal.

- **Hexadecimal Conversion**:
  - `bytes_to_hex(bytes, arena)`: Converts a byte array into a hexadecimal
string representation, using memory from the arena.
  - `bytes_from_hex(s, arena)`: Creates a byte array from a hexadecimal string,
allocating memory from the arena.

## Usage
Create new Bytes from a literal.
```c
Bytes bytes = BYTES(0xff, 0x11);
Bytes bytes_str = BYTES_STR("Bytes from a string");
```

Or from a pointer with size.
```c
int a = 69;
Bytes bytes = bytes_from_parts(sizeof(a), &a);
```

if you need to take ownership of the bytes you can copy it.
```c
int a = 69;
Bytes bytes = bytes_from_parts(sizeof(a), &a);

Arena arena = {0};
Bytes owned_bytes = bytes_copy(bytes, &arena);
```
*/

#ifndef __CEBUS_BYTES_H__
#define __CEBUS_BYTES_H__

// #include "cebus/core/arena.h"
// #include "cebus/core/defines.h"

////////////////////////////////////////////////////////////////////////////

Bytes bytes_from_parts(usize size, const void *data);

////////////////////////////////////////////////////////////////////////////

Bytes bytes_copy(Bytes bytes, Arena *arena);

////////////////////////////////////////////////////////////////////////////

Bytes bytes_slice(Bytes bytes, usize idx1, size_t idx2);
Bytes bytes_take(Bytes *bytes, usize count);

////////////////////////////////////////////////////////////////////////////

bool bytes_eq(Bytes b1, Bytes b2);
u64 bytes_hash(Bytes bytes);

////////////////////////////////////////////////////////////////////////////

Str bytes_to_hex(Bytes bytes, Arena *arena);
Bytes bytes_from_hex(Str s, Arena *arena);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_BYTES_H__ */

/* DOCUMENTATION
## Character Classification Functions

- `c_is_alnum(c)`: Checks if a character is alphanumeric.
- `c_is_alpha(c)`: Checks if a character is alphabetic.
- `c_is_lower(c)`: Checks if a character is lowercase.
- `c_is_upper(c)`: Checks if a character is uppercase.
- `c_is_space(c)`: Checks if a character is a whitespace character.
- `c_is_cntrl(c)`: Checks if a character is a control character.
- `c_is_print(c)`: Checks if a character is printable.
- `c_is_graph(c)`: Checks if a character has a graphical representation.
- `c_is_blank(c)`: Checks if a character is blank (space or tab).
- `c_is_punct(c)`: Checks if a character is punctuation.
- `c_is_digit(c)`: Checks if a character is a digit.
- `c_is_xdigit(c)`: Checks if a character is a hexadecimal digit.

## Character Conversion Functions

- `c_to_lower(c)`: Converts a character to lowercase.
- `c_to_upper(c)`: Converts a character to uppercase.
- `c_to_u8(c)`: Converts a character to an unsigned 8-bit integer.
- `c_hex_to_u8(c)`: Converts a hexadecimal character to an unsigned 8-bit
integer.
- `c_u8_to_c(d)`: Converts an unsigned 8-bit integer to a character.
- `c_u8_to_hex(d)`: Converts an unsigned 8-bit integer to a hexadecimal
character (lowercase).
- `c_u8_to_HEX(d)`: Converts an unsigned 8-bit integer to a hexadecimal
character (uppercase).
*/

#ifndef __CEBUS_CHAR_H__
#define __CEBUS_CHAR_H__

// #include "cebus/core/defines.h"

///////////////////////////////////////////////////////////////////////////////

CONST_FN bool c_is_alnum(char c);
CONST_FN bool c_is_alpha(char c);
CONST_FN bool c_is_lower(char c);
CONST_FN bool c_is_upper(char c);
CONST_FN bool c_is_space(char c);
CONST_FN bool c_is_cntrl(char c);
CONST_FN bool c_is_print(char c);
CONST_FN bool c_is_graph(char c);
CONST_FN bool c_is_blank(char c);
CONST_FN bool c_is_punct(char c);
CONST_FN bool c_is_digit(char c);
CONST_FN bool c_is_xdigit(char c);

///////////////////////////////////////////////////////////////////////////////

CONST_FN char c_to_lower(char c);
CONST_FN char c_to_upper(char c);

CONST_FN u8 c_to_u8(char c);
CONST_FN u8 c_hex_to_u8(char c);

CONST_FN char c_u8_to_c(u8 d);
CONST_FN char c_u8_to_hex(u8 d);
CONST_FN char c_u8_to_HEX(u8 d);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_CHAR_H__ */

/* DOCUMENTATION
## Functions

These functions are available for `f32` and `f64`.

- `f32_eq(a, b)`: Tests if two `f32` are equal.
- `f32_eq_eps(a, b)`: Tests if two `f32` are equal within a give epsilon.
- `f32_isnan(value)`: Tests if `f32` is `nan`.
- `f32_abs(value)`: Returns the absolute value.
- `f32_min(a, b)`: Returns the min of `a` or `b`.
- `f32_max(a, b)` Returns the max of `a` or b.
- `f32_clamp(min, max, value)`: Clamps a `f32` between `min` and `max`.
- `f32_lerp(min, max, value)`: Linear interpolation between `min` and `max`.
- `f32_rad(deg)`: Converts degrees to radians.
- `f32_deg(rad)`: Converts radians to degrees.
*/

#ifndef __CEBUS_FLOATS_H__
#define __CEBUS_FLOATS_H__

// #include "cebus/core/defines.h"

#define FLOAT_DECL(T)                                                          \
  CONST_FN bool T##_eq(T a, T b);                                              \
  CONST_FN bool T##_eq_eps(T a, T b, T epsilon);                               \
  CONST_FN bool T##_isnan(T value);                                            \
  CONST_FN T T##_abs(T value);                                                 \
  CONST_FN T T##_min(T a, T b);                                                \
  CONST_FN T T##_max(T a, T b);                                                \
  CONST_FN T T##_clamp(T min, T max, T value);                                 \
  CONST_FN T T##_lerp(T min, T max, T value);                                  \
  CONST_FN T T##_rad(T deg);                                                   \
  CONST_FN T T##_deg(T rad);

FLOAT_DECL(f32)
FLOAT_DECL(f64)

#undef FLOAT_DECL

#endif /* !__CEBUS_FLOATS_H__ */

/* DOCUMENTATION

All these functions are defined for these types: `u8`, `i8`, `u16`, `i16`,
`u32`, `i32`, `u64`, `i64`, `usize`.

## Bit Operations

- `T_reverse_bits(T value)`: Reverses the bits in `value`.
- `T_leading_ones(T value)`: Counts the number of leading ones in `value`.
- `T_trailing_ones(T value)`: Counts the number of trailing ones in `value`.
- `T_leading_zeros(T value)`: Counts the number of leading zeros in `value`.
- `T_trailing_zeros(T value)`: Counts the number of trailing zeros in `value`.
- `T_count_zeros(T value)`: Counts the number of zeros in `value`.
- `T_count_ones(T value)`: Counts the number of ones in `value`.

## Byte Order Operations

- `T_swap_bytes(T value)`: Swaps the byte order of `value`.
- `T_to_be(T value)`: Converts `value` to big endian format.
- `T_from_be(T value)`: Converts `value` from big endian format.
- `T_to_le(T value)`: Converts `value` to little endian format.
- `T_from_le(T value)`: Converts `value` from little endian format.
- `T_to_ne_bytes(T value)`: Converts `value` to native endian format.

## Math Operations

Basic math operations including max, min, and clamp are provided.

- `T_max(T a, T b)`: Returns the maximum of `a` and `b`.
- `T_min(T a, T b)`: Returns the minimum of `a` and `b`.
- `T_clamp(T min, T max, T value)`: Clamps `value` between `min` and `max`.

## Utilities

- `T_hash(T value)`: Generates a hash for `value`.
- `T_swap(T *v1, T *v2)`: Swaps the values of `v1` and `v2`.
- `T_compare_lt(T a, T b)`: Compares `a` and `b` for less than.
- `T_compare_gt(T a, T b)`: Compares `a` and `b` for greater than.
- `T_compare_qsort(CmpOrdering ordering)`: Provides a comparison function
suitable for `qsort`.

*/

#ifndef __CEBUS_INTEGERS_H__
#define __CEBUS_INTEGERS_H__

// #include "cebus/core/arena.h"
// #include "cebus/core/defines.h"

#define INTEGER_DECL(T)                                                        \
  /* BIT OPERATIONS */                                                         \
  CONST_FN T T##_reverse_bits(T value);                                        \
  CONST_FN usize T##_leading_ones(T value);                                    \
  CONST_FN usize T##_trailing_ones(T value);                                   \
  CONST_FN usize T##_leading_zeros(T value);                                   \
  CONST_FN usize T##_trailing_zeros(T value);                                  \
  CONST_FN usize T##_count_zeros(T value);                                     \
  CONST_FN usize T##_count_ones(T value);                                      \
  /* BIT OPERATIONS END */                                                     \
                                                                               \
  /* BYTE ORDER OPERATIONS */                                                  \
  CONST_FN T T##_swap_bytes(T value);                                          \
  /*  BIG ENDIAN OPERATIONS */                                                 \
  CONST_FN T T##_to_be(T value);                                               \
  CONST_FN T T##_from_be(T value);                                             \
  T T##_from_be_bytes(Bytes bytes);                                            \
  Bytes T##_to_be_bytes(T value, Arena *arena);                                \
  /*  LITTLE ENDIAN OPERATIONS */                                              \
  CONST_FN T T##_to_le(T value);                                               \
  CONST_FN T T##_from_le(T value);                                             \
  T T##_from_le_bytes(Bytes bytes);                                            \
  Bytes T##_to_le_bytes(T value, Arena *arena);                                \
  /*  NATIVE ENDIAN OPERATIONS */                                              \
  T T##_from_ne_bytes(Bytes bytes);                                            \
  Bytes T##_to_ne_bytes(T value, Arena *arena);                                \
  /* BYTE ORDER OPERATIONS END */                                              \
                                                                               \
  /* MATH OPERATIONS */                                                        \
  CONST_FN T T##_max(T a, T b);                                                \
  CONST_FN T T##_min(T a, T b);                                                \
  CONST_FN T T##_clamp(T min, T max, T value);                                 \
  /* MATH OPERATIONS END */                                                    \
                                                                               \
  /* UTILS */                                                                  \
  CONST_FN u64 T##_hash(T value);                                              \
  void T##_swap(T *v1, T *v2);                                                 \
  CONST_FN CmpOrdering T##_compare_lt(T a, T b);                               \
  CONST_FN CmpOrdering T##_compare_gt(T a, T b);                               \
  CONST_FN CompareFn T##_compare_qsort(CmpOrdering ordering);                  \
  /* UTILS END */

INTEGER_DECL(u8)
INTEGER_DECL(i8)
INTEGER_DECL(u16)
INTEGER_DECL(i16)
INTEGER_DECL(u32)
INTEGER_DECL(i32)
INTEGER_DECL(u64)
INTEGER_DECL(i64)
INTEGER_DECL(usize)

#undef INTEGER_DECL

#endif /* !__CEBUS_INTEGERS_H__ */

/* DOCUMENTATION
## Features and Functions
- **String Creation and Printing**:
  - `STR("Hello World")`: Create a new string from a string literal.
  - `STR_STATIC("Hello")`: Create a new static string from a string literal.
  - `str_from_cstr(str)`: Create new string from a char array.
  - `str_from_bytes(str)`: Create new string from bytes.
  - `str_format(fmt, ...)`: Create new string as formated.
  - `printf(STR_FMT"\n", STR_ARG(str))`: Print strings using macros.

- **String Manipulation**:
  - `str_lower(str, &arena)`, `str_upper(str, &arena)`: Convert to lower or
upper case.
  - `str_append(str, suffix, &arena)`, `str_prepend(str, prefix, &arena)`:
Append or prepend strings.
  - `str_wrap(str, wrap, &arena)`: Wrap a string.
  - `str_join(sep, count, strs, &arena)`: Join strings with a separator.

- **String Trimming and Slicing**:
  - `str_trim(str)`, `str_trim_left(str)`, `str_trim_right(str)`: Trim
whitespace.
  - `str_chop_by_delim(str, delim)`, `str_try_chop_by_delim(str, delim,
&chunk)`: Chop strings by delimiter.
  - `str_substring(str, start, end)`: Extract a substring.

- **String Comparison and Search**:
  - `str_eq(s1, s2)`, `str_eq_ignorecase(s1, s2)`: Check string equality.
  - `str_startswith(str, prefix)`, `str_endswith(str, suffix)`: Check
prefixes/suffixes.
  - `str_contains(haystack, needle)`: Check if string contains a substring.

- **Conversion and Utility**:
  - `str_to_u64(str)`, `str_u64(n, &arena)`: Convert between strings and
unsigned 64-bit integers.
  - `str_hash(str)`: Generate a hash value for a string.

## Usage Example

```c
Arena arena = {0};
Str greeting = STR("Hello World");
Str lower = str_lower(greeting, &arena);
printf(STR_FMT"\n", STR_ARG(lower));
```
*/

#ifndef __CEBUS_STR_H__
#define __CEBUS_STR_H__

// #include "cebus/core/arena.h"
// #include "cebus/core/defines.h" // IWYU pragma: private: include "str.h"

///////////////////////////////////////////////////////////////////////////////

#define STR_NOT_FOUND SIZE_MAX

#define STR_LETTERS STR("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define STR_UPPERCASE STR("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define STR_LOWERCASE STR("abcdefghijklmnopqrstuvwxyz")
#define STR_DIGITS STR("0123456789")
#define STR_HEXDIGITS STR("0123456789abcdefABCDEF")
#define STR_PUNCTUATION STR("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~")
#define STR_WHITESPACE STR(" \t\n\r\x0b\x0c")

///////////////////////////////////////////////////////////////////////////////

Str str_from_parts(usize size, const char *cstr);
Str str_from_bytes(Bytes bytes);
Bytes str_to_bytes(Str s);
Str str_from_cstr(const char *cstr);
FMT(2) Str str_format(Arena *arena, const char *fmt, ...);

///////////////////////////////////////////////////////////////////////////////

Str str_copy(Str s, Arena *arena);
Str str_append(Str s1, Str suffix, Arena *arena);
Str str_prepend(Str s1, Str prefix, Arena *arena);
Str str_wrap(Str s, Str wrap, Arena *arena);

// Inserts sep in between elements
Str str_join(Str sep, usize count, Str *s, Arena *arena);
// Appends suffix to every element
Str str_join_suffix(Str suffix, usize count, Str *s, Arena *arena);
// Prepends prefix to every element
Str str_join_prefix(Str prefix, usize count, Str *s, Arena *arena);

Str str_join_wrap(Str sep, Str wrap, usize count, Str *s, Arena *arena);

Str str_upper(Str s, Arena *arena);
Str str_lower(Str s, Arena *arena);
Str str_replace(Str s, Str old, Str new, Arena *arena);
Str str_center(Str s, usize width, char fillchar, Arena *arena);
Str str_ljust(Str s, usize width, char fillchar, Arena *arena);
Str str_rjust(Str s, usize width, char fillchar, Arena *arena);
Str str_repeat(Str s, usize count, Arena *arena);
Str str_reverse(Str s, Arena *arena);

///////////////////////////////////////////////////////////////////////////////

bool str_eq(Str s1, Str s2);
bool str_eq_ignorecase(Str s1, Str s2);
bool str_startswith(Str s1, Str prefix);
bool str_endswith(Str s1, Str suffix);

bool str_contains(Str haystack, Str needle);
bool str_includes(Str haystack, char needle);
bool str_empty(Str s);
CmpOrdering str_compare_gt(Str s1, Str s2);
CmpOrdering str_compare_lt(Str s1, Str s2);

// Returns function that you can pass to qsort
CompareFn str_compare_qsort(CmpOrdering ordering);

///////////////////////////////////////////////////////////////////////////////

Str str_trim_left(Str s);
Str str_trim_left_by_delim(Str s, char delim);
Str str_trim_left_by_predicate(Str s, bool (*predicate)(char));

Str str_trim_right(Str s);
Str str_trim_right_by_delim(Str s, char delim);
Str str_trim_right_by_predicate(Str s, bool (*predicate)(char));

Str str_trim(Str s);
Str str_trim_by_delim(Str s, char delim);
Str str_trim_by_predicate(Str s, bool (*predicate)(char));

bool str_try_chop_by_delim(Str *s, char delim, Str *chunk);
Str str_chop_by_delim(Str *s, char delim);
bool str_try_chop_by_predicate(Str *s, bool (*predicate)(char), Str *chunk);
Str str_chop_by_predicate(Str *s, bool (*predicate)(char));
Str str_chop_right_by_delim(Str *s, char delim);
Str str_chop_right_by_predicate(Str *s, bool (*predicate)(char));
Str str_take(Str *s, usize count);
bool str_try_take(Str *s, usize n, Str *chunk);

Str str_substring(Str s, usize start, usize end);

///////////////////////////////////////////////////////////////////////////////

u64 str_u64(Str s);
u64 str_chop_u64(Str *s);

i64 str_i64(Str s);
i64 str_chop_i64(Str *s);

f64 str_f64(Str s);
f64 str_chop_f64(Str *s);

///////////////////////////////////////////////////////////////////////////////

// Returns 'STR_NOT_FOUND' if 'needle' was not found.
usize str_find(Str haystack, Str needle);
// Returns 'STR_NOT_FOUND' if 'needle' was not found.
usize str_find_last(Str haystack, Str needle);
usize str_count(Str haystack, Str needle);
// Returns '\0' if the index is out of bounds.
char str_getc(Str s, usize idx);

// Basic FNV hash.
u64 str_hash(Str s);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_STR_H__ */

#ifndef __CEBUS_UTF8_H__
#define __CEBUS_UTF8_H__

// #include "cebus/core/arena.h"
// #include "cebus/core/defines.h" // IWYU pragma: private: include "utf8.h"
// #include "cebus/core/error.h"

typedef enum {
  UTF8_OK,
  UTF8_DECODE,
  UTF8_ENCODE,
} Utf8Error;

Utf8 utf8_decode(Bytes bytes, Error *error);
Bytes utf8_encode(Utf8 s, Error *error);

bool utf8_eq(Utf8 s1, Utf8 s2);
bool utf8_starts_with(Utf8 s, Utf8 prefix);
bool utf8_ends_with(Utf8 s, Utf8 suffix);

bool utf8_try_next(Utf8 *str, Utf8 *out);
Utf8 utf8_next(Utf8 *str);

Utf8 utf8_copy(Utf8 str, Arena *arena);
Utf8 utf8_append(Utf8 s, Utf8 suffix, Arena *arena);
Utf8 utf8_prepend(Utf8 s, Utf8 prefix, Arena *arena);
Utf8 utf8_join(Utf8 sep, usize count, Utf8 *s, Arena *arena);
Utf8 utf8_upper(Utf8 s, Arena *arena);
Utf8 utf8_lower(Utf8 s, Arena *arena);
// Utf8 utf8_replace(Utf8 s, Utf8 old, Utf8 new, Arena *arena);
// Utf8 utf8_center(Utf8 s, usize width, char fillchar, Arena *arena);
// Utf8 utf8_ljust(Utf8 s, usize width, char fillchar, Arena *arena);
// Utf8 utf8_rjust(Utf8 s, usize width, char fillchar, Arena *arena);
// Utf8 utf8_repeat(Utf8 s, usize count, Arena *arena);
// Utf8 utf8_reverse(Utf8 s, Arena *arena);

bool utf8_validate_bytes(Bytes s);
bool utf8_validate(Utf8 s);

#endif /* !__CEBUS_UTF8_H__ */

#ifdef CEBUS_IMPLEMENTATION
// #include "hashmap.h"

// #include "cebus/core/debug.h"
// #include "cebus/core/defines.h"
// #include "cebus/type/integer.h"

#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

#define HM_TYPES(DO)                                                           \
  DO(f32)                                                                      \
  DO(f64)                                                                      \
  DO(i8)                                                                       \
  DO(u8)                                                                       \
  DO(i16)                                                                      \
  DO(u16)                                                                      \
  DO(i32)                                                                      \
  DO(u32)                                                                      \
  DO(i64)                                                                      \
  DO(u64)                                                                      \
  DO(usize)

#define HM_DECLARE_ENUM(T) HM_TYPE_##T,

typedef enum {
  HM_NONE,
  HM_PTR,
  HM_CONST_PTR,
  HM_TYPES(HM_DECLARE_ENUM)
} HashTypes;

#define HM_DECLARE_MEMBER(T) T T;

typedef struct {
  union {
    void *ptr;
    const void *const_ptr;
    HM_TYPES(HM_DECLARE_MEMBER)
  } as;
} HashValue;

typedef struct {
  u64 key;
  HashValue value;
} HashNode;

struct HashMap {
  HashTypes type;
  usize cap;
  usize count;
  usize deleted;
  Arena *arena;
  HashNode *nodes;
};

////////////////////////////////////////////////////////////////////////////

#define HM_DEFAULT_SIZE 8
#define HM_DELETED_HASH ((u64)0xdeaddeaddeaddead)

////////////////////////////////////////////////////////////////////////////

static bool hm_insert(HashMap *hm, u64 hash, HashValue value) {
  if (hash == 0 || hash == HM_DELETED_HASH) {
    hash = u64_hash(hash);
  }
  if (hm->cap <= hm->count + hm->deleted) {
    hm_resize(hm, hm->cap * 2);
  }

  while (true) {
    usize idx = hash % hm->cap;

    for (usize i = 0; i < hm->cap; i++) {
      if (!hm->nodes[idx].key) {
        hm->nodes[idx] = (HashNode){.key = hash, .value = value};
        hm->count++;
        return true;
      }
      if (hm->nodes[idx].key == hash) {
        hm->nodes[idx].value = value;
        return false;
      }
      idx = (idx + i * i) % hm->cap;
    }

    hm_resize(hm, hm->cap * 2);
  }
}

static HashValue *hm_get(const HashMap *hm, u64 hash) {
  if (hm->count == 0) {
    return NULL;
  }
  if (hash == 0 || hash == HM_DELETED_HASH) {
    hash = u64_hash(hash);
  }

  usize idx = hash % hm->cap;
  for (usize i = 0; i < hm->cap; i++) {
    if (hm->nodes[idx].key == 0) {
      return NULL;
    }
    if (hm->nodes[idx].key == hash) {
      return &hm->nodes[idx].value;
    }
    idx = (idx + i * i) % hm->cap;
  }
  return NULL;
}

static const char *hm_type(HashTypes type) {
#define RETURN_STR(T)                                                          \
  case HM_TYPE_##T:                                                            \
    return #T;

  switch (type) {
  case HM_PTR:
    return "HM_PTR";
  case HM_CONST_PTR:
    return "HM_CONST_PTR";
  case HM_NONE:
    return "HM_NONE";

    HM_TYPES(RETURN_STR)
  }

  UNREACHABLE();
}

////////////////////////////////////////////////////////////////////////////

HashMap *hm_create(Arena *arena) {
  HashMap *hm = arena_calloc(arena, sizeof(HashMap));
  hm->type = HM_NONE;
  hm->arena = arena;
  return hm;
}

HashMap *hm_with_size(Arena *arena, usize size) {
  HashMap *hm = arena_calloc(arena, sizeof(HashMap));
  hm->type = HM_NONE;
  hm->arena = arena;
  hm->cap = usize_max(size, HM_DEFAULT_SIZE);
  hm->nodes = arena_calloc_chunk(arena, hm->cap * sizeof(hm->nodes[0]));
  return hm;
}

void hm_clear(HashMap *hm) {
  hm->type = HM_NONE;
  hm->count = 0;
  memset(hm->nodes, 0, hm->cap * sizeof(hm->nodes[0]));
}

HashMap *hm_copy(HashMap *hm, Arena *arena) {
  HashMap *new = hm_with_size(arena, hm->count * 2);
  new->type = hm->type;
  for (size_t i = 0; i < hm->cap; i++) {
    if (hm->nodes[i].key && hm->nodes[i].key != HM_DELETED_HASH) {
      hm_insert(new, hm->nodes[i].key, hm->nodes[i].value);
    }
  }
  return new;
}

void hm_resize(HashMap *hm, usize size) {
  if (size < hm->cap) {
    return;
  }
  usize old_cap = hm->cap;
  HashNode *old_nodes = hm->nodes;

  hm->cap = size == 0 ? HM_DEFAULT_SIZE : size;
  hm->nodes = arena_calloc_chunk(hm->arena, hm->cap * sizeof(hm->nodes[0]));

  hm->count = 0;
  hm->deleted = 0;
  for (usize i = 0; i < old_cap; ++i) {
    if (old_nodes[i].key && hm->nodes[i].key != HM_DELETED_HASH) {
      hm_insert(hm, old_nodes[i].key, old_nodes[i].value);
    }
  }
  arena_free_chunk(hm->arena, old_nodes);
}

void hm_reserve(HashMap *hm, usize size) {
  const usize required_size = hm->count + size;
  if (required_size < hm->cap) {
    return;
  }
  usize new_size = hm->cap == 0 ? HM_DEFAULT_SIZE : hm->cap;
  while (new_size < required_size) {
    new_size *= 2;
  }
  hm_resize(hm, new_size);
}

void hm_update(HashMap *hm, HashMap *other) {
  hm_reserve(hm, other->count);
  for (usize i = 0; i < other->cap; ++i) {
    if (other->nodes[i].key && other->nodes[i].key != HM_DELETED_HASH) {
      hm_insert(hm, other->nodes[i].key, other->nodes[i].value);
    }
  }
}

bool hm_remove(HashMap *hm, u64 hash) {
  if (hm->count == 0) {
    return false;
  }
  if (hash == 0 || hash == HM_DELETED_HASH) {
    hash = u64_hash(hash);
  }

  usize idx = hash % hm->cap;
  for (usize i = 0; i < hm->cap; i++) {
    if (hm->nodes[idx].key && hm->nodes[idx].key == hash) {
      hm->nodes[idx].key = HM_DELETED_HASH;
      hm->count--;
      hm->deleted++;
      return true;
    }
    idx = (idx + i * i) % hm->cap;
  }
  return false;
}

#define TYPE_CHECK(hm, T, ret)                                                 \
  do {                                                                         \
    if (hm->type != HM_NONE && (hm->type != T)) {                              \
      cebus_log_warning("HashMap Type Error: expected '%s' got '%s'",          \
                        hm_type(hm->type), #T);                                \
      return ret;                                                              \
    }                                                                          \
  } while (0)

#define HM_INSERT_IMPL(T)                                                      \
  bool hm_insert_##T(HashMap *hm, u64 hash, T value) {                         \
    TYPE_CHECK(hm, HM_TYPE_##T, false);                                        \
    hm->type = HM_TYPE_##T;                                                    \
    return hm_insert(hm, hash, (HashValue){.as.T = value});                    \
  }

HM_TYPES(HM_INSERT_IMPL)

bool hm_insert_mut_ptr(HashMap *hm, u64 hash, void *value) {
  TYPE_CHECK(hm, HM_PTR, false);
  hm->type = HM_PTR;
  return hm_insert(hm, hash, (HashValue){.as.ptr = value});
}

bool hm_insert_ptr(HashMap *hm, u64 hash, const void *value) {
  TYPE_CHECK(hm, HM_CONST_PTR, false);
  hm->type = HM_CONST_PTR;
  return hm_insert(hm, hash, (HashValue){.as.const_ptr = value});
}

////////////////////////////////////////////////////////////////////////////

#define HM_GET_MUT_IMPL(T)                                                     \
  T *hm_get_##T##_mut(const HashMap *hm, u64 hash) {                           \
    TYPE_CHECK(hm, HM_TYPE_##T, NULL);                                         \
    HashValue *value = hm_get(hm, hash);                                       \
    return value ? &value->as.T : NULL;                                        \
  }

HM_TYPES(HM_GET_MUT_IMPL)

void *hm_get_ptr_mut(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_PTR, NULL);
  HashValue *value = hm_get(hm, hash);
  return value ? value->as.ptr : NULL;
}

///////////////////////////////////////////////////////////////////////////////

#define HM_GET_IMPL(T)                                                         \
  const T *hm_get_##T(const HashMap *hm, u64 hash) {                           \
    return hm_get_##T##_mut(hm, hash);                                         \
  }

HM_TYPES(HM_GET_IMPL)

const void *hm_get_ptr(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_CONST_PTR && hm->type != HM_PTR, NULL);
  HashValue *value = hm_get(hm, hash);
  return value ? value->as.const_ptr : NULL;
}

///////////////////////////////////////////////////////////////////////////////

// #include "set.h"

// #include "cebus/type/integer.h"

#include <string.h>

//////////////////////////////////////////////////////////////////////////////

#define SET_DEFAULT_SIZE 8
#define SET_DELETED_HASH 0xdeaddeaddeaddead

//////////////////////////////////////////////////////////////////////////////

Set set_create(Arena *arena) {
  Set set = {0};
  set.arena = arena;
  return set;
}

Set set_with_size(Arena *arena, usize size) {
  Set set = {0};
  set.arena = arena;
  set.cap = usize_max(size, SET_DEFAULT_SIZE);
  set.items = arena_calloc_chunk(arena, set.cap * sizeof(set.items[0]));
  return set;
}

void set_clear(Set *set) {
  set->count = 0;
  set->deleted = 0;
  memset(set->items, 0, set->cap * sizeof(set->items[0]));
}

Set set_copy(Arena *arena, Set *set) {
  Set new = set_with_size(arena, set->count * 2);
  for (size_t i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      set_add(&new, set->items[i]);
    }
  }
  return new;
}

void set_resize(Set *set, usize size) {
  if (size < set->cap) {
    return;
  }
  usize old_cap = set->cap;
  u64 *old_items = set->items;

  set->cap = size == 0 ? SET_DEFAULT_SIZE : size;
  set->items = arena_calloc_chunk(set->arena, set->cap * sizeof(set->items[0]));

  set->count = 0;
  set->deleted = 0;
  for (usize i = 0; i < old_cap; ++i) {
    if (old_items[i] && old_items[i] != SET_DELETED_HASH) {
      set_add(set, old_items[i]);
    }
  }
  arena_free_chunk(set->arena, old_items);
}

void set_reserve(Set *set, usize size) {
  const usize required_size = set->count + size;
  if (required_size < set->cap) {
    return;
  }
  usize new_size = set->cap == 0 ? SET_DEFAULT_SIZE : set->cap;
  while (new_size < required_size) {
    new_size *= 2;
  }
  set_resize(set, new_size);
}

//////////////////////////////////////////////////////////////////////////////

bool set_add(Set *set, u64 hash) {
  if (hash == 0 || hash == SET_DELETED_HASH) {
    hash = u64_hash(hash);
  }
  if (set->cap <= set->count + set->deleted) {
    set_resize(set, set->cap * 2);
  }

  while (true) {
    usize idx = hash % set->cap;

    for (usize i = 0; i < set->cap; i++) {
      if (!set->items[idx]) {
        set->items[idx] = hash;
        set->count++;
        return true;
      }
      if (set->items[idx] == hash) {
        return false;
      }
      idx = (idx + i * i) % set->cap;
    }

    set_resize(set, set->cap * 2);
  }
}

void set_extend(Set *set, usize count, const u64 *hashes) {
  set_reserve(set, count);
  for (usize i = 0; i < count; i++) {
    set_add(set, hashes[i]);
  }
}

void set_update(Set *dest, const Set *set) {
  set_reserve(dest, set->count);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i]) {
      set_add(dest, set->items[i]);
    }
  }
}

bool set_remove(Set *set, u64 hash) {
  if (set->count == 0) {
    return false;
  }
  if (hash == 0 || hash == SET_DELETED_HASH) {
    hash = u64_hash(hash);
  }

  usize idx = hash % set->cap;
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[idx] && set->items[idx] == hash) {
      set->items[idx] = SET_DELETED_HASH;
      set->count--;
      set->deleted++;
      return true;
    }
    idx = (idx + i * i) % set->cap;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////

bool set_contains(const Set *set, u64 hash) {
  if (hash == 0 || hash == SET_DELETED_HASH) {
    hash = u64_hash(hash);
  }

  usize idx = hash % set->cap;
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[idx] == 0) {
      return false;
    }
    if (set->items[idx] == hash) {
      return true;
    }
    idx = (idx + i * i) % set->cap;
  }
  return false;
}

bool set_eq(const Set *set, const Set *other) {
  if (other->count != set->count) {
    return false;
  }

  if (other->cap < set->cap) {
    const Set *temp = set;
    set = other;
    other = temp;
  }
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(other, set->items[i])) {
        return false;
      }
    }
  }
  return true;
}

bool set_subset(const Set *set, const Set *other) {
  if (other->count < set->count) {
    return false;
  }
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(other, set->items[i])) {
        return false;
      }
    }
  }
  return true;
}

bool set_disjoint(const Set *set, const Set *other) {
  if (other->count == 0 || set->count == 0) {
    return true;
  }

  if (other->cap < set->cap) {
    const Set *temp = set;
    set = other;
    other = temp;
  }
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (set_contains(other, set->items[i])) {
        return false;
      }
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////

Set set_intersection(const Set *set, const Set *other, Arena *arena) {
  if (other->cap < set->cap) {
    const Set *temp = set;
    set = other;
    other = temp;
  }

  Set intersection = set_create(arena);
  set_reserve(&intersection, usize_min(set->count, other->count) * 2);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (set_contains(other, set->items[i])) {
        set_add(&intersection, set->items[i]);
      }
    }
  }
  return intersection;
}

Set set_difference(const Set *set, const Set *other, Arena *arena) {
  Set difference = set_create(arena);
  set_reserve(&difference, set->count * 2);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(other, set->items[i])) {
        set_add(&difference, set->items[i]);
      }
    }
  }
  return difference;
}

Set set_union(const Set *set, const Set *other, Arena *arena) {
  Set _union = set_with_size(arena, set->count * 2);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(other, set->items[i])) {
        set_add(&_union, set->items[i]);
      }
    }
  }

  for (usize i = 0; i < other->cap; i++) {
    if (other->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(set, other->items[i])) {
        set_add(&_union, other->items[i]);
      }
    }
  }
  return _union;
}

//////////////////////////////////////////////////////////////////////////////

// #include "string_builder.h"

// #include "cebus/type/string.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

StringBuilder sb_init(Arena *arena) {
  StringBuilder sb = {0};
  da_init(&sb, arena);
  return sb;
}

void sb_clear(StringBuilder *sb) { da_clear(sb); }

Str sb_to_str(StringBuilder *sb) { return str_from_parts(sb->len, sb->items); }

void sb_append_parts(StringBuilder *sb, usize size, const char *s) {
  da_extend(sb, size, s);
}

void sb_append_cstr(StringBuilder *sb, const char *cstr) {
  da_extend(sb, strlen(cstr), cstr);
}

void sb_append_str(StringBuilder *sb, Str str) {
  da_extend(sb, str.len, str.data);
}

void sb_append_c(StringBuilder *sb, char c) { da_push(sb, c); }

usize sb_append_fmt(StringBuilder *sb, const char *fmt, ...) {
  va_list va;

  // get the size size without '\0'
  va_start(va, fmt);
  usize size = (usize)vsnprintf(NULL, 0, fmt, va);
  va_end(va);

  // allocate +1 so it does not overwrite
  da_reserve(sb, size + 1);
  // write into buffer
  va_start(va, fmt);
  vsnprintf(&da_last(sb) + 1, size + 1, fmt, va);
  va_end(va);
  sb->len += size;

  return size;
}

usize sb_append_va(StringBuilder *sb, const char *fmt, va_list va) {
  // copy the va_list
  va_list va2;
  va_copy(va2, va);

  // get the size size without '\0'
  usize size = (usize)vsnprintf(NULL, 0, fmt, va);

  // allocate +1 so it does not overwrite
  da_reserve(sb, size + 1);

  // write into buffer
  vsnprintf(&da_last(sb) + 1, size + 1, fmt, va2);
  sb->len += size;

  return size;
}

// #include "arena.h"

// #include "cebus/core/debug.h"
// #include "cebus/core/defines.h"
// #include "cebus/type/integer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

#define CHUNK_DEFAULT_SIZE KILOBYTES(8)

struct Chunk {
  Chunk *next, *prev;
  usize cap;
  usize allocated;
  u8 data[];
};

////////////////////////////////////////////////////////////////////////////

static Chunk *chunk_allocate(usize size) {
  Chunk *chunk = malloc(sizeof(Chunk) + size);
  cebus_assert(chunk != NULL, "Memory allocation failed: %s", strerror(errno));
  chunk->cap = size;
  chunk->allocated = 0;
  chunk->next = chunk->prev = 0;
  return chunk;
}

static void chunk_free(Chunk *chunk) { free(chunk); }

CONST_FN static usize align(usize size) {
  const usize mask = sizeof(void *) - 1;
  return (size + mask) & ~mask;
}

////////////////////////////////////////////////////////////////////////////

void arena_free(Arena *arena) {
  Chunk *next = arena->begin;
  while (next != NULL) {
    Chunk *temp = next;
    next = next->next;
    chunk_free(temp);
  }
  arena->begin = NULL;
}

void arena_reset(Arena *arena) {
  for (Chunk *next = arena->begin; next != NULL; next = next->next) {
    next->allocated = 0;
  }
}

void *arena_alloc(Arena *arena, usize size) {
  size = align(size);
  Chunk *chunk = arena->begin;
  for (; chunk != NULL; chunk = chunk->next) {
    cebus_assert_debug(size <= SIZE_MAX - chunk->allocated, "integer overflow");
    if (chunk->allocated + size < chunk->cap) {
      break;
    }
  }
  if (chunk == NULL) {
    const usize chunk_size = usize_max(size, CHUNK_DEFAULT_SIZE);
    chunk = chunk_allocate(chunk_size);
    chunk->next = arena->begin;
    if (arena->begin) {
      arena->begin->prev = chunk;
    }
    arena->begin = chunk;
  }
  void *ptr = &chunk->data[chunk->allocated];
  chunk->allocated += size;
  return ptr;
}

void *arena_calloc(Arena *arena, usize size) {
  void *ptr = arena_alloc(arena, size);
  memset(ptr, 0, size);
  return ptr;
}

////////////////////////////////////////////////////////////////////////////

void *arena_alloc_chunk(Arena *arena, usize size) {
  Chunk *chunk = chunk_allocate(size);
  chunk->allocated = size;
  chunk->next = arena->begin;
  if (arena->begin) {
    arena->begin->prev = chunk;
  }
  arena->begin = chunk;
  return chunk->data;
}

void *arena_calloc_chunk(Arena *arena, usize size) {
  void *data = arena_alloc_chunk(arena, size);
  memset(data, 0, size);
  return data;
}

void *arena_realloc_chunk(Arena *arena, void *ptr, usize size) {
  if (ptr == NULL) {
    return arena_alloc_chunk(arena, size);
  }
  Chunk *chunk = (Chunk *)((u8 *)ptr - sizeof(Chunk));
  if (size < chunk->allocated) {
    return chunk->data;
  }
  Chunk *new_chunk = realloc(chunk, sizeof(Chunk) + size);
  if (new_chunk->prev) {
    new_chunk->prev->next = new_chunk;
  }
  if (new_chunk->next) {
    new_chunk->next->prev = new_chunk;
  }
  if (arena->begin == chunk) {
    arena->begin = new_chunk;
  }
  return new_chunk->data;
}

void arena_free_chunk(Arena *arena, void *ptr) {
  if (ptr == NULL) {
    return;
  }
  Chunk *chunk = (Chunk *)((u8 *)ptr - sizeof(Chunk));
  if (chunk == arena->begin) {
    arena->begin = chunk->next;
  }
  if (chunk->prev) {
    chunk->prev->next = chunk->next;
  }
  if (chunk->next) {
    chunk->next->prev = chunk->prev;
  }
  free(chunk);
}

////////////////////////////////////////////////////////////////////////////

// #include "error.h"

// #include "cebus/core/arena.h"
// #include "cebus/core/error.h"
// #include "cebus/type/string.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////

static void error_dump(Error *error) {

  fprintf(stderr, "[Error]: %s:%d: %s()\n", error->location.file,
          error->location.line, error->location.function);
  Str message = sb_to_str(&error->info->message);
  Str msg = str_chop_by_delim(&message, '\n');
  fprintf(stderr, "  [Message]: " STR_FMT "\n", STR_ARG(msg));
  for (Str note = {0}; str_try_chop_by_delim(&message, '\n', &note);) {
    fprintf(stderr, "  [NOTE]: " STR_FMT "\n", STR_ARG(note));
  }

  // Stack trace
  fprintf(stderr, "[STACK TRACE]\n");
  usize location_count = error->info->locations.len;
  for (usize i = 0; i < location_count; ++i) {
    FileLocation *location = &da_pop(&error->info->locations);
    fprintf(stderr, "  [%" USIZE_FMT "]: %s:%d: %s()\n", i + 1, location->file,
            location->line, location->function);
  }
}

////////////////////////////////////////////////////////////////////////////

void _error_internal_emit(Error *err, i32 code, FileLocation location,
                          const char *fmt, ...) {
  if (err == ErrDefault) {
    err = ((Error[]){{
        .failure = true,
        .panic_on_emit = true,
        .location = location,
        .arena = {0},
    }});
  }

  err->failure = true;

  err->info = arena_calloc(&err->arena, sizeof(ErrorInfo));
  err->info->code = code;

  err->info->message = sb_init(&err->arena);
  da_init(&err->info->locations, &err->arena);

  da_push(&err->info->locations, location);

  va_list va;
  va_start(va, fmt);
  sb_append_va(&err->info->message, fmt, va);
  va_end(va);
  err->info->msg = sb_to_str(&err->info->message);

  if (err->panic_on_emit) {
    _error_internal_panic(err);
  }
}

bool _error_internal_occured(Error *err) { return err && err->failure; }

void _error_internal_panic(Error *err) {
  error_dump(err);
  arena_free(&err->arena);
  abort();
}

void _error_internal_except(Error *err) {
  arena_free(&err->arena);
  err->info = NULL;
  err->failure = false;
}

void _error_internal_set_code(Error *err, i32 code) { err->info->code = code; }

void _error_internal_set_msg(Error *err, const char *fmt, ...) {
  sb_clear(&err->info->message);
  va_list va;
  va_start(va, fmt);
  sb_append_va(&err->info->message, fmt, va);
  va_end(va);
  err->info->msg = sb_to_str(&err->info->message);
}

void _error_internal_add_note(Error *err, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  sb_append_c(&err->info->message, '\n');
  sb_append_va(&err->info->message, fmt, va);
  va_end(va);

  err->info->msg.data = err->info->message.items;
}

void _error_internal_add_location(Error *err, FileLocation location) {
  da_push(&err->info->locations, location);
}

////////////////////////////////////////////////////////////////////////////

// #include "logging.h"

#include <stdarg.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////

#if defined(LINUX)
#include <unistd.h>
#elif defined(WINDOWS)
#include <io.h>
#define isatty(...) _isatty(__VA_ARGS__)
#else
#define isatty(...) false
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 1
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 2
#endif

////////////////////////////////////////////////////////////////////////////

struct CmLogLevelPrefix {
  const char *prefix;
  const char *color;
  int file;
};

static const struct CmLogLevelPrefix log_level_str[] = {
    [CEBUS_LOG_FATAL] = {"FATAL", "\033[1m\033[91m", STDERR_FILENO},
    [CEBUS_LOG_ERROR] = {"ERROR", "\033[91m", STDERR_FILENO},
    [CEBUS_LOG_WARNING] = {"WARNING", "\033[33m", STDERR_FILENO},
    [CEBUS_LOG_INFO] = {"INFO", "\033[34m", STDOUT_FILENO},
    [CEBUS_LOG_DEBUG] = {"DEBUG", "\033[1m\033[95m", STDOUT_FILENO},
    [CEBUS_LOG_TRACE] = {"TRACE", "\033[97m", STDOUT_FILENO},
};

#define FMT_RESET "\033[0m"

////////////////////////////////////////////////////////////////////////////

#define _LOG(__log_level, __fmt)                                               \
  _cebus_log(__log_level);                                                     \
  va_list __args;                                                              \
  va_start(__args, __fmt);                                                     \
  vfprintf(log_level_str[__log_level].file == STDERR_FILENO ? stderr : stdout, \
           __fmt, __args);                                                     \
  va_end(__args);                                                              \
  fprintf(log_level_str[__log_level].file == STDERR_FILENO ? stderr : stdout,  \
          "%s\n", display_colors ? FMT_RESET : "");

static bool display_colors = false;
static bool tty_checked = false;

static void _cebus_log(LogLevel log_level) {
  if (UNLIKELY(!tty_checked)) {
    tty_checked = true;
    display_colors = isatty(STDOUT_FILENO) && isatty(STDERR_FILENO);
  }

  fprintf(log_level_str[log_level].file == STDERR_FILENO ? stderr : stdout,
          "%s[%s]: ", display_colors ? log_level_str[log_level].color : "",
          log_level_str[log_level].prefix);
}

////////////////////////////////////////////////////////////////////////////

void cebus_log_level(LogLevel level, const char *fmt, ...) { _LOG(level, fmt); }

void cebus_log(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  printf("\n");
  va_end(args);
}
void cebus_log_fatal(const char *fmt, ...) { _LOG(CEBUS_LOG_FATAL, fmt); }
void cebus_log_error(const char *fmt, ...) { _LOG(CEBUS_LOG_ERROR, fmt); }
void cebus_log_warning(const char *fmt, ...) { _LOG(CEBUS_LOG_WARNING, fmt); }
void cebus_log_info(const char *fmt, ...) { _LOG(CEBUS_LOG_INFO, fmt); }

////////////////////////////////////////////////////////////////////////////

void _cebus_log_debug(const char *fmt, ...) { _LOG(CEBUS_LOG_DEBUG, fmt); }
void _cebus_log_trace(const char *fmt, ...) { _LOG(CEBUS_LOG_TRACE, fmt); }

////////////////////////////////////////////////////////////////////////////

// #include "sorting.h"

#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

void quicksort(const void *src, void *dest, usize size, usize nelem,
               CompareFn compare) {
  if (dest != src) {
    memcpy(dest, src, size * nelem);
  }
  qsort(dest, nelem, size, compare);
}

////////////////////////////////////////////////////////////////////////////

static inline void swap(void *a, void *b, usize size) {
  u8 *pa = a;
  u8 *pb = b;
  for (usize i = 0; i < size; ++i) {
    u8 temp = pa[i];
    pa[i] = pb[i];
    pb[i] = temp;
  }
}

////////////////////////////////////////////////////////////////////////////

static inline usize partition_ctx(u8 *base, usize size, usize low, usize high,
                                  CompareCtxFn compare, const void *ctx) {
  u8 *pivot = &base[high * size];
  usize i = low - 1;
  for (usize j = low; j <= high - 1; j++) {
    if (compare(ctx, &base[j * size], pivot) < 0) {
      i++;
      swap(&base[i * size], &base[j * size], size);
    }
  }
  swap(&base[(i + 1) * size], pivot, size);
  return (i + 1);
}

static void _quicksort_ctx(void *base, size_t size, usize low, usize high,
                           CompareCtxFn compare, const void *ctx) {
  if (low < high) {
    usize pi = partition_ctx(base, size, low, high, compare, ctx);
    _quicksort_ctx(base, size, low, pi ? pi - 1 : 0, compare, ctx);
    _quicksort_ctx(base, size, pi + 1, high, compare, ctx);
  }
}

void quicksort_ctx(const void *src, void *dest, usize size, usize nelem,
                   CompareCtxFn compare, const void *ctx) {
  if (dest != src) {
    memcpy(dest, src, size * nelem);
  }
  _quicksort_ctx(dest, size, 0, nelem - 1, compare, ctx);
}

////////////////////////////////////////////////////////////////////////////

// #include "cmd.h"

// #include "cebus/collection/da.h"
// #include "cebus/core/arena.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cmd_exec_da(Error *error, const Cmd *cmd) {
  cmd_exec(error, cmd->len, cmd->items);
}

////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)

#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

void cmd_exec(Error *error, size_t argc, Str *argv) {
  errno = 0;
  pid_t pid = fork();
  if (pid == -1) {
    error_emit(error, CMD_FORK, "fork failed: %s", strerror(errno));
  } else if (pid == 0) {
    Arena arena = {0};

    DA(char *) args = {0};
    da_init(&args, &arena);
    for (size_t i = 0; i < argc; i++) {
      char *cmd = arena_calloc(&arena, argv[i].len + 1);
      strncpy(cmd, argv[i].data, argv[i].len);
      da_push(&args, cmd);
    }
    da_push(&args, NULL);
    execvp(args.items[0], args.items);

    arena_free(&arena);
    exit(CMD_NOT_FOUND);
  }

  int status = 0;
  waitpid(pid, &status, 0);
  if (WIFEXITED(status)) {
    int exit_code = WEXITSTATUS(status);
    if (exit_code == CMD_NOT_FOUND) {
      error_emit(error, CMD_NOT_FOUND, "command not found: " STR_FMT,
                 STR_ARG(argv[0]));
    } else if (exit_code != 0) {
      error_emit(error, exit_code,
                 "command failed: " STR_FMT ": exit code: %d\n",
                 STR_ARG(argv[0]), exit_code);
    }
  }
}

////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

// #include "cebus/type/string.h"

#include <windows.h>

void cmd_exec(Error *error, size_t argc, Str *argv) {
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  Arena arena = {0};

  Str command = str_wrap(argv[0], STR("\""), &arena);
  Str args = str_join(STR(" "), argc - 1, argv + 1, &arena);
  char *cmd = arena_calloc(&arena, command.len + 1 + args.len + 1);
  strncpy(cmd, command.data, command.len);
  cmd[command.len] = ' ';
  strncpy(cmd + command.len + 1, args.data, args.len);

  if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
    DWORD ec = GetLastError();
    if (ec == 0x2) {
      error_emit(error, CMD_NOT_FOUND, "command not found: " STR_FMT,
                 STR_ARG(argv[0]));
    } else {
      error_emit(error, (i32)ec, "command creation failed: " STR_FMT ": %lu",
                 STR_ARG(argv[0]), ec);
    }
    goto defer;
  }
  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exit_code = 0;
  if (!GetExitCodeProcess(pi.hProcess, &exit_code)) {
    DWORD ec = GetLastError();
    error_emit(error, (i32)ec,
               "Could not get exit code of process: " STR_FMT ": %lu",
               STR_ARG(argv[0]), ec);
    goto defer;
  }
  if (exit_code != 0) {
    error_emit(error, (i32)exit_code, "command failed: " STR_FMT ": %lu",
               STR_ARG(command), exit_code);
    goto defer;
  }

defer:
  arena_free(&arena);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

////////////////////////////////////////////////////////////////////////////
#endif

// #include "dll.h"

// #include "cebus/core/platform.h"
// #include "cebus/os/fs.h"
// #include "cebus/type/integer.h"
// #include "cebus/type/string.h"

#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)
#include <dlfcn.h>

Dll *dll_load(Str path, Error *error) {
  if (!file_exists(path)) {
    error_emit(error, -1, "dll: library does not exist: " STR_FMT,
               STR_ARG(path));
    return NULL;
  }
  char lib_path[FILENAME_MAX] = {0};
  memcpy(lib_path, path.data, usize_min(path.len, FILENAME_MAX));

  Dll *handle = dlopen(lib_path, RTLD_LAZY);
  if (handle == NULL) {
    error_emit(error, -1, "dll: %s: %s\n", lib_path, dlerror());
    return NULL;
  }
  return handle;
}

void dll_close(Dll *handle) { dlclose(handle); }

Function dll_symbol(Dll *handle, Str symbol, Error *error) {
  Arena arena = {0};
  symbol = str_copy(symbol, &arena);

  Function fn;
  *(void **)(&fn) = dlsym(handle, symbol.data);
  if (fn == NULL) {
    error_emit(error, -1, "dll function: %s: %s\n", symbol.data, dlerror());
    goto defer;
  }

defer:
  arena_free(&arena);
  return fn;
}

//////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include <windows.h>

Dll *dll_load(Str path, Error *error) {
  if (!file_exists(path)) {
    error_emit(error, -1, "dll: library does not exist: " STR_FMT,
               STR_ARG(path));
    return NULL;
  }
  char lib_path[FILENAME_MAX] = {0};
  memcpy(lib_path, path.data, usize_min(path.len, FILENAME_MAX));

  char temp_path[MAX_PATH];
  GetTempPath(MAX_PATH, temp_path);

  char temp_file_name[MAX_PATH];
  GetTempFileName(temp_path, TEXT("lib"), 0, temp_file_name);
  CopyFile(lib_path, temp_file_name, 0);

  HINSTANCE handle = LoadLibraryA(temp_file_name);
  if (handle == NULL) {
    DWORD ec = GetLastError();
    error_emit(error, (i32)ec, "error loading library: " STR_FMT ": %lu\n",
               STR_ARG(path), GetLastError());
    return NULL;
  }
  return handle;
}

void dll_close(Dll *handle) {
  char temp_file_name[MAX_PATH];
  GetModuleFileName(handle, temp_file_name, MAX_PATH);
  FreeLibrary(handle);
  DeleteFile(temp_file_name);
}

Function dll_symbol(Dll *handle, Str symbol, Error *error) {
  Arena arena = {0};
  symbol = str_copy(symbol, &arena);
  Function fn = (Function)GetProcAddress(handle, symbol.data);
  if (fn == NULL) {
    DWORD err_code = GetLastError();
    error_emit(error, (i32)err_code, "dll function: %s: %lu\n", symbol.data,
               err_code);
    goto defer;
  }

defer:
  arena_free(&arena);
  return fn;
}

//////////////////////////////////////////////////////////////////////////////
#endif

// #include "fs.h"

// #include "cebus/core/error.h"
// #include "cebus/type/string.h"
// #include "cebus/type/utf8.h"
// #include "io.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

static usize file_size(FILE *handle, Error *error) {
  fseek(handle, 0, SEEK_END);
  const long size = ftell(handle);
  fseek(handle, 0, SEEK_SET);
  if (size < 0) {
    error_emit(error, errno, "Could not get file size: %s", strerror(errno));
    return 0;
  }
  return (usize)size;
}

////////////////////////////////////////////////////////////////////////////

FILE *file_open(Str filename, const char *mode, Error *error) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);
  errno = 0;
  FILE *handle = fopen(_filename, mode);
  if (handle == NULL) {
    error_emit(error, errno, "Could not open file: '%s': %s", _filename,
               strerror(errno));
  }
  return handle;
}

void file_close(FILE *file, Error *error) {
  if (file == NULL) {
    error_emit(error, FILE_INVALID, "can't close a FILE* that is NULL");
    return;
  }
  errno = 0;
  int ret = fclose(file);
  if (ret == EOF) {
    error_emit(error, errno, "closing file failed: %s", strerror(errno));
  }
}

Bytes file_read_bytes(Str filename, Arena *arena, Error *error) {
  Bytes result = {0};
  FILE *handle = file_open(filename, "r", error);
  error_propagate(error, { goto defer; });
  usize size = file_size(handle, error);
  error_propagate(error, { goto defer; });

  u8 *buffer = arena_alloc(arena, size);
  result = io_read_bytes(handle, size, buffer, error);
  error_propagate(error, { goto defer; });

defer:
  if (handle) {
    fclose(handle);
  }
  return result;
}

Str file_read_str(Str filename, Arena *arena, Error *error) {
  Bytes bytes = file_read_bytes(filename, arena, error);
  error_propagate(error, { return (Str){0}; });
  return str_from_bytes(bytes);
}

Utf8 file_read_utf8(Str filename, Arena *arena, Error *error) {
  Utf8 res = {0};
  Bytes bytes = file_read_bytes(filename, arena, error);
  error_propagate(error, { return (Utf8){0}; });
  res = utf8_decode(bytes, error);
  error_propagate(error, { return (Utf8){0}; });
  return res;
}

void file_write_bytes(Str filename, Bytes bytes, Error *error) {
  FILE *handle = file_open(filename, "w", error);
  error_propagate(error, { goto defer; });

  io_write_bytes(handle, bytes, error);
  error_propagate(error, { goto defer; });

defer:
  if (handle) {
    fclose(handle);
  }
}

void file_write_str(Str filename, Str content, Error *error) {
  file_write_bytes(filename, str_to_bytes(content), error);
}

void file_write_utf8(Str filename, Utf8 content, Error *error) {
  Bytes bytes = utf8_encode(content, error);
  error_propagate(error, { return; });
  file_write_bytes(filename, bytes, error);
}

void file_rename(Str old_name, Str new_name, Error *error) {
  char _old_name[FILENAME_MAX] = {0};
  memcpy(_old_name, old_name.data, old_name.len);

  char _new_name[FILENAME_MAX] = {0};
  memcpy(_new_name, new_name.data, new_name.len);

  errno = 0;
  int ret = rename(_old_name, _new_name);
  if (ret == -1) {
    error_emit(error, errno, "Could not rename the file: " STR_FMT ": %s",
               STR_ARG(old_name), strerror(errno));
  }
}

void file_remove(Str filename, Error *error) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);

  errno = 0;
  int ret = remove(_filename);
  if (ret == -1) {
    error_emit(error, errno, "Could not remove the file: " STR_FMT ": %s",
               STR_ARG(filename), strerror(errno));
  }
}

////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)

#include <unistd.h>

bool file_exists(Str filename) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);
  return access(_filename, 0) == 0;
}

//////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include <io.h>

bool file_exists(Str filename) {
  char _filename[FILENAME_MAX] = {0};
  memcpy(_filename, filename.data, filename.len);
  return _access(_filename, 0) == 0;
}

//////////////////////////////////////////////////////////////////////////////
#endif

// #include "io.h"

// #include "cebus/type/byte.h"
// #include "cebus/type/string.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

#define IO_MAX_INPUT 512

////////////////////////////////////////////////////////////////////////////

usize io_write_fmt(FILE *file, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  i32 size = vfprintf(file, fmt, va);
  va_end(va);
  return (usize)size;
}

void io_write_bytes(FILE *file, Bytes bytes, Error *error) {
  errno = 0;
  fwrite(bytes.data, sizeof(bytes.data[0]), bytes.size, file);
  if (ferror(file)) {
    error_emit(error, errno, "Could not write file: %s", strerror(errno));
  }
}

void io_write_str(FILE *file, Str string, Error *error) {
  io_write_bytes(file, str_to_bytes(string), error);
}

Bytes io_read_bytes(FILE *file, usize size, void *buffer, Error *error) {
  errno = 0;
  const usize bytes_read = fread(buffer, sizeof(u8), size, file);
  if (ferror(file)) {
    error_emit(error, errno, "Could not read file: %s", strerror(errno));
    return (Bytes){0};
  }
  return bytes_from_parts(bytes_read, buffer);
}

Str io_read_line(FILE *file, usize size, char *buffer, Error *error) {
  clearerr(file);
  if (fgets(buffer, (int)size, file) == NULL) {
    if (ferror(file)) {
      error_emit(error, errno, "Could not read line: %s", strerror(errno));
      return (Str){0};
    }
    error_emit(error, EOF, "EOF");
    return (Str){0};
  }
  return str_from_cstr(buffer);
}

////////////////////////////////////////////////////////////////////////////

Str input(Str prefix) {
  static char buffer[IO_MAX_INPUT] = {0};
  printf(STR_FMT, STR_ARG(prefix));
  fflush(stdout);
  Str res = io_read_line(stdin, IO_MAX_INPUT, buffer, NULL);
  return str_substring(res, 0, res.len - 1);
}

////////////////////////////////////////////////////////////////////////////

// #include "os.h"

// #include "cebus/core/debug.h"
// #include "cebus/type/integer.h"
// #include "cebus/type/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

Str os_getenv(const char *env, Error *error) {
  const char *var = getenv(env);
  if (var == NULL) {
    error_emit(error, 0, "Could not get environment variable: %s", env);
    return (Str){0};
  }
  return str_from_cstr(var);
}

////////////////////////////////////////////////////////////////////////////
#if defined(LINUX)

#include <errno.h>
#include <string.h>
#include <unistd.h>

void os_chdir(Str path) {
  char pathname[FILENAME_MAX] = {0};
  memcpy(pathname, path.data, usize_min(path.len, FILENAME_MAX));
  cebus_assert(chdir(pathname) == -1,
               "Could not change directory to '" STR_FMT "': %s", STR_ARG(path),
               strerror(errno));
}

Str os_getcwd(Arena *arena) {
  char *buf = arena_alloc(arena, FILENAME_MAX);
  char *ret = getcwd(buf, FILENAME_MAX);
  cebus_assert(ret, "Could not get cwd");
  return str_from_cstr(ret);
}

//////////////////////////////////////////////////////////////////////////////
#elif defined(WINDOWS)

#include <direct.h>
#include <windows.h>

void os_chdir(Str path) {
  char buffer[FILENAME_MAX] = {0};
  memcpy(buffer, path.data, usize_min(path.len, FILENAME_MAX));
  _chdir(buffer);
}

Str os_getcwd(Arena *arena) {
  DWORD size = GetCurrentDirectory(0, NULL);
  char *buf = arena_alloc(arena, size);
  GetCurrentDirectory((DWORD)size, buf);
  // TODO error handling
  return str_from_parts(size, buf);
}

//////////////////////////////////////////////////////////////////////////////

#endif

// #include "byte.h"

// #include "cebus/type/char.h"
// #include "cebus/type/integer.h"
// #include "cebus/type/string.h"

#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////

Bytes bytes_from_parts(usize size, const void *data) {
  return (Bytes){.size = size, .data = data};
}

///////////////////////////////////////////////////////////////////////////////

Bytes bytes_copy(Bytes bytes, Arena *arena) {
  u8 *buffer = arena_alloc(arena, bytes.size);
  memcpy(buffer, bytes.data, bytes.size);
  return bytes_from_parts(bytes.size, buffer);
}

///////////////////////////////////////////////////////////////////////////////

Bytes bytes_slice(Bytes bytes, usize idx1, usize idx2) {
  if (idx2 <= idx1 || bytes.size <= idx1 || bytes.size < idx2) {
    return bytes_from_parts(0, bytes.data);
  }
  return bytes_from_parts(idx2 - idx1, &bytes.data[idx1]);
}

Bytes bytes_take(Bytes *bytes, usize count) {
  count = usize_min(bytes->size, count);
  Bytes ret = bytes_from_parts(count, bytes->data);
  bytes->size -= count;
  bytes->data += count;
  return ret;
}

///////////////////////////////////////////////////////////////////////////////

bool bytes_eq(Bytes b1, Bytes b2) {
  if (b1.size != b2.size) {
    return false;
  }
  return memcmp(b1.data, b2.data, b1.size) == 0;
}

u64 bytes_hash(Bytes bytes) {
  const u64 offset = 2166136261UL;
  const u64 prime = 16777619;
  u64 hash = offset;
  for (usize i = 0; i < bytes.size; i++) {
    hash ^= (unsigned long)bytes.data[i];
    hash *= prime;
  }
  return hash;
}

///////////////////////////////////////////////////////////////////////////////

Str bytes_to_hex(Bytes bytes, Arena *arena) {
  char *buf = arena_calloc(arena, bytes.size * 2 + 1);
  usize idx = 0;
  for (usize i = 0; i < bytes.size; i++) {
    idx += (usize)snprintf(&buf[idx], 3, "%0*x", (i != 0) + 1, bytes.data[i]);
  }
  return (Str){.len = idx, .data = buf};
}

Bytes bytes_from_hex(Str s, Arena *arena) {
  if (str_startswith(s, STR("0x"))) {
    s = str_substring(s, 2, s.len);
  }

  u8 *buffer = arena_calloc(arena, (s.len / 2) + (s.len % 2));
  // to convert strings like "0x101".
  // in the first iteration:
  // take 1 or 2 chars depending if s.len is even or odd
  usize idx = 0;
  for (Str ch = {0}; str_try_take(&s, idx == 0 ? 2 - s.len % 2 : 2, &ch);) {
    for (usize i = 0; i < ch.len; i++) {
      buffer[idx] <<= 4;
      if (c_is_xdigit(ch.data[i])) {
        buffer[idx] |= c_hex_to_u8(ch.data[i]);
      }
    }
    idx++;
  }
  return bytes_from_parts(idx, buffer);
}

///////////////////////////////////////////////////////////////////////////////

// #include "char.h"
// #include "cebus/core/debug.h"

#include <ctype.h>

#define DBASE 10
#define XBASE 16

///////////////////////////////////////////////////////////////////////////////

bool c_is_alnum(char c) { return isalnum(c); }
bool c_is_alpha(char c) { return isalpha(c); }
bool c_is_lower(char c) { return islower(c); }
bool c_is_upper(char c) { return isupper(c); }
bool c_is_space(char c) { return isspace(c); }
bool c_is_cntrl(char c) { return iscntrl(c); }
bool c_is_print(char c) { return isprint(c); }
bool c_is_graph(char c) { return isgraph(c); }
bool c_is_blank(char c) { return isblank(c); }
bool c_is_punct(char c) { return ispunct(c); }
bool c_is_digit(char c) { return isdigit(c); }
bool c_is_xdigit(char c) { return isxdigit(c); }

///////////////////////////////////////////////////////////////////////////////

char c_to_lower(char c) { return (char)tolower(c); }
char c_to_upper(char c) { return (char)toupper(c); }

u8 c_to_u8(char c) {
  cebus_assert_debug(c_is_digit(c), "not convertible: '%c'", c);
  return (u8)c - '0';
}

u8 c_hex_to_u8(char c) {
  cebus_assert_debug(c_is_xdigit(c), "not convertible: '%c'", c);
  if ('0' <= c && c <= '9') {
    return c_to_u8(c);
  }
  if ('a' <= c && c <= 'f') {
    return DBASE + (u8)c - 'a';
  }
  if ('A' <= c && c <= 'F') {
    return DBASE + (u8)c - 'A';
  }
  return 0;
}

char c_u8_to_c(u8 d) {
  cebus_assert_debug(d < DBASE, "not convertible: '%d'", d);
  return '0' + (i8)d;
}

char c_u8_to_hex(u8 d) {
  cebus_assert_debug(d < XBASE, "not convertible: '%d'", d);
  if (d < DBASE) {
    return c_u8_to_c(d);
  }
  if (d < XBASE) {
    return 'a' + ((i8)d - DBASE);
  }
  return 0;
}

char c_u8_to_HEX(u8 d) {
  cebus_assert_debug(d < XBASE, "not convertible: '%d'", d);
  if (d < DBASE) {
    return c_u8_to_c(d);
  }
  if (d < XBASE) {
    return 'A' + ((i8)d - DBASE);
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

// #include "float.h" // IWYU pragma: keep

#define FLOAT_IMPL(T, BITS)                                                    \
  bool T##_eq(T a, T b) { return T##_abs(a - b) < F##BITS##_EPSILON; }         \
  bool T##_eq_eps(T a, T b, T epsilon) { return T##_abs(a - b) < epsilon; }    \
  bool T##_isnan(T value) { return value != value; }                           \
  T T##_abs(T value) {                                                         \
    u##BITS *i = (u##BITS *)&value;                                            \
    *i &= ~((u##BITS)0x8 << (BITS - 1 - 3));                                   \
    return value;                                                              \
  }                                                                            \
  T T##_min(T a, T b) { return a < b ? a : b; }                                \
  T T##_max(T a, T b) { return a > b ? a : b; }                                \
  T T##_clamp(T min, T max, T value) {                                         \
    return value < min ? min : value > max ? max : value;                      \
  }                                                                            \
  T T##_lerp(T min, T max, T value) { return min + value * (max - min); }      \
  T T##_rad(T deg) { return deg * ((T)F64_PI / 180.0f); }                      \
  T T##_deg(T rad) { return rad * ((T)F64_PI / 180.0f); }

FLOAT_IMPL(f32, 32)
FLOAT_IMPL(f64, 64)

// #include "integer.h" // IWYU pragma: keep

// #include "cebus/core/debug.h"
// #include "cebus/core/platform.h"
// #include "cebus/type/byte.h"

#define INTEGER_IMPL(T, BITS)                                                  \
  /* BIT OPERATIONS */                                                         \
  T T##_reverse_bits(T value) {                                                \
    T reversed = 0;                                                            \
    for (usize i = 0; i < BITS; i++) {                                         \
      reversed = (T)(reversed << 1);                                           \
      if (value & 1) {                                                         \
        reversed = reversed | 1;                                               \
      }                                                                        \
      value = value >> 1;                                                      \
    }                                                                          \
    return reversed;                                                           \
  }                                                                            \
                                                                               \
  usize T##_leading_ones(T value) {                                            \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (!(value >> (BITS - i - 1) & (T)0x1)) {                               \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_trailing_ones(T value) {                                           \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (!(value >> i & (T)0x1)) {                                            \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_leading_zeros(T value) {                                           \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (value >> (BITS - i - 1) & (T)0x1) {                                  \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_trailing_zeros(T value) {                                          \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (value >> i & (T)0x1) {                                               \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_count_zeros(T value) {                                             \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (!(value >> i & (T)0x1)) {                                            \
        count++;                                                               \
      }                                                                        \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
                                                                               \
  usize T##_count_ones(T value) {                                              \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (value >> i & (T)0x1) {                                               \
        count++;                                                               \
      }                                                                        \
    }                                                                          \
    return count;                                                              \
  }                                                                            \
  /* BIT OPERATIONS END */                                                     \
                                                                               \
  /* BYTE ORDER OPERATIONS */                                                  \
  T T##_swap_bytes(T value) {                                                  \
    if (1 < sizeof(T)) {                                                       \
      u8 *bytes = (u8 *)&value;                                                \
      for (usize i = 0; i < (sizeof(T) + 1) / 2; i++) {                        \
        u8 temp = bytes[i];                                                    \
        bytes[i] = bytes[sizeof(T) - i - 1];                                   \
        bytes[sizeof(T) - i - 1] = temp;                                       \
      }                                                                        \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  /*  BIG ENDIAN OPERATIONS */                                                 \
  T T##_to_be(T value) {                                                       \
    if (CEBUS_BYTE_ORDER == ENDIAN_LITTLE) {                                   \
      return T##_swap_bytes(value);                                            \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  T T##_from_be(T value) {                                                     \
    if (CEBUS_BYTE_ORDER == ENDIAN_LITTLE) {                                   \
      return T##_swap_bytes(value);                                            \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  T T##_from_be_bytes(Bytes bytes) {                                           \
    cebus_assert(sizeof(T) == bytes.size,                                      \
                 "expected %" USIZE_FMT " bytes but got %" USIZE_FMT,          \
                 sizeof(T), bytes.size);                                       \
    if (CEBUS_BYTE_ORDER == ENDIAN_LITTLE) {                                   \
      return T##_swap_bytes(*(const T *)bytes.data);                           \
    }                                                                          \
    return *(const T *)bytes.data;                                             \
  }                                                                            \
                                                                               \
  Bytes T##_to_be_bytes(T value, Arena *arena) {                               \
    u8 *buffer = arena_alloc(arena, sizeof(value));                            \
    u8 *bytes = (u8 *)&value;                                                  \
    for (usize i = 0; i < sizeof(value); i++) {                                \
      if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                    \
        buffer[i] = bytes[i];                                                  \
      } else {                                                                 \
        buffer[sizeof(value) - i - 1] = bytes[i];                              \
      }                                                                        \
    }                                                                          \
    return bytes_from_parts(sizeof(value), buffer);                            \
  }                                                                            \
                                                                               \
  /*  LITTLE ENDIAN OPERATIONS */                                              \
  T T##_to_le(T value) {                                                       \
    if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                      \
      return T##_swap_bytes(value);                                            \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  T T##_from_le(T value) {                                                     \
    if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                      \
      return T##_swap_bytes(value);                                            \
    }                                                                          \
    return value;                                                              \
  }                                                                            \
                                                                               \
  T T##_from_le_bytes(Bytes bytes) {                                           \
    cebus_assert(sizeof(T) == bytes.size,                                      \
                 "expected %" USIZE_FMT " bytes but got %" USIZE_FMT,          \
                 sizeof(T), bytes.size);                                       \
    if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                      \
      return T##_swap_bytes(*(const T *)bytes.data);                           \
    }                                                                          \
    return *(const T *)bytes.data;                                             \
  }                                                                            \
                                                                               \
  Bytes T##_to_le_bytes(T value, Arena *arena) {                               \
    u8 *buffer = arena_alloc(arena, sizeof(value));                            \
    u8 *bytes = (u8 *)&value;                                                  \
    for (usize i = 0; i < sizeof(value); i++) {                                \
      if (CEBUS_BYTE_ORDER == ENDIAN_LITTLE) {                                 \
        buffer[i] = bytes[i];                                                  \
      } else {                                                                 \
        buffer[sizeof(value) - i - 1] = bytes[i];                              \
      }                                                                        \
    }                                                                          \
    return bytes_from_parts(sizeof(value), buffer);                            \
  }                                                                            \
                                                                               \
  /*  NATIVE ENDIAN OPERATIONS */                                              \
  T T##_from_ne_bytes(Bytes bytes) {                                           \
    cebus_assert(sizeof(T) == bytes.size,                                      \
                 "expected %" USIZE_FMT " bytes but got %" USIZE_FMT,          \
                 sizeof(T), bytes.size);                                       \
    return *(const T *)bytes.data;                                             \
  }                                                                            \
                                                                               \
  Bytes T##_to_ne_bytes(T value, Arena *arena) {                               \
    if (CEBUS_BYTE_ORDER == ENDIAN_BIG) {                                      \
      return T##_to_be_bytes(value, arena);                                    \
    }                                                                          \
    return T##_to_le_bytes(value, arena);                                      \
  }                                                                            \
  /* BYTE ORDER OPERATIONS END */                                              \
                                                                               \
  /* MATH OPERATIONS */                                                        \
  T T##_max(T a, T b) { return a < b ? b : a; }                                \
  T T##_min(T a, T b) { return a > b ? b : a; }                                \
  T T##_clamp(T min, T max, T value) {                                         \
    return value < min ? min : max < value ? max : value;                      \
  }                                                                            \
  /* MATH OPERATIONS END */                                                    \
                                                                               \
  /* UTILS */                                                                  \
  u64 T##_hash(T value) {                                                      \
    u64 hash = ((u64)value) + 1;                                               \
    hash = (((hash >> 16) ^ hash) % 0x3AA387A8B1) * 0x45d9f3b;                 \
    hash = (((hash >> 16) ^ hash) % 0x3AA387A8B1) * 0x45d9f3b;                 \
    hash = (hash >> 16) ^ hash;                                                \
    return hash;                                                               \
  }                                                                            \
                                                                               \
  void T##_swap(T *v1, T *v2) {                                                \
    T temp = *v1;                                                              \
    *v1 = *v2;                                                                 \
    *v2 = temp;                                                                \
  }                                                                            \
                                                                               \
  CmpOrdering T##_compare_lt(T a, T b) {                                       \
    return a == b ? CMP_EQUAL : a < b ? CMP_LESS : CMP_GREATER;                \
  }                                                                            \
                                                                               \
  CmpOrdering T##_compare_gt(T a, T b) {                                       \
    return a == b ? CMP_EQUAL : a > b ? CMP_LESS : CMP_GREATER;                \
  }                                                                            \
                                                                               \
  static CmpOrdering _##T##_cmp_gt(const void *a, const void *b) {             \
    return T##_compare_gt(*(const T *)a, *(const T *)b);                       \
  }                                                                            \
                                                                               \
  static CmpOrdering _##T##_cmp_lt(const void *a, const void *b) {             \
    return T##_compare_lt(*(const T *)a, *(const T *)b);                       \
  }                                                                            \
                                                                               \
  CompareFn T##_compare_qsort(CmpOrdering ordering) {                          \
    return ordering == CMP_LESS ? _##T##_cmp_lt : _##T##_cmp_gt;               \
  } /* UTILS END */

INTEGER_IMPL(u8, U8_BITS)
INTEGER_IMPL(i8, I8_BITS)
INTEGER_IMPL(u16, U16_BITS)
INTEGER_IMPL(i16, I16_BITS)
INTEGER_IMPL(u32, U32_BITS)
INTEGER_IMPL(i32, I32_BITS)
INTEGER_IMPL(u64, U64_BITS)
INTEGER_IMPL(i64, I64_BITS)
INTEGER_IMPL(usize, USIZE_BITS)

// #include "string.h"

// #include "cebus/core/arena.h"
// #include "cebus/type/byte.h"
// #include "cebus/type/char.h"
// #include "cebus/type/integer.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////

Str str_from_parts(usize size, const char *cstr) {
  return (Str){.len = size, .data = cstr};
}

Str str_from_bytes(Bytes bytes) {
  return str_from_parts(bytes.size, (const char *)bytes.data);
}

Bytes str_to_bytes(Str s) { return bytes_from_parts(s.len, s.data); }

Str str_from_cstr(const char *cstr) {
  return (Str){.len = strlen(cstr), .data = cstr};
}

Str str_format(Arena *arena, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  usize size = (usize)vsnprintf(NULL, 0, fmt, va) + 1;
  va_end(va);

  char *buffer = arena_calloc(arena, size);
  va_start(va, fmt);
  vsnprintf(buffer, size, fmt, va);
  va_end(va);
  return str_from_parts(size - 1, buffer);
}

///////////////////////////////////////////////////////////////////////////////

Str str_copy(Str s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.len + 1);
  memcpy(buffer, s.data, s.len);
  buffer[s.len] = '\0';
  return str_from_parts(s.len, buffer);
}

Str str_append(Str s, Str suffix, Arena *arena) {
  const usize new_size = s.len + suffix.len;
  char *buffer = arena_alloc(arena, new_size + 1);
  memcpy(&buffer[0], s.data, s.len);
  memcpy(&buffer[s.len], suffix.data, suffix.len);
  buffer[new_size] = '\0';
  return str_from_parts(new_size, buffer);
}

Str str_prepend(Str s, Str prefix, Arena *arena) {
  const usize new_size = s.len + prefix.len;
  char *buffer = arena_alloc(arena, new_size + 1);
  memcpy(&buffer[0], prefix.data, prefix.len);
  memcpy(&buffer[prefix.len], s.data, s.len);
  buffer[new_size] = '\0';
  return str_from_parts(new_size, buffer);
}

Str str_wrap(Str s, Str wrap, Arena *arena) {
  const usize new_size = s.len + wrap.len * 2;
  char *buffer = arena_alloc(arena, new_size + 1);
  memcpy(&buffer[0], wrap.data, wrap.len);
  memcpy(&buffer[wrap.len], s.data, s.len);
  memcpy(&buffer[wrap.len + s.len], wrap.data, wrap.len);
  buffer[new_size] = '\0';
  return str_from_parts(new_size, buffer);
}

Str str_join(Str sep, usize count, Str *s, Arena *arena) {
  usize size = sep.len * (count - 1);
  for (usize i = 0; i < count; i++) {
    size += s[i].len;
  }
  char *buffer = arena_alloc(arena, size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    if (i != 0) {
      memcpy(&buffer[b_idx], sep.data, sep.len);
      b_idx += sep.len;
    }
    memcpy(&buffer[b_idx], s[i].data, s[i].len);
    b_idx += s[i].len;
  }
  buffer[size] = '\0';

  return str_from_parts(size, buffer);
}

Str str_join_suffix(Str suffix, usize count, Str *s, Arena *arena) {
  usize size = suffix.len * count;
  for (usize i = 0; i < count; i++) {
    size += s[i].len;
  }
  char *buffer = arena_alloc(arena, size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    memcpy(&buffer[b_idx], s[i].data, s[i].len);
    b_idx += s[i].len;
    memcpy(&buffer[b_idx], suffix.data, suffix.len);
    b_idx += suffix.len;
  }
  buffer[size] = '\0';

  return str_from_parts(size, buffer);
}

Str str_join_prefix(Str prefix, usize count, Str *s, Arena *arena) {
  usize size = prefix.len * count;
  for (usize i = 0; i < count; i++) {
    size += s[i].len;
  }
  char *buffer = arena_alloc(arena, size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    memcpy(&buffer[b_idx], prefix.data, prefix.len);
    b_idx += prefix.len;
    memcpy(&buffer[b_idx], s[i].data, s[i].len);
    b_idx += s[i].len;
  }
  buffer[size] = '\0';

  return str_from_parts(size, buffer);
}

Str str_join_wrap(Str sep, Str wrap, usize count, Str *s, Arena *arena) {
  usize size = sep.len * (count - 1) + wrap.len * count * 2;
  for (usize i = 0; i < count; i++) {
    size += s[i].len;
  }
  char *buffer = arena_alloc(arena, size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    if (i != 0) {
      memcpy(&buffer[b_idx], sep.data, sep.len);
      b_idx += sep.len;
    }
    memcpy(&buffer[b_idx], wrap.data, wrap.len);
    b_idx += wrap.len;
    memcpy(&buffer[b_idx], s[i].data, s[i].len);
    b_idx += s[i].len;
    memcpy(&buffer[b_idx], wrap.data, wrap.len);
    b_idx += wrap.len;
  }
  buffer[size] = '\0';

  return str_from_parts(size, buffer);
}

Str str_upper(Str s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.len + 1);
  buffer[s.len] = '\0';
  for (usize i = 0; i < s.len; i++) {
    buffer[i] = c_to_upper(s.data[i]);
  }
  return str_from_parts(s.len, buffer);
}

Str str_lower(Str s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.len + 1);
  buffer[s.len] = '\0';
  for (usize i = 0; i < s.len; i++) {
    buffer[i] = c_to_lower(s.data[i]);
  }
  return str_from_parts(s.len, buffer);
}

Str str_replace(Str s, Str old, Str new, Arena *arena) {
  usize count = str_count(s, old);
  usize new_size = (s.len - (old.len * count) + (count * new.len));
  char *buffer = arena_alloc(arena, new_size + 1);
  buffer[new_size] = '\0';

  for (usize i = 0, j = 0; i < s.len;) {
    if (old.len <= s.len - i && memcmp(&s.data[i], old.data, old.len) == 0) {
      memcpy(&buffer[j], new.data, new.len);
      i += old.len;
      j += new.len;
    } else {
      buffer[j++] = s.data[i++];
    }
  }

  return str_from_parts(new_size, buffer);
}

Str str_center(Str s, usize width, char fillchar, Arena *arena) {
  if (width < s.len) {
    return str_copy(s, arena);
  }
  char *buffer = arena_calloc(arena, width + 1);
  const usize left_width = (width - s.len) / 2;
  const usize right_width = (width - s.len - left_width);
  usize idx = 0;
  for (usize i = 0; i < left_width; i++) {
    buffer[idx++] = fillchar;
  }
  for (usize i = 0; i < s.len; i++) {
    buffer[idx++] = s.data[i];
  }
  for (usize i = 0; i < right_width; i++) {
    buffer[idx++] = fillchar;
  }
  return str_from_parts(width, buffer);
}

Str str_ljust(Str s, usize width, char fillchar, Arena *arena) {
  if (width < s.len) {
    return str_copy(s, arena);
  }
  char *buffer = arena_calloc(arena, width + 1);
  usize idx = 0;
  for (usize i = 0; i < s.len; i++) {
    buffer[idx++] = s.data[i];
  }
  for (usize i = 0; i < width - s.len; i++) {
    buffer[idx++] = fillchar;
  }
  return str_from_parts(width, buffer);
}

Str str_rjust(Str s, usize width, char fillchar, Arena *arena) {
  if (width < s.len) {
    return str_copy(s, arena);
  }
  char *buffer = arena_calloc(arena, width + 1);
  usize idx = 0;
  for (usize i = 0; i < width - s.len; i++) {
    buffer[idx++] = fillchar;
  }
  for (usize i = 0; i < s.len; i++) {
    buffer[idx++] = s.data[i];
  }

  return str_from_parts(width, buffer);
}

Str str_repeat(Str s, usize count, Arena *arena) {
  usize len = s.len * count;
  char *buffer = arena_alloc(arena, len + 1);
  buffer[len] = '\0';

  usize idx = 0;
  for (usize i = 0; i < count; i++) {
    for (usize j = 0; j < s.len; j++) {
      buffer[idx++] = s.data[j];
    }
  }

  return str_from_parts(len, buffer);
}

Str str_reverse(Str s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.len + 1);
  buffer[s.len] = '\0';
  for (usize i = 0; i < s.len; i++) {
    buffer[i] = s.data[s.len - i - 1];
  }
  return str_from_parts(s.len, buffer);
}

///////////////////////////////////////////////////////////////////////////////

bool str_eq(Str s1, Str s2) {
  if (s1.len != s2.len) {
    return false;
  }
  return memcmp(s1.data, s2.data, s1.len) == 0;
}

bool str_eq_ignorecase(Str s1, Str s2) {
  if (s1.len != s2.len) {
    return false;
  }
  for (size_t i = 0; i < s1.len; i++) {
    if (c_to_lower(s1.data[i]) != c_to_lower(s2.data[i])) {
      return false;
    }
  }
  return true;
}

bool str_startswith(Str s1, Str prefix) {
  if (s1.len < prefix.len) {
    return false;
  }
  return memcmp(s1.data, prefix.data, prefix.len) == 0;
}

bool str_endswith(Str s1, Str suffix) {
  if (s1.len < suffix.len) {
    return false;
  }
  const usize idx = s1.len - suffix.len;
  return memcmp(&s1.data[idx], suffix.data, suffix.len) == 0;
}

bool str_contains(Str haystack, Str needle) {
  if (haystack.len < needle.len) {
    return false;
  }
  for (usize i = 0; i < haystack.len - needle.len + 1; i++) {
    if (memcmp(&haystack.data[i], needle.data, needle.len) == 0) {
      return true;
    }
  }
  return false;
}

bool str_includes(Str haystack, char needle) {
  if (haystack.len == 0) {
    return false;
  }
  for (usize i = 0; i < haystack.len; i++) {
    if (haystack.data[i] == needle) {
      return true;
    }
  }
  return false;
}

bool str_empty(Str s) { return s.len == 0; }

CmpOrdering str_compare_gt(Str s1, Str s2) {
  const usize min_bytes = usize_min(s1.len, s2.len);
  const int r = memcmp(s1.data, s2.data, min_bytes);
  return r < 0   ? CMP_LESS    // less
         : 0 < r ? CMP_GREATER // greater
                 : CMP_EQUAL;  // equal
}

CmpOrdering str_compare_lt(Str s1, Str s2) { return str_compare_gt(s2, s1); }

static CmpOrdering _str_cmp_gt(const void *s1, const void *s2) {
  return str_compare_gt(*(const Str *)s1, *(const Str *)s2);
}

static CmpOrdering _str_cmp_lt(const void *s1, const void *s2) {
  return str_compare_gt(*(const Str *)s2, *(const Str *)s1);
}

///////////////////////////////////////////////////////////////////////////////

CompareFn str_compare_qsort(CmpOrdering ordering) {
  return ordering == CMP_LESS ? _str_cmp_lt : _str_cmp_gt;
}

Str str_trim_left(Str s) {
  Str result = s;
  for (usize i = 0; i < s.len && c_is_space(s.data[i]); ++i) {
    result.data++;
    result.len--;
  }
  return result;
}

Str str_trim_left_by_delim(Str s, char delim) {
  Str result = s;
  for (usize i = 0; i < s.len && s.data[i] == delim; ++i) {
    result.data++;
    result.len--;
  }
  return result;
}

Str str_trim_left_by_predicate(Str s, bool (*predicate)(char)) {
  Str result = s;
  for (usize i = 0; i < s.len && predicate(s.data[i]); ++i) {
    result.data++;
    result.len--;
  }
  return result;
}

Str str_trim_right(Str s) {
  Str result = s;
  for (usize i = 0; i < s.len && c_is_space(s.data[s.len - i - 1]); ++i) {
    result.len--;
  }
  return result;
}

Str str_trim_right_by_delim(Str s, char delim) {
  Str result = s;
  for (usize i = 0; i < s.len && s.data[s.len - i - 1] == delim; ++i) {
    result.len--;
  }
  return result;
}

Str str_trim_right_by_predicate(Str s, bool (*predicate)(char)) {
  Str result = s;
  for (usize i = 0; i < s.len && predicate(s.data[s.len - i - 1]); ++i) {
    result.len--;
  }
  return result;
}

Str str_trim(Str s) { return str_trim_left(str_trim_right(s)); }
Str str_trim_by_delim(Str s, char delim) {
  return str_trim_left_by_delim(str_trim_right_by_delim(s, delim), delim);
}
Str str_trim_by_predicate(Str s, bool (*predicate)(char)) {
  return str_trim_left_by_predicate(str_trim_right_by_predicate(s, predicate),
                                    predicate);
}

bool str_try_chop_by_delim(Str *s, char delim, Str *chunk) {
  if (s->len == 0) {
    return false;
  }
  usize i = 0;
  while (i < s->len && s->data[i] != delim) {
    ++i;
  }

  if (s->len && i <= s->len) {
    if (chunk) {
      *chunk = str_from_parts(i, s->data);
    }
    const usize new_len = usize_min(s->len, i + 1);
    s->data += new_len;
    s->len -= new_len;
    *s = str_trim_left_by_delim(*s, delim);
    return true;
  }

  return false;
}

Str str_chop_by_delim(Str *s, char delim) {
  usize i = 0;
  while (i < s->len && s->data[i] != delim) {
    ++i;
  }

  if (s->len && i <= s->len) {
    Str chunk = str_from_parts(i, s->data);
    const usize new_len = usize_min(s->len, i + 1);
    s->data += new_len;
    s->len -= new_len;
    *s = str_trim_left_by_delim(*s, delim);
    return chunk;
  }

  return *s;
}

bool str_try_chop_by_predicate(Str *s, bool (*predicate)(char), Str *chunk) {

  usize i = 0;
  while (i < s->len && !predicate(s->data[i])) {
    ++i;
  }

  if (s->len && i <= s->len) {
    if (chunk) {
      *chunk = str_from_parts(i, s->data);
    }
    const usize new_len = usize_min(s->len, i + 1);
    s->data += new_len;
    s->len -= new_len;
    *s = str_trim_left_by_predicate(*s, predicate);
    return true;
  }
  return false;
}

Str str_chop_by_predicate(Str *s, bool (*predicate)(char)) {
  usize i = 0;
  while (i < s->len && !predicate(s->data[i])) {
    ++i;
  }

  if (s->len && i <= s->len) {
    Str chunk = str_from_parts(i, s->data);
    const usize new_len = usize_min(s->len, i + 1);
    s->data += new_len;
    s->len -= new_len;
    *s = str_trim_left_by_predicate(*s, predicate);
    return chunk;
  }
  return *s;
}

Str str_chop_right_by_delim(Str *s, char delim) {
  usize i = 0;
  while (i < s->len && s->data[s->len - i - 1] != delim) {
    ++i;
  }

  if (s->len && i <= s->len) {
    Str chunk = str_from_parts(i, &s->data[s->len - i]);
    s->len -= usize_min(s->len, i + 1);
    *s = str_trim_right_by_delim(*s, delim);
    return chunk;
  }
  return *s;
}

Str str_chop_right_by_predicate(Str *s, bool (*predicate)(char)) {
  usize i = 0;
  while (i < s->len && !predicate(s->data[s->len - i - 1])) {
    ++i;
  }

  if (s->len && i <= s->len) {
    Str chunk = str_from_parts(i, &s->data[s->len - i]);
    s->len -= usize_min(s->len, i + 1);
    *s = str_trim_right_by_predicate(*s, predicate);
    return chunk;
  }
  return *s;
}

Str str_substring(Str s, usize start, usize end) {
  if (end <= start || s.len <= start || s.len < end) {
    return STR("");
  }
  return str_from_parts(end - start, &s.data[start]);
}

Str str_take(Str *s, usize count) {
  count = usize_min(s->len, count);
  Str ret = str_from_parts(count, s->data);
  s->len -= count;
  s->data += count;
  return ret;
}

bool str_try_take(Str *s, usize count, Str *chunk) {
  if (s->len == 0) {
    return false;
  }
  count = usize_min(s->len, count);
  *chunk = str_from_parts(count, s->data);
  s->len -= count;
  s->data += count;
  return true;
}

///////////////////////////////////////////////////////////////////////////////

u64 str_u64(Str s) {
  const int radix = 10;
  Arena arena = {0};
  Str owned = str_copy(s, &arena);
  u64 value = strtoull(owned.data, NULL, radix);
  arena_free(&arena);
  return value;
}

u64 str_chop_u64(Str *s) {
  const int radix = 10;
  Arena arena = {0};
  Str owned = str_copy(*s, &arena);
  char *endptr;
  u64 value = strtoull(owned.data, &endptr, radix);

  const usize size = (usize)(endptr - owned.data);
  s->data += size;
  s->len -= size;

  arena_free(&arena);
  return value;
}

i64 str_i64(Str s) {
  const int radix = 10;
  Arena arena = {0};
  Str owned = str_copy(s, &arena);
  i64 value = strtoll(owned.data, NULL, radix);
  arena_free(&arena);
  return value;
}

i64 str_chop_i64(Str *s) {
  const int radix = 10;
  Arena arena = {0};
  Str owned = str_copy(*s, &arena);
  char *endptr;
  i64 value = strtoll(owned.data, &endptr, radix);

  const usize size = (usize)(endptr - owned.data);
  s->data += size;
  s->len -= size;

  arena_free(&arena);
  return value;
}

f64 str_f64(Str s) {
  Arena arena = {0};
  Str owned = str_copy(s, &arena);
  double value = strtod(owned.data, NULL);
  arena_free(&arena);
  return value;
}

f64 str_chop_f64(Str *s) {
  Arena arena = {0};
  Str owned = str_copy(*s, &arena);

  char *endptr;
  f64 value = strtod(owned.data, &endptr);

  const usize size = (usize)(endptr - owned.data);
  s->data += size;
  s->len -= size;

  arena_free(&arena);
  return value;
}

usize str_find(Str haystack, Str needle) {
  if (haystack.len < needle.len) {
    return STR_NOT_FOUND;
  }
  for (usize i = 0; i < haystack.len - needle.len + 1; i++) {
    if (memcmp(&haystack.data[i], needle.data, needle.len) == 0) {
      return i;
    }
  }
  return STR_NOT_FOUND;
}

usize str_find_last(Str haystack, Str needle) {
  if (haystack.len < needle.len) {
    return STR_NOT_FOUND;
  }
  for (usize i = haystack.len - needle.len + 1; i > 0; i--) {
    if (memcmp(&haystack.data[i - 1], needle.data, needle.len) == 0) {
      return i - 1;
    }
  }
  return STR_NOT_FOUND;
}

usize str_count(Str haystack, Str needle) {
  usize count = 0;
  if (haystack.len < needle.len) {
    return count;
  }
  for (usize i = 0; i < haystack.len - needle.len + 1; i++) {
    if (memcmp(&haystack.data[i], needle.data, needle.len) == 0) {
      count++;
      i += needle.len;
    }
  }
  return count;
}

char str_getc(Str s, usize idx) {
  if (s.len <= idx) {
    return '\0';
  }
  return s.data[idx];
}

u64 str_hash(Str s) {
  const uint64_t magic_prime = 0x00000100000001b3;
  uint64_t hash = 0xcbf29ce484222325; // NOLINT
  for (usize i = 0; i < s.len; ++i) {
    hash = (hash ^ (u64)s.data[i]) * magic_prime;
  }
  return hash;
}

///////////////////////////////////////////////////////////////////////////////

// #include "utf8.h"

// #include "cebus/core/arena.h"
// #include "cebus/core/debug.h"
// #include "cebus/type/byte.h"
// #include "cebus/type/char.h"
// #include "cebus/type/integer.h"

#include <string.h>

Utf8 utf8_decode(Bytes bytes, Error *error) {
  Utf8 str = {0};
  usize len = 0;
  for (usize i = 0; i < bytes.size; i++) {
    usize bit_count = u8_leading_ones(bytes.data[i]);
    if (!(bit_count <= 4)) {
      error_emit(error, UTF8_DECODE,
                 "Decoding utf-8 failed: too many starting bits at %" USIZE_FMT
                 ": found %" USIZE_FMT,
                 i, bit_count);
      return str;
    }
    usize idx = i;
    for (usize j = 1; j < bit_count; j++) {
      if (!(u8_leading_ones(bytes.data[idx + j]) == 1)) {
        error_emit(
            error, UTF8_DECODE,
            "Decoding utf-8 failed: wrong bits in between at %" USIZE_FMT,
            idx + j);
        return str;
      }
      i++;
    }
    len++;
  }
  str.len = len;
  str.size = bytes.size;
  str.data = (const char *)bytes.data;
  return str;
}

Bytes utf8_encode(Utf8 s, Error *error) {
  bool ret = utf8_validate(s);
  if (ret == false) {
    error_emit(error, UTF8_ENCODE, "Encoding utf-8 failed");
    return (Bytes){0};
  }
  return bytes_from_parts(s.size, s.data);
}

bool utf8_eq(Utf8 s1, Utf8 s2) {
  if (s1.size != s2.size || s1.len != s2.len) {
    return false;
  }
  return memcmp(s1.data, s2.data, s1.size) == 0;
}

bool utf8_starts_with(Utf8 s, Utf8 prefix) {
  if (s.size < prefix.size) {
    return false;
  }
  return memcmp(s.data, prefix.data, prefix.size) == 0;
}

bool utf8_ends_with(Utf8 s, Utf8 prefix) {
  if (s.size < prefix.size) {
    return false;
  }
  return memcmp(&s.data[s.size - prefix.size], prefix.data, prefix.size) == 0;
}

bool utf8_try_next(Utf8 *str, Utf8 *out) {
  usize bit_count = u8_leading_ones((u8)str->data[0]);
  cebus_assert_return(bit_count <= 4, false);
  cebus_assert_return(bit_count != 1, false);
  usize bytes = usize_max(1, bit_count);
  cebus_assert_return(bytes <= str->size, false);
  *out = (Utf8){.len = 1, .size = bytes, .data = str->data};
  str->size -= bytes;
  str->len--;
  str->data += bytes;
  return true;
}

Utf8 utf8_next(Utf8 *str) {
  Utf8 res = {0};
  bool ret = utf8_try_next(str, &res);
  cebus_assert(ret, "Getting next character failed");
  return res;
}

Utf8 utf8_copy(Utf8 str, Arena *arena) {
  char *buffer = arena_alloc(arena, str.size + 1);
  buffer[str.size] = '\0';
  memcpy(buffer, str.data, str.size);
  return (Utf8){.len = str.len, .size = str.size, .data = buffer};
}

Utf8 utf8_append(Utf8 s, Utf8 suffix, Arena *arena) {
  const usize new_size = s.size + suffix.size;
  char *buffer = arena_alloc(arena, new_size + 1);
  buffer[new_size] = '\0';
  memcpy(&buffer[0], s.data, s.size);
  memcpy(&buffer[s.size], suffix.data, suffix.size);
  return (Utf8){.len = s.len + suffix.len, .size = new_size, .data = buffer};
}

Utf8 utf8_prepend(Utf8 s, Utf8 prefix, Arena *arena) {
  const usize new_size = s.size + prefix.size;
  char *buffer = arena_alloc(arena, new_size + 1);
  buffer[new_size] = '\0';
  memcpy(&buffer[0], prefix.data, prefix.size);
  memcpy(&buffer[prefix.size], s.data, s.size);
  return (Utf8){.len = s.len + prefix.len, .size = new_size, .data = buffer};
}

Utf8 utf8_join(Utf8 sep, usize count, Utf8 *s, Arena *arena) {
  usize new_size = (sep.size * (count - 1));
  usize new_len = (sep.len * (count - 1));
  for (usize i = 0; i < count; i++) {
    new_size += s[i].size;
    new_len += s[i].len;
  }
  char *buffer = arena_calloc(arena, new_size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    if (i != 0) {
      memcpy(&buffer[b_idx], sep.data, sep.size);
      b_idx += sep.size;
    }
    memcpy(&buffer[b_idx], s[i].data, s[i].size);
    b_idx += s[i].size;
  }
  return (Utf8){.len = new_len, .size = new_size, .data = buffer};
}

Utf8 utf8_upper(Utf8 s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.size);
  for (usize i = 0; i < s.size; ++i) {
    usize bit_count = u8_leading_ones((u8)s.data[i]);
    if (bit_count == 0) {
      buffer[i] = c_to_upper(s.data[i]);
    } else {
      memcpy(&buffer[i], &s.data[i], bit_count);
      i += bit_count - 1;
    }
  }
  return (Utf8){.len = s.len, .size = s.size, .data = buffer};
}

Utf8 utf8_lower(Utf8 s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.size);
  for (usize i = 0; i < s.size; ++i) {
    usize bit_count = u8_leading_ones((u8)s.data[i]);
    if (bit_count == 0) {
      buffer[i] = c_to_lower(s.data[i]);
    } else {
      memcpy(&buffer[i], &s.data[i], bit_count);
      i += bit_count - 1;
    }
  }
  return (Utf8){.len = s.len, .size = s.size, .data = buffer};
}

bool utf8_validate_bytes(Bytes bytes) {
  for (usize i = 0; i < bytes.size; i++) {
    usize bit_count = u8_leading_ones(bytes.data[i]);
    cebus_assert_return(bit_count <= 4, false);
    usize idx = i;
    for (usize j = 1; j < bit_count; j++) {
      cebus_assert_return(u8_leading_ones(bytes.data[idx + j]) == 1, false);
      i++;
    }
  }
  return true;
}

bool utf8_validate(Utf8 s) {
  return utf8_validate_bytes(bytes_from_parts(s.size, (const u8 *)s.data));
}

#endif // !CEBUS_IMPLEMENTATION
