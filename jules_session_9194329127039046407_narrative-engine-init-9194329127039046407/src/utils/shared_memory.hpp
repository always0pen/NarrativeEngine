#pragma once

#include <cstddef>
#include <memory>

namespace narrative {

class SharedBuffer {
public:
    SharedBuffer(size_t size_bytes);
    ~SharedBuffer();
    
    // Non-copyable
    SharedBuffer(const SharedBuffer&) = delete;
    SharedBuffer& operator=(const SharedBuffer&) = delete;
    
    void* contents();
    const void* contents() const;
    size_t size() const;
    
    // Platform specific handle (MTLBuffer on Apple)
    void* platform_handle() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace narrative
