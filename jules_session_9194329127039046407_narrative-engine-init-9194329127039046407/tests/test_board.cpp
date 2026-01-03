#include "../src/core/position.hpp"
#include <iostream>
#include <cassert>

int main() {
    narrative::Position pos;
    // Start position FEN
    pos.set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    const narrative::Board& board = pos.get_board();
    
    // Check white pawns
    narrative::Bitboard w_pawns = board.by_color_piece(narrative::WHITE, narrative::PAWN);
    // Rank 2 is full of pawns: 0000...11111111... (bits 8-15)
    // 0xFF << 8
    assert(w_pawns == 0xFF00);
    
    // Check black king on e8 (square 60)
    narrative::Piece k = board.piece_on(60);
    assert(k == narrative::B_KING);
    
    std::cout << "Board tests passed!" << std::endl;
    return 0;
}
