// Created by Walid KESBI and ABDELHAKIM NAHILI on 12/11/2025.

#include <iostream>
#include "domino.hpp"
#include "main_window.hpp"
#include "my_data.hpp"
#include "board.hpp"

//------------------------------- D O M I N O -------------------------------

Domino::Domino(Board* board,int a, int b,double xc,double yc, int angle)
    : Piece(board, xc,yc, DOMINO_SIDE*2, DOMINO_SIDE), m_a{a}, m_b{b}, m_angle{angle} {
    m_validated = false;
    }

void Domino::dot(const CairoContext& cr, double x, double y) const { 
    const double r = 6;
    cr->arc(x, y, r, 0.0, 2 * M_PI);
    cr->fill();
};

void Domino::drawPips(const CairoContext& cr, double cx, double cy, int value) const {
    const double offset = DOMINO_SIDE * 0.25;

    switch (value) {
        case 1:
            dot(cr, cx, cy);
            break;
        case 2:
            dot(cr, cx - offset, cy - offset);
            dot(cr, cx + offset, cy + offset);
            break;
        case 3:
            dot(cr, cx, cy);
            dot(cr, cx - offset, cy - offset);
            dot(cr, cx + offset, cy + offset);
            break;
        case 4:
            dot(cr, cx - offset, cy - offset);
            dot(cr, cx + offset, cy - offset);
            dot(cr, cx - offset, cy + offset);
            dot(cr, cx + offset, cy + offset);
            break;
        case 5:
            dot(cr, cx, cy);
            dot(cr, cx - offset, cy - offset);
            dot(cr, cx + offset, cy - offset);
            dot(cr, cx - offset, cy + offset);
            dot(cr, cx + offset, cy + offset);
            break;
        case 6:
            if(m_angle == 90 || m_angle == 270) {
                dot(cr, cx - offset, cy - offset);
                dot(cr, cx + offset, cy - offset);
                dot(cr, cx - offset, cy);
                dot(cr, cx + offset, cy);
                dot(cr, cx - offset, cy + offset);
                dot(cr, cx + offset, cy + offset);
            } else {
                dot(cr, cx - offset, cy - offset);
                dot(cr, cx - offset, cy + offset);
                dot(cr, cx, cy - offset);
                dot(cr, cx, cy + offset);
                dot(cr, cx + offset, cy - offset);
                dot(cr, cx + offset, cy + offset);
            }
            break;
    }
};


// --------------------------- DRAW ---------------------------------

void Domino::draw (const CairoContext& cr) const {

    switch (m_angle) {
        case 0: 
            cr->rectangle(xc() - DOMINO_SIDE, yc() - DOMINO_SIDE / 2,
                          2 * DOMINO_SIDE, DOMINO_SIDE);
            cr->move_to(xc(), yc() - DOMINO_SIDE / 2);
            cr->line_to(xc(), yc() + DOMINO_SIDE / 2);
            break;

        case 90: 
            cr->rectangle(xc() - DOMINO_SIDE / 2, yc() - DOMINO_SIDE,
                          DOMINO_SIDE, 2 * DOMINO_SIDE);
            cr->move_to(xc() - DOMINO_SIDE / 2, yc());
            cr->line_to(xc() + DOMINO_SIDE / 2, yc());
            break;

        case 180:
            cr->rectangle(xc() - DOMINO_SIDE, yc() - DOMINO_SIDE / 2,
                          2 * DOMINO_SIDE, DOMINO_SIDE);
            cr->move_to(xc(), yc() - DOMINO_SIDE / 2);
            cr->line_to(xc(), yc() + DOMINO_SIDE / 2);
            break;

        case 270:
            cr->rectangle(xc() - DOMINO_SIDE / 2, yc() - DOMINO_SIDE,
                          DOMINO_SIDE, 2 * DOMINO_SIDE);
            cr->move_to(xc() - DOMINO_SIDE / 2, yc());
            cr->line_to(xc() + DOMINO_SIDE / 2, yc());
            break;
    }

    if (m_validated) {
        cr->set_source_rgb(1.0, 1.0, 1.0);  
    } else {
        cr->set_source_rgb(0.5, 0.8, 1.0);  
    }
    cr->set_line_width(2);
    cr->fill_preserve();
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->stroke();


    cr->set_source_rgb(1, 0, 0);
    cr->arc(xc(), yc(), 4, 0, 2 * M_PI);
    cr->fill();

   
    cr->set_source_rgb(0, 0, 0);

    if (m_angle == 0) { 
        drawPips(cr, xc() - DOMINO_SIDE / 2, yc(), m_a);
        drawPips(cr, xc() + DOMINO_SIDE / 2, yc(), m_b);
    }
    if (m_angle == 90) {
        drawPips(cr, xc(), yc() - DOMINO_SIDE / 2, m_a);
        drawPips(cr, xc(), yc() + DOMINO_SIDE / 2, m_b);
    }
    if (m_angle == 180) { 
        drawPips(cr, xc() - DOMINO_SIDE / 2, yc(), m_b);
        drawPips(cr, xc() + DOMINO_SIDE / 2, yc(), m_a);
    }
    if (m_angle == 270) { 
        drawPips(cr, xc(), yc() - DOMINO_SIDE / 2, m_b);
        drawPips(cr, xc(), yc() + DOMINO_SIDE / 2, m_a);
    }
}


void Domino::on_button_release(double ev_x, double ev_y, int ev_button) {
    std::cout << "Domino Button released at (" << ev_x << ", " << ev_y
              << ") with button " << ev_button << std::endl;

    if (rivet_is_clicked(ev_x, ev_y)) {
        double cx = xc();
        double cy = yc();

        angle((m_angle + 90) % 360);

        width ((m_angle == 0 || m_angle == 180) ? DOMINO_SIDE * 2 : DOMINO_SIDE);
        height((m_angle == 0 || m_angle == 180) ? DOMINO_SIDE : DOMINO_SIDE * 2);

        xc(cx);
        yc(cy);
    }
    if (place_on_magnetic_grid(DOMINO_SIDE/2.0, DOMINO_SIDE/4.0)) {
        if (m_board->validate_domino_position(*this)) {
            m_validated = true;
        } else {
            m_validated = false;
        }
    }
}

bool Domino::rivet_is_clicked(double ev_x, double ev_y) const {
    return (ev_x >= xc() - 4 &&
            ev_x <= xc() + 4 &&
            ev_y >= yc() - 4 &&
            ev_y <= yc() + 4);
}
