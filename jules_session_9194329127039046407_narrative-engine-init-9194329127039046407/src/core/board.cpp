#include "board.hpp"
#include <cstring> // for memset

namespace narrative {

Board::Board() {
    clear();
}

void Board::clear() {
    color_bb_.fill(0);
    piece_bb_.fill(0);
    board_arr_.fill(NO_PIECE);
}

Bitboard Board::by_color(Color c) const {
    return color_bb_[c];
}

Bitboard Board::by_piece(PieceType pt) const {
    return piece_bb_[pt];
}

Bitboard Board::by_color_piece(Color c, PieceType pt) const {
    return color_bb_[c] & piece_bb_[pt];
}

Piece Board::piece_on(Square s) const {
    return board_arr_[s];
}

void Board::put_piece(Piece p, Square s) {
    if (p == NO_PIECE) return;
    
    Color c = (p < 8) ? WHITE : BLACK;
    PieceType pt = static_cast<PieceType>(p % 8);
    
    Bitboard mask = 1ULL << s;
    color_bb_[c] |= mask;
    piece_bb_[pt] |= mask;
    board_arr_[s] = p;
}

void Board::remove_piece(Square s) {
    Piece p = board_arr_[s];
    if (p == NO_PIECE) return;
    
    Color c = (p < 8) ? WHITE : BLACK;
    PieceType pt = static_cast<PieceType>(p % 8);
    
    Bitboard mask = ~(1ULL << s);
    color_bb_[c] &= mask;
    piece_bb_[pt] &= mask;
    board_arr_[s] = NO_PIECE;
}

Square Board::square_from_coords(int rank, int file) {
    return rank * 8 + file;
}

std::string Board::square_to_string(Square s) {
    if (s < 0 || s >= 64) return "-";
    char f = 'a' + (s % 8);
    char r = '1' + (s / 8);
    return {f, r};
}

Bitboard SimdBitboardOps::unite_4(Bitboard b1, Bitboard b2, Bitboard b3, Bitboard b4) {
    return b1 | b2 | b3 | b4;
}

void SimdBitboardOps::batch_and(const Bitboard* inputs, Bitboard mask, Bitboard* outputs, size_t count) {
    // Process using SIMD chunks
    size_t i = 0;
    
    // Check alignment and stride
    // This is a simplified loop that works for both scalar wrapper and real SIMD
    // provided stdx::native_simd supports load/store or we interpret it.
    
    // If using real SIMD:
    #if defined(__cpp_lib_experimental_parallel_simd) || defined(__cpp_lib_simd)
    using V = stdx::native_simd;
    size_t vec_size = V::size();
    
    // Broadcast mask
    V v_mask(mask);
    
    for (; i + vec_size <= count; i += vec_size) {
        V v_in;
        v_in.copy_from(inputs + i, stdx::element_aligned_tag{});
        V v_out = v_in & v_mask;
        v_out.copy_to(outputs + i, stdx::element_aligned_tag{});
    }
    #endif
    
    // Scalar cleanup (or full loop if no SIMD)
    for (; i < count; ++i) {
        outputs[i] = inputs[i] & mask;
    }
}

} // namespace narrative
