#include "piece.hpp"
#include <iostream>
#include "board.hpp"

Piece::Piece(Board* board, double xb, double yb, double width, double height)
    : m_board(board), m_xb(xb), m_yb(yb), m_width(width), m_height(height) {}


void Piece::draw(const CairoContext &cr) const
{
    cr->rectangle(m_xb, m_yb, m_width, m_height);
    cr->set_source_rgb(0.0, 0.5, 0.0);
    cr->fill_preserve();
    cr->set_source_rgb(0.5, 0.5, 0.5);
    cr->set_line_width(2.0);
    cr->stroke();
}

bool Piece::is_clicked(double ev_x, double ev_y) const
{

    std::cout << "is_clicked: (" << ev_x << ", " << ev_y << ") on piece at (" << m_xb << ", " << m_yb << ") with size (" << m_width << ", " << m_height << ")" << std::endl;

    return (ev_x >= m_xb && ev_x <= m_xb + m_width &&
            ev_y >= m_yb && ev_y <= m_yb + m_height);
}

void Piece::on_button_press(double ev_x, double ev_y, int ev_button, bool &raise_on_top)
{
    if (is_clicked(ev_x, ev_y))
    {
        raise_on_top = true;
    }
}

void Piece::on_button_release(double ev_x, double ev_y, int ev_button)
{

    std::cout << "Button released at (" << ev_x << ", " << ev_y << ") with button " << ev_button << std::endl;
}

void Piece::on_motion_notify(double ev_x, double ev_y, double prev_x, double prev_y, int ev_button)
{

    std::cout << "Motion notify: (" << ev_x << ", " << ev_y << ") from (" << prev_x << ", " << prev_y << ") with button " << ev_button << std::endl;
    if (ev_button == 1)
    {
        double dx = ev_x - prev_x;
        double dy = ev_y - prev_y;
        m_xb += dx;
        m_yb += dy;
    }
}

void Piece::get_closer_magnet(double grid_side, int& x_mag, int& y_mag) const
{
    x_mag = get_magnet_coord(grid_side, m_xb);
    y_mag = get_magnet_coord(grid_side, m_yb);
}

bool Piece::place_on_magnetic_grid(double grid_side, double dist_threshold)
{
    int x_mag, y_mag;
    get_closer_magnet(grid_side, x_mag, y_mag);

    double mag_x = x_mag * grid_side;
    double mag_y = y_mag * grid_side;

    double dist = std::sqrt((m_xb - mag_x) * (m_xb - mag_x) + (m_yb - mag_y) * (m_yb - mag_y));

    if (dist <= dist_threshold)
    {
        m_xb = mag_x;
        m_yb = mag_y;
        return true;
    }
    return false;
}