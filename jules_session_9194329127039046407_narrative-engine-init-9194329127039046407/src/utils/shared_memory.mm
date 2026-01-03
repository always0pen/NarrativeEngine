#include "shared_memory.hpp"
#import <Metal/Metal.h>
#include <stdexcept>

namespace narrative {

struct SharedBuffer::Impl {
    id<MTLBuffer> buffer;
    size_t size;
    
    Impl(size_t s) : size(s) {
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        if (!device) {
            throw std::runtime_error("Failed to create Metal device");
        }
        
        // Use Shared storage mode for Zero-Copy access by both CPU and GPU
        MTLResourceOptions options = MTLResourceStorageModeShared;
        buffer = [device newBufferWithLength:size options:options];
        
        if (!buffer) {
            throw std::runtime_error("Failed to allocate Shared Buffer");
        }
    }
};

SharedBuffer::SharedBuffer(size_t size_bytes) 
    : impl_(std::make_unique<Impl>(size_bytes)) {}

SharedBuffer::~SharedBuffer() = default;

void* SharedBuffer::contents() {
    return [impl_->buffer contents];
}

const void* SharedBuffer::contents() const {
    return [impl_->buffer contents];
}

size_t SharedBuffer::size() const {
    return impl_->size;
}

void* SharedBuffer::platform_handle() const {
    return (__bridge void*)impl_->buffer;
}

} // namespace narrative
