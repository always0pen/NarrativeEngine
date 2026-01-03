#ifdef __APPLE__

#include "inference_engine.hpp"
#import <Metal/Metal.h>
#import <MetalPerformanceShadersGraph/MetalPerformanceShadersGraph.h>

namespace narrative {

class MPSInferenceEngine : public InferenceEngine {
public:
    MPSInferenceEngine() {
        // Initialize Metal Device and MPSGraph
        device_ = MTLCreateSystemDefaultDevice();
        commandQueue_ = [device_ newCommandQueue];
        graph_ = [[MPSGraph alloc] init];
        
        build_graph();
    }
    
    PolicyValue evaluate(const Position& pos) override {
        // Runtime execution using MPSGraphExecutable
        // 1. Convert Position to input Tensor (SharedBuffer)
        // 2. Feed to Graph
        // 3. Extract output
        
        // This is a stub for the runtime logic. 
        // In a real implementation, we would create an MPSGraphTensorData,
        // bind it to the input placeholder, and run the graph.
        return {{}, 0.5f};
    }

private:
    void build_graph() {
        // Define Input: Batch=1, H=8, W=8, C=112 (Standard Chess Input)
        MPSGraphTensor* input = [graph_ placeholderWithShape:@[@1, @8, @8, @112]
                                                    dataType:MPSDataTypeFloat32
                                                        name:@"input"];
        
        // --- MobileViT Block Simulation ---
        
        // 1. Initial Convolution (Feature Extraction)
        // Conv 3x3, 112 -> 256
        MPSGraphTensor* conv1 = [graph_ convolution2DWithSourceTensor:input
                                                        weightsTensor:[graph_ constantWithScalar:0.1 shape:@[@3, @3, @112, @256] dataType:MPSDataTypeFloat32]
                                                           descriptor:[MPSGraphConvolution2DDescriptor descriptorWithStrideInX:1 strideInY:1 dilationRateInX:1 dilationRateInY:1 groups:1 paddingLeft:1 paddingRight:1 paddingTop:1 paddingBottom:1 dataLayout:MPSGraphTensorNamedDataLayoutNHWC weightsLayout:MPSGraphTensorNamedDataLayoutHWIO]
                                                                 name:@"conv1"];
        
        MPSGraphTensor* relu1 = [graph_ reLUWithTensor:conv1 name:@"relu1"];
        
        // 2. Global Representation (Self-Attention) using MultiHeadAttention
        // This maps to the ANE's hardware acceleration for attention.
        // Reshape for attention: [B, H*W, C] -> [1, 64, 256]
        MPSGraphTensor* flat = [graph_ reshapeTensor:relu1 
                                           withShape:@[@1, @64, @256] 
                                                name:@"flat"];
        
        // Query, Key, Value projections (Simplified: identity for this stub)
        MPSGraphTensor* query = flat;
        MPSGraphTensor* key = flat;
        MPSGraphTensor* value = flat;
        
        // MultiHeadAttention
        // Using explicit API or building it from primitives. 
        // MPSGraph doesn't have a single "TransformerBlock" API, but has MatMul + Softmax.
        // We will simulate it with MatMuls.
        
        // Q * K^T
        MPSGraphTensor* qt = [graph_ transposeTensor:key dimension:-1 withDimension:-2 name:nil];
        MPSGraphTensor* scores = [graph_ matrixMultiplicationWithPrimaryTensor:query 
                                                               secondaryTensor:qt 
                                                                          name:@"scores"];
        MPSGraphTensor* attn = [graph_ softMaxWithTensor:scores axis:-1 name:@"attn"];
        
        // Attn * V
        MPSGraphTensor* context = [graph_ matrixMultiplicationWithPrimaryTensor:attn 
                                                                secondaryTensor:value 
                                                                           name:@"context"];
        
        // 3. Readout Heads
        // Value Head: Global Pooling -> Dense -> Tanh
        MPSGraphTensor* pooled = [graph_ meanWithTensor:context axes:@[@1] name:@"pool"]; // [1, 256]
        MPSGraphTensor* value_logit = [graph_ matrixMultiplicationWithPrimaryTensor:pooled
                                                                   secondaryTensor:[graph_ constantWithScalar:0.1 shape:@[@256, @1] dataType:MPSDataTypeFloat32]
                                                                              name:@"value_w"];
        MPSGraphTensor* value_out = [graph_ tanhWithTensor:value_logit name:@"value_out"];
        
        // Policy Head: Conv 1x1 -> Flatten -> Softmax
        // ... omitted for brevity, similar structure
        
        // Final Outputs (referenced to keep graph alive)
        targetTensors_ = @[value_out];
    }

    id<MTLDevice> device_;
    id<MTLCommandQueue> commandQueue_;
    MPSGraph* graph_;
    NSArray<MPSGraphTensor*>* targetTensors_;
};

std::unique_ptr<InferenceEngine> create_mps_inference_engine() {
    return std::make_unique<MPSInferenceEngine>();
}

} // namespace narrative

#endif // __APPLE__
