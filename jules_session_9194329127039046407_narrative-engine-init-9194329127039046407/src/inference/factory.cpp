#include "inference_engine.hpp"
#include "cpu_backend.hpp"

namespace narrative {

#ifdef __APPLE__
std::unique_ptr<InferenceEngine> create_mps_inference_engine();
#endif

std::unique_ptr<InferenceEngine> create_inference_engine() {
#ifdef __APPLE__
    return create_mps_inference_engine();
#else
    return std::make_unique<CPUInferenceEngine>();
#endif
}

} // namespace narrative
