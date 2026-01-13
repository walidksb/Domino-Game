#pragma once
#include "piece.hpp"
#include "utils.hpp"


class Deck : public Piece {
    private:
    bool m_motion_flag = false;
    bool m_is_border_clicked = false;
    

    public:
        Deck(Board* board);
        void on_button_press(double ev_x, double ev_y, int ev_button, bool &raise_on_top) override;
        void on_button_release(double ev_x, double ev_y, int ev_button) override;
        void on_motion_notify(double ev_x, double ev_y, double prev_x, double prev_y, int ev_button) override;
        void draw(const CairoContext& cr) const override;

};