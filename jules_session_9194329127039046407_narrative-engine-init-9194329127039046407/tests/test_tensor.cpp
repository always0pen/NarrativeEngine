#include "../src/core/tensor.hpp"
#include <iostream>
#include <cassert>

int main() {
    // 8x8x112 tensor (e.g. Lc0 input)
    narrative::Tensor3D<float, 8, 8, 112> input_tensor;
    
    auto view = input_tensor.view();
    
    // Write to it
    view[0, 0, 0] = 1.0f;
    view[7, 7, 111] = 0.5f;
    
    // Read back
    assert((view[0, 0, 0] == 1.0f));
    assert((view[7, 7, 111] == 0.5f));
    
    std::cout << "Tensor tests passed!" << std::endl;
    return 0;
}
