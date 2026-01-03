#pragma once

#include "../utils/mdspan.hpp"
#include "../utils/shared_memory.hpp"
#include <memory>

namespace narrative {

// A tensor that owns its memory (via SharedBuffer) 
// but exposes it via mdspan for Zero-Copy access.
template<typename T, size_t D0, size_t D1, size_t D2>
class Tensor3D {
public:
    Tensor3D() : buffer_(std::make_shared<SharedBuffer>(sizeof(T) * D0 * D1 * D2)) {}
    
    // Get a view of the tensor
    auto view() {
        return stdx::mdspan<T, stdx::extents<size_t, D0, D1, D2>>(
            static_cast<T*>(buffer_->contents()), D0, D1, D2
        );
    }
    
    // Const view
    auto view() const {
        return stdx::mdspan<const T, stdx::extents<size_t, D0, D1, D2>>(
            static_cast<const T*>(buffer_->contents()), D0, D1, D2
        );
    }

private:
    std::shared_ptr<SharedBuffer> buffer_;
};

} // namespace narrative
