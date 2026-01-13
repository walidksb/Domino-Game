#include "board.hpp"
#include <iomanip>

Board::Board() : m_deck(this) {
    populate_dominos();
    populate_pieces();
}

void Board::populate_dominos() {
    int index = 0;

    for (int i = 0; i <= 6; ++i) {
        for (int j = i; j <= 6; ++j) {

            int col = index % 7;
            int row = index / 7;

            double xc = 100 + col * (DOMINO_SIDE + 20);
            double yc = 100 + row * (DOMINO_SIDE*2 + 40);

            m_dominos.emplace_back(this, i, j, xc, yc, 90);

            index++;
        }
    };
}



void Board::populate_pieces() {
    for(int i =0; i < m_dominos_shown_num; i++){
        m_pieces.push_back(&m_dominos[i]);
    }
    m_pieces.push_back(&m_deck);
}

void Board::draw_pieces(const CairoContext& cr) const {
    for (std::size_t i = m_pieces.size(); i-- > 0; ) {
        m_pieces[i]->draw(cr);
    }
}

bool Board::find_piece_clicked(double ev_x, double ev_y) {
    for (size_t i = 0; i < m_pieces.size(); ++i) {
        if (m_pieces[i]->is_clicked(ev_x, ev_y)) {
            m_piece_num_clicked = i;
            std::cout << "Piece clicked: " << i << std::endl;
            return true;
        }
    }
    return false;
}

void Board::move_piece_to_top(int piece_num) {
    if (piece_num < 0) {
        return;
    }
    Piece* selected_piece = m_pieces[piece_num];
    m_pieces.erase(m_pieces.begin() + piece_num);
    m_pieces.insert(m_pieces.begin(), selected_piece);
}

void Board::shuffle_dominos() {
    m_dominos = shuffle_vector(m_dominos);
    for(size_t i = 0; i < m_dominos.size(); i++){
        m_dominos[i].angle(90);
    }
    m_pieces.clear();
    populate_pieces();
}

void Board::align_dominos_on_grid() {
    int index = 10;
    for (auto &d : m_dominos) {
        int col = index % 7;
        int row = index / 7;

        double xc = 100 + col * (DOMINO_SIDE + 20);
        double yc = 100 + row * (DOMINO_SIDE*2 + 40);

        d.xc(xc);
        d.yc(yc);

        index++;
    }
}

void Board::reveal_next_domino(double x, double y) {
    if (m_dominos_shown_num >= 28) {
        return;
    }
    m_dominos[m_dominos_shown_num].validated(false);
    m_dominos[m_dominos_shown_num].xc(x);
    m_dominos[m_dominos_shown_num].yc(y);
    m_pieces.push_back(&m_dominos[m_dominos_shown_num]);
    m_dominos_shown_num++;
}

void Board::restart_game() {
    m_pieces.clear();
    m_pieces.push_back(&m_deck);
    m_dominos_shown_num = 0;
    shuffle_dominos();
}

void Board::draw_magnetic_grid(const CairoContext& cr, int darea_width, int darea_height) const {
    const double grid_spacing = 50.0;

    cr->set_source_rgb(0.8, 0.8, 0.8);

    for (double x = 0; x <= darea_width; x += grid_spacing) {
        cr->move_to(x, 0);
        cr->line_to(x, darea_height);
    }

    for (double y = 0; y <= darea_height; y += grid_spacing) {
        cr->move_to(0, y);
        cr->line_to(darea_width, y);
    }

    cr->set_line_width(1.0);
    cr->stroke();
}

bool Board::validate_domino_position(const Domino& domino) const
{
    const int step = DOMINO_SIDE;
    BoundingBox bbox;
    
    // Compter les dominos validés
    int validated_count = 0;
    for (const auto& d : m_dominos)
        if (d.validated())
            validated_count++;

    if (validated_count == 0)
        return true;
    
    find_bounding_box(bbox);
    
    bbox.m_xmin -= step;
    bbox.m_xmax += step;
    bbox.m_ymin -= step;
    bbox.m_ymax += step;

   
   int gxmin = get_magnet_coord(DOMINO_SIDE/2, bbox.m_xmin);
    int gymin = get_magnet_coord(DOMINO_SIDE/2, bbox.m_ymin);
    int gxmax = get_magnet_coord(DOMINO_SIDE/2, bbox.m_xmax);
    int gymax = get_magnet_coord(DOMINO_SIDE/2, bbox.m_ymax);

    int grid_w = gxmax - gxmin;
    int grid_h = gymax - gymin;

    Grid2D<int> grid(grid_w, grid_h, -1);
    fill_marking_pieces(grid, bbox);

    for (int y = 0; y < grid.height(); ++y) {
        for (int x = 0; x < grid.width(); ++x)
            std::cout << std::setw(3) << grid.data(x, y);
        std::cout << '\n';
    }
    
    if (!marking_is_empty(grid, bbox, domino))
    return false;

    return check_neighbours(grid, bbox, domino);

}

void Board::find_bounding_box(BoundingBox& bbox) const
{
    bool first = true;

    for (const auto& d : m_dominos) {
        if (!d.validated()) continue;

        double xmin = d.xc() - DOMINO_SIDE;
        double xmax = d.xc() + DOMINO_SIDE;
        double ymin = d.yc() - DOMINO_SIDE;
        double ymax = d.yc() + DOMINO_SIDE;

        if (first) {
            bbox.init(xmin, xmax, ymin, ymax);
            first = false;
        } else {
            bbox.update_min(xmin, ymin);
            bbox.update_max(xmax, ymax);
        }
    }
}

void Board::fill_marking_pieces(Grid2D<int>& grid, const BoundingBox& bbox) const
{
    const int step = DOMINO_SIDE / 2;

    int gxmin = get_magnet_coord(step, bbox.m_xmin);
    int gymin = get_magnet_coord(step, bbox.m_ymin);

    for (size_t k = 0; k < m_dominos.size(); ++k) {
        const Domino& d = m_dominos[k];
        if (!d.validated()) continue;

        int angle = d.angle();

        // coin haut-gauche du domino dans la grille locale
        int gx1 = get_magnet_coord(step, d.xb()) - gxmin;
        int gy1 = get_magnet_coord(step, d.yb()) - gymin;

        // astuce officielle de l’énoncé
        int xa = (angle == 180);
        int ya = (angle == 270);
        int xb = (angle == 0);
        int yb = (angle == 90);

        // demi-domino A
        for (int dx = 0; dx < 2; ++dx)
            for (int dy = 0; dy < 2; ++dy)
                grid.set(gx1 + xa*2 + dx,
                         gy1 + ya*2 + dy,
                         static_cast<int>(k));

        // demi-domino B
        for (int dx = 0; dx < 2; ++dx)
            for (int dy = 0; dy < 2; ++dy)
                grid.set(gx1 + xb*2 + dx,
                         gy1 + yb*2 + dy,
                         static_cast<int>(k));
    }
}

bool Board::marking_is_empty(const Grid2D<int>& grid,
                             const BoundingBox& bbox,
                             const Domino& d) const
{
    const int step = DOMINO_SIDE / 2;

    int gxmin = get_magnet_coord(step, bbox.m_xmin);
    int gymin = get_magnet_coord(step, bbox.m_ymin);

    int gx1 = get_magnet_coord(step, d.xb()) - gxmin;
    int gy1 = get_magnet_coord(step, d.yb()) - gymin;

    int angle = d.angle();

    int xa = (angle == 180);
    int ya = (angle == 270);
    int xb = (angle == 0);
    int yb = (angle == 90);

    for (int dx = 0; dx < 2; ++dx)
        for (int dy = 0; dy < 2; ++dy)
            if (grid.data(gx1 + xa*2 + dx,
                          gy1 + ya*2 + dy) != -1)
                return false;

    for (int dx = 0; dx < 2; ++dx)
        for (int dy = 0; dy < 2; ++dy)
            if (grid.data(gx1 + xb*2 + dx,
                          gy1 + yb*2 + dy) != -1)
                return false;

    return true;
}

bool Board::check_neighbours(const Grid2D<int>& grid,
                             const BoundingBox& bbox,
                             const Domino& d) const
{
    const int step = DOMINO_SIDE / 2;

    int gxmin = get_magnet_coord(step, bbox.m_xmin);
    int gymin = get_magnet_coord(step, bbox.m_ymin);

    int gx1 = get_magnet_coord(step, d.xb()) - gxmin;
    int gy1 = get_magnet_coord(step, d.yb()) - gymin;

    int angle = d.angle();

    int xa = (angle == 180);
    int ya = (angle == 270);
    int xb = (angle == 0);
    int yb = (angle == 90);

    int n1 = 0;
    int n2 = 0;

    const int dx8[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    const int dy8[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

    // carré A
    for (int dx = 0; dx < 2; ++dx)
        for (int dy = 0; dy < 2; ++dy) {
            int x = gx1 + xa*2 + dx;
            int y = gy1 + ya*2 + dy;

            for (int k = 0; k < 8; ++k)
                if (grid.data(x + dx8[k], y + dy8[k]) == d.a())
                    n1++;
        }

    // carré B
    for (int dx = 0; dx < 2; ++dx)
        for (int dy = 0; dy < 2; ++dy) {
            int x = gx1 + xb*2 + dx;
            int y = gy1 + yb*2 + dy;

            for (int k = 0; k < 8; ++k)
                if (grid.data(x + dx8[k], y + dy8[k]) == d.b())
                    n2++;
        }

    return (n1 + n2 == 2);
}
