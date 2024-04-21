// Copyright 2024 Leonhard S.

#pragma once

// Helper macro for disabling warnings generated within Qt MOC sources.
// Usage:
//
// // regular sources here
//
// PUSH_MOC_WARNINGS_FILTER;
// #include "moc_mysourcefile.cpp"
// POP_MOC_WARNINGS_FILTER;
#if defined(_WIN32) && !defined(__clang__)

#  define PUSH_MOC_WARNINGS_FILTER \
__pragma(warning(push)) \
__pragma(warning(disable: 4464)) \
static_assert(true, "")

#  define POP_MOC_WARNINGS_FILTER \
__pragma(warning(pop)) \
static_assert(true, "")

#elif defined(__clang__)

#  define PUSH_MOC_WARNINGS_FILTER \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
_Pragma("clang diagnostic ignored \"-Wunsafe-buffer-usage\"") \
static_assert(true, "")

#  define POP_MOC_WARNINGS_FILTER \
_Pragma("clang diagnostic pop") \
static_assert(true, "")

#elif defined(__GNUC__)

#  define PUSH_NO_MOC_WARNINGS_FILTER \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wreserved-identifier\"") \
static_assert(true, "")

#  define POP_MOC_WARNINGS_FILTER \
_Pragma("GCC diagnostic pop") \
static_assert(true, "")

#endif
