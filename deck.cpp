#include "deck.hpp"
#include "board.hpp"

Deck::Deck(Board* board)
    : Piece(board, 20, 20, DOMINO_SIDE*2, DOMINO_SIDE*3){}

    
void Deck::draw(const CairoContext& cr) const
{
    Cairo::TextExtents extents;
    const char* s = "?";
    const char* s2 = "X";
    double fsize = 60.0;
    
    cr->set_font_size(fsize);
    cr->get_text_extents(s, extents);
    
    
    cr->rectangle(m_xb, m_yb , m_width, m_height);
    cr->set_source_rgb(0.2, 0.5, 0.2);
    cr->fill_preserve();
    cr->set_source_rgb(0.0, 0.3, 0.0);
    cr->set_line_width(8.0);
    cr->stroke();
    
    
    double x = m_xb + m_width / 2.0;
    double y = m_yb + m_height / 2.0;
    
    cr->move_to(
        x - extents.width / 2.0,
        y + extents.height / 2.0
    );
    
    cr->set_source_rgb(1.0, 1.0, 1.0);
    if(m_board->dominos_shown_num() >= 28){
        cr->show_text(s2);
    }else{
        cr->show_text(s);
    }
    cr->stroke();
}

void Deck::on_button_press(double ev_x, double ev_y, int ev_button, bool &raise_on_top)
{
    if (is_clicked(ev_x, ev_y))
    {
        raise_on_top = false;
    }
   

    if((ev_x >= xb() && ev_x <= xb() + width() && ev_y >= yb() && ev_y <= yb() + 8) 
        || (ev_x >= xb() && ev_x <= xb() + width() &&  ev_y >= yb() + height() - 8 && ev_y <= yb() + height())
        || (ev_x >= xb() && ev_x <= xb() + 8 && ev_y >= yb() && ev_y <= yb() + height()) 
        || (ev_x >= xb() + width() - 8 && ev_x <= xb() + width() &&ev_y >= yb() && ev_y <= yb() + height()))
    {
        m_motion_flag = true;
        m_is_border_clicked = true;
    }
}

void Deck::on_button_release(double ev_x, double ev_y, int ev_button)
{
    if(m_motion_flag) place_on_magnetic_grid(DOMINO_SIDE/2.0, DOMINO_SIDE/4.0);
    
    m_motion_flag = false;
    if(!m_is_border_clicked)m_board->reveal_next_domino(ev_x, ev_y);
    m_is_border_clicked = false;

}

void Deck::on_motion_notify(double ev_x, double ev_y, double prev_x, double prev_y, int ev_button)
{

    if (m_motion_flag) this->Piece::on_motion_notify(ev_x, ev_y, prev_x, prev_y, ev_button);
}