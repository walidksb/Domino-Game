// Created by Walid KESBI and ABDELHAKIM NAHILI on 12/11/2025.

#pragma once
#include <gtkmm-3.0/gtkmm.h>

// Pour connecter un signal à une fonction
#define CONNECT(signal, func) \
    signal().connect( sigc::mem_fun(*this, &func) )

// Pour les dessins avec Cairo
typedef ::Cairo::RefPtr<::Cairo::Context> CairoContext;

extern const double PI;
extern const int DOMINO_SIDE;
int get_magnet_coord (double grid_side, double t);
struct BoundingBox {
    double m_xmin = 0;
    double m_xmax = 0;
    double m_ymin = 0;
    double m_ymax = 0;

    void init(double xmin, double xmax, double ymin, double ymax);
    void update_min(double x, double y);
    void update_max(double x, double y);
};