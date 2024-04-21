#pragma once

#include <gsl/pointers>

/**
 * Convert gsl::owner<T*> to T*.
 *
 * This is really a glorified cast used to show that a pointer is owned by the
 * Qt ownership model.
*/
template <typename T>
T* adopt_pointer(gsl::owner<T*> ptr) {
    return ptr;
}
