#include "position.hpp"
#include <sstream>

namespace narrative {

Position::Position() : side_to_move_(WHITE) {}

void Position::set_fen(const std::string& fen) {
    board_.clear();
    
    std::istringstream ss(fen);
    std::string placement, turn, castling, ep, half, full;
    ss >> placement >> turn;
    
    int rank = 7;
    int file = 0;
    
    for (char c : placement) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (isdigit(c)) {
            file += c - '0';
        } else {
            Piece p = NO_PIECE;
            switch (c) {
                case 'P': p = W_PAWN; break;
                case 'N': p = W_KNIGHT; break;
                case 'B': p = W_BISHOP; break;
                case 'R': p = W_ROOK; break;
                case 'Q': p = W_QUEEN; break;
                case 'K': p = W_KING; break;
                case 'p': p = B_PAWN; break;
                case 'n': p = B_KNIGHT; break;
                case 'b': p = B_BISHOP; break;
                case 'r': p = B_ROOK; break;
                case 'q': p = B_QUEEN; break;
                case 'k': p = B_KING; break;
            }
            if (p != NO_PIECE) {
                board_.put_piece(p, Board::square_from_coords(rank, file));
                file++;
            }
        }
    }
    
    side_to_move_ = (turn == "w") ? WHITE : BLACK;
}

} // namespace narrative
