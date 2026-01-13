#pragma once
#include "utils.hpp"


class Board;

class Piece {
    protected:
        Board* m_board;
        double m_xb;
        double m_yb;
        double m_width;
        double m_height;

    public:
        double xb() const { return m_xb; }
        double yb() const { return m_yb; }
        double width() const { return m_width; }
        double height() const { return m_height; }
        
        void xb(double val) { m_xb = val; }
        void yb(double val) { m_yb = val; }
        void width(double val) { m_width = val; }
        void height(double val) { m_height = val; }

        Piece(Board* board, double xb, double yb, double width, double height);

        virtual void draw(const CairoContext& cr) const;
        virtual void on_button_press(double ev_x, double ev_y, int ev_button, bool &raise_on_top);
        virtual void on_button_release(double ev_x, double ev_y, int ev_button);
        virtual void on_motion_notify(double ev_x, double ev_y, double prev_x, double prev_y, int ev_button);
        bool is_clicked(double ev_x, double ev_y) const;
        void get_closer_magnet(double grid_side, int& x_mag, int& y_mag) const;
        bool place_on_magnetic_grid(double grid_side, double dist_threshold);

};