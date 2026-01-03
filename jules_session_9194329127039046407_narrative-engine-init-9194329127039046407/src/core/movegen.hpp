#pragma once

#include "position.hpp"
#include <vector>

namespace narrative {

struct Move {
    Square from;
    Square to;
    // Special flags (promo, castle) omitted for Phase 1
    
    bool operator==(const Move& other) const { return from == other.from && to == other.to; }
};

class MoveGen {
public:
    static std::vector<Move> generate(const Position& pos);
};

} // namespace narrative
