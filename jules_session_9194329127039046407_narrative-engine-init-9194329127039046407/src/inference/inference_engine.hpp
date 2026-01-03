#pragma once

#include "../core/tensor.hpp"
#include "../core/position.hpp"
#include <vector>
#include <utility>

namespace narrative {

struct PolicyValue {
    std::vector<float> policy; // Move probabilities
    float value;               // Win probability
};

class InferenceEngine {
public:
    virtual ~InferenceEngine() = default;
    
    // Evaluate a batch of positions
    // In a real engine, we'd batch multiple requests.
    // Here we just define the interface.
    virtual PolicyValue evaluate(const Position& pos) = 0;
};

// Factory to create the appropriate backend
std::unique_ptr<InferenceEngine> create_inference_engine();

} // namespace narrative
