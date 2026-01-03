#pragma once

#include <vector>
#include <cstddef>
#include <array>
#include <type_traits>

// Minimal mdspan polyfill for C++23/26 compatibility on older compilers
namespace stdx {

inline constexpr size_t dynamic_extent = -1;

template<class IndexType, size_t... Extents>
struct extents {
    static constexpr size_t rank() { return sizeof...(Extents); }
    static constexpr size_t rank_dynamic() { return ((Extents == dynamic_extent) + ...); }
};

namespace layout {
    struct right {}; // Row-major (C-style)
    struct left {};  // Column-major (Fortran-style)
}

template<class T, class Extents, class Layout = layout::right>
class mdspan {
public:
    using element_type = T;
    using index_type = size_t;
    using pointer = T*;
    using reference = T&;

    // Simplified constructor for dynamic extents (assuming 3D for now based on proposal)
    mdspan(pointer ptr, size_t d0, size_t d1, size_t d2) 
        : data_(ptr), dims_{d0, d1, d2} {}
    
    mdspan(pointer ptr, size_t d0, size_t d1) 
        : data_(ptr), dims_{d0, d1, 1} {} // stored as 3d with depth 1

    reference operator[](size_t i, size_t j, size_t k) const {
        // Assume row-major (layout_right)
        return data_[i * dims_[1] * dims_[2] + j * dims_[2] + k];
    }
    
    reference operator[](size_t i, size_t j) const {
        return data_[i * dims_[1] + j];
    }

private:
    pointer data_;
    std::array<size_t, 3> dims_;
};

} // namespace stdx
