// Created by Walid KESBI and ABDELHAKIM NAHILI on 12/11/2025.

#pragma once
#include "utils.hpp"
#include "piece.hpp"

class Domino : public Piece {   
    private:
        int m_a;
        int m_b;
        int m_angle;
        bool m_validated;
        
    public:
        int a() const { return m_a; }
        int b() const { return m_b; }
        double xc() const { return xb() + width() / 2; }
        double yc() const { return yb() + height() / 2; }
        int angle() const { return m_angle; }
        bool validated() const { return m_validated; }
        
        void a(int val) { m_a = val; }
        void b(int val) { m_b = val; }
        void xc(double val) { xb(val - width() / 2); }
        void yc(double val) { yb(val - height() / 2); }
        void angle(int val) { m_angle = val; }
        void validated(bool val) { m_validated = val; }

        Domino(Board* board, int a, int b, double xc, double yc, int angle);
        void dot(const CairoContext& cr, double x, double y) const;
        void drawPips(const CairoContext& cr, double cx, double cy, int value) const;
        void draw (const CairoContext& cr) const override;
        void on_button_release(double ev_x, double ev_y, int ev_button) override;
        bool rivet_is_clicked(double ev_x, double ev_y) const;
};