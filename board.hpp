#pragma once
#include <iostream>
#include "domino.hpp"
#include <vector>
#include "utils.hpp"
#include "deck.hpp"
#include "utils.tpp"

class Board {
    private:
    int m_piece_num_clicked = -1;
    int m_dominos_shown_num = 0;
    
    public:
    std::vector<Domino> m_dominos;
    std::vector<Piece*> m_pieces;
    Deck m_deck;

    int piece_num_clicked() const { return m_piece_num_clicked; }
    void piece_num_clicked(int val) { m_piece_num_clicked = val; }
    int dominos_shown_num() const { return m_dominos_shown_num; }
    void dominos_shown_num(int val) { m_dominos_shown_num = val; }

    Board();
    void populate_pieces();
    void draw_pieces(const CairoContext& cr) const;
    bool find_piece_clicked(double ev_x, double ev_y);
    void move_piece_to_top(int piece_num);
    void shuffle_dominos();
    void align_dominos_on_grid();
    void reveal_next_domino(double x, double y);
    void restart_game();
    void draw_magnetic_grid(const CairoContext& cr, int darea_width, int darea_height) const;
    void populate_dominos();
    bool validate_domino_position(const Domino& domino) const;
    void find_bounding_box(BoundingBox& bbox) const;
    void fill_marking_pieces(Grid2D<int>& grid, const BoundingBox& bbox) const;
    bool marking_is_empty(const Grid2D<int>& grid, const BoundingBox& bbox, const Domino& d) const;
    bool check_neighbours(const Grid2D<int>& grid, const BoundingBox& bbox, const Domino& d) const;

};