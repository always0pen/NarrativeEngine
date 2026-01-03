#include "shared_memory.hpp"
#include <vector>
#include <cstdlib>

namespace narrative {

struct SharedBuffer::Impl {
    std::vector<uint8_t> data;
    size_t size;
    
    Impl(size_t s) : size(s) {
        data.resize(s);
        // Ensure alignment if needed, vector is usually aligned enough for float
    }
};

SharedBuffer::SharedBuffer(size_t size_bytes) 
    : impl_(std::make_unique<Impl>(size_bytes)) {}

SharedBuffer::~SharedBuffer() = default;

void* SharedBuffer::contents() {
    return impl_->data.data();
}

const void* SharedBuffer::contents() const {
    return impl_->data.data();
}

size_t SharedBuffer::size() const {
    return impl_->size;
}

void* SharedBuffer::platform_handle() const {
    return nullptr;
}

} // namespace narrative
