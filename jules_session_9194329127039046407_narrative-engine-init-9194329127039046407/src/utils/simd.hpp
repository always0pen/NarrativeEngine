#pragma once

#if __has_include(<experimental/simd>)
#include <experimental/simd>
namespace stdx {
    namespace simd = std::experimental;
    using native_simd = std::experimental::native_simd<uint64_t>;
    using element_aligned_tag = std::experimental::element_aligned_tag;
    using vector_aligned_tag = std::experimental::vector_aligned_tag;
    using overaligned_tag = std::experimental::overaligned_tag<16>; // fallback
}
#else
// Fallback if no SIMD available
#include <array>
#include <cstdint>
namespace stdx {
    struct element_aligned_tag {};
    struct vector_aligned_tag {};
    
    // Minimal scalar fallback mimicking simd interface
    template<typename T>
    struct scalar_simd {
        T value;
        scalar_simd(T v) : value(v) {}
        scalar_simd() : value(0) {}
        
        // Basic operators
        scalar_simd operator&(const scalar_simd& other) const { return value & other.value; }
        scalar_simd operator|(const scalar_simd& other) const { return value | other.value; }
        scalar_simd operator^(const scalar_simd& other) const { return value ^ other.value; }
        scalar_simd operator~() const { return ~value; }
        
        explicit operator T() const { return value; }
        
        // Load/Store methods (stub for fallback)
        void copy_from(const T* ptr, element_aligned_tag) { value = *ptr; }
        void copy_to(T* ptr, element_aligned_tag) const { *ptr = value; }
        
        static constexpr size_t size() { return 1; }
    };
    using native_simd = scalar_simd<uint64_t>;
}
#endif
