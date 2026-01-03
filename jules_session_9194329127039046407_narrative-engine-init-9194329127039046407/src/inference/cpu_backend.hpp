#pragma once

#include "inference_engine.hpp"

namespace narrative {

class CPUInferenceEngine : public InferenceEngine {
public:
    PolicyValue evaluate(const Position& pos) override {
        // Dummy evaluation for testing
        // Random policy, drawish value
        return {{}, 0.0f}; 
    }
};

} // namespace narrative
