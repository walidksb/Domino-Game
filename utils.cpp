// Created by Walid KESBI and ABDELHAKIM NAHILI on 12/11/2025.

#include <cmath>
#include "utils.hpp"
#include "domino.hpp"

const double PI = 3.141592653589793238463;
const int DOMINO_SIDE = 80;

int get_magnet_coord (double grid_side, double t)
{
    return t / grid_side + (t >= 0 ? 0.5 : -0.5);
}

void BoundingBox::init(double xmin, double xmax, double ymin, double ymax)
{
    m_xmin = xmin;
    m_xmax = xmax;
    m_ymin = ymin;
    m_ymax = ymax;
}

void BoundingBox::update_min(double x, double y)
{
    m_xmin = std::min(m_xmin, x);
    m_ymin = std::min(m_ymin, y);
}
void BoundingBox::update_max(double x, double y)
{
    m_xmax = std::max(m_xmax, x);
    m_ymax = std::max(m_ymax, y);
}

