/*
 * Half Life 1 SDK License
 * Copyright(c) Valve Corp
 *
 * DISCLAIMER OF WARRANTIES. THE HALF LIFE 1 SDK AND ANY OTHER MATERIAL
 * DOWNLOADED BY LICENSEE IS PROVIDED "AS IS". VALVE AND ITS SUPPLIERS
 * DISCLAIM ALL WARRANTIES WITH RESPECT TO THE SDK, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NON-INFRINGEMENT, TITLE AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * LIMITATION OF LIABILITY. IN NO EVENT SHALL VALVE OR ITS SUPPLIERS BE LIABLE
 * FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 * BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
 * LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE ENGINE AND/OR THE
 * SDK, EVEN IF VALVE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * For commercial use, contact: sourceengine@valvesoftware.com
 */

#pragma once

/**
 * @brief Defines macros for different compilers on Windows and Linux.
 *
 * Checks for the presence of various predefined macros to determine
 * the compiler being used and defines a corresponding macro for that compiler.
 *
 * Supported compilers:
 * - Microsoft Visual C++
 * - Intel C++ (Windows and Linux)
 * - Intel LLVM (Windows and Linux)
 * - Clang (Windows and Linux)
 * - GCC
 * - MinGW
 * - Cygwin
 */
#if defined(_MSC_VER)
  #if defined(__INTEL_COMPILER)
    #if defined(__INTEL_LLVM_COMPILER)
      #define INTEL_LLVM_WINDOWS_COMPILER
    #else
      #define INTEL_WINDOWS_COMPILER
    #endif
  #elif defined(__clang__)
    #define CLANG_WINDOWS_COMPILER
  #else
    #define MSVC_COMPILER
  #endif
#elif defined(__INTEL_COMPILER)
  #if defined(__INTEL_LLVM_COMPILER)
    #define INTEL_LLVM_LINUX_COMPILER
  #else
    #define INTEL_LINUX_COMPILER
  #endif
#elif defined(__clang__)
  #define CLANG_LINUX_COMPILER
#elif defined(__GNUC__) || defined(__GNUG__)
  #if defined(__MINGW32__) || defined(__MINGW64__)
    #define MINGW_COMPILER
  #elif defined(__CYGWIN__)
    #define CYGWIN_COMPILER
  #else
    #define GCC_COMPILER
  #endif
#endif

/**
 * @def NO_VTABLE
 * @brief Prevents the generation of a virtual function table for a class.
 */
#ifdef _WIN32
  #define NO_VTABLE __declspec(novtable)
#else
  #define NO_VTABLE
#endif

/**
 * @def NO_INLINE
 * @brief Prevents the compiler from inlining a function.
 */
#ifdef _WIN32
  #define NO_INLINE __declspec(noinline)
#else
  #define NO_INLINE [[gnu::noinline]]
#endif

/**
 * @def FORCE_INLINE
 * @brief Forces the compiler to inline a function.
 */
#ifdef _WIN32
  #define FORCE_INLINE __forceinline
#else
  #define FORCE_INLINE [[gnu::always_inline]]
#endif

/**
 * @def DLL_EXPORT
 * @brief Exports a symbol from a DLL.
 */
#ifdef _WIN32
  #define DLL_EXPORT __declspec(dllexport)
#else
  #define DLL_EXPORT [[gnu::visibility("default")]]
#endif

/**
 * @def FORCE_STACK_ALIGN
 * @brief Aligns the stack pointer to a specific boundary.
 */
#ifdef _WIN32
  #define FORCE_STACK_ALIGN NO_INLINE
#else
  #define FORCE_STACK_ALIGN [[gnu::force_align_arg_pointer, gnu::noinline, gnu::used]]
#endif

/**
 * @def ATTR_HOT
 * @brief Marks a function as a hot spot in the compiled program.
 */
#ifdef _WIN32
  #define ATTR_HOT
#else
  #define ATTR_HOT [[gnu::hot]]
#endif
