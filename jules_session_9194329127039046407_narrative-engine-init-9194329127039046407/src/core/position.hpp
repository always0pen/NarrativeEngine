#pragma once

#include "board.hpp"

namespace narrative {

class Position {
public:
    Position();
    
    // Setup position from FEN (simplified)
    void set_fen(const std::string& fen);
    
    const Board& get_board() const { return board_; }
    Color side_to_move() const { return side_to_move_; }

private:
    Board board_;
    Color side_to_move_;
    // Castling rights, ep square, etc. omitted for brevity in Phase 1
};

} // namespace narrative
