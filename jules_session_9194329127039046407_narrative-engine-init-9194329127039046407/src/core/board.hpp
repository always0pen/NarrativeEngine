#pragma once

#include <cstdint>
#include <array>
#include <string>
#include "../utils/simd.hpp"

namespace narrative {

using Bitboard = uint64_t;
using Square = int;

// Constants for Board
constexpr int BOARD_SIZE = 64;
constexpr int NUM_COLORS = 2;
constexpr int NUM_PIECE_TYPES = 6;

enum Color { WHITE, BLACK, COLOR_NB };
enum PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, PIECE_TYPE_NB, NO_PIECE_TYPE = 6 };
enum Piece {
    W_PAWN = 0, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN = 8, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
    NO_PIECE
};

class Board {
public:
    Board();
    
    // Core bitboard access
    Bitboard by_color(Color c) const;
    Bitboard by_piece(PieceType pt) const;
    Bitboard by_color_piece(Color c, PieceType pt) const;
    
    Piece piece_on(Square s) const;
    void put_piece(Piece p, Square s);
    void remove_piece(Square s);
    
    // Helpers
    static Square square_from_coords(int rank, int file);
    static std::string square_to_string(Square s);
    
    void clear();

private:
    std::array<Bitboard, NUM_COLORS> color_bb_;
    std::array<Bitboard, NUM_PIECE_TYPES> piece_bb_;
    std::array<Piece, BOARD_SIZE> board_arr_;
};

// SIMD Operations Helper
struct SimdBitboardOps {
    // Process 4 bitboards in parallel using SIMD
    // Returns the bitwise OR of 4 bitboards
    static Bitboard unite_4(Bitboard b1, Bitboard b2, Bitboard b3, Bitboard b4);
    
    // Batch operation: Apply a mask to 'count' bitboards using SIMD
    static void batch_and(const Bitboard* inputs, Bitboard mask, Bitboard* outputs, size_t count);
};

} // namespace narrative
