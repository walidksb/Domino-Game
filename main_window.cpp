// Created by Walid KESBI and ABDELHAKIM NAHILI on 12/11/2025.

#include "main_window.hpp"
#include <iostream>
#include "my_data.hpp"  

MainWindow::MainWindow(MyData& my){
        m_my = &my;

        // Fenêtre
        m_window.set_title ("Domino");
        m_window.set_size_request (my.win_width, my.win_height);
        m_window.add (m_vbox1);
        m_vbox1.pack_start (m_hbox1, false, false, 5);

        // Boutons
        m_hbox1.pack_end (m_button_quit, false, false, 5);
        CONNECT (m_button_quit.signal_clicked, 
            MainWindow::button_quit_on_clicked);
        m_hbox1.pack_start (m_button_shuffle, false, false, 5);
        CONNECT (m_button_shuffle.signal_clicked, 
            MainWindow::button_shuffle_on_clicked);
        m_hbox1.pack_start (m_button_restart, false, false, 5);
        CONNECT (m_button_restart.signal_clicked,
            MainWindow::button_restart_on_clicked);
        
        
        // Zone de Dessin m_darea
        m_vbox1.pack_start (m_frame1, true, true, 2);
        m_frame1.set_border_width (2);
        m_frame1.add (m_darea);
        CONNECT (m_darea.signal_draw, MainWindow::darea_on_draw);

        m_darea.set_events (Gdk::BUTTON_PRESS_MASK |
                            Gdk::BUTTON_RELEASE_MASK |
                            Gdk::POINTER_MOTION_MASK |
                            Gdk::SCROLL_MASK );

        CONNECT (m_darea.signal_button_press_event,
            MainWindow::darea_on_button_press);
        CONNECT (m_darea.signal_button_release_event,
            MainWindow::darea_on_button_release);
        CONNECT (m_darea.signal_motion_notify_event,
            MainWindow::darea_on_motion_notify);
        CONNECT (m_darea.signal_scroll_event,
            MainWindow::darea_on_scroll);
        CONNECT (m_darea.signal_size_allocate,
            MainWindow::darea_on_size_allocate);

        m_darea.set_can_focus (true);
        m_darea.grab_focus();
        CONNECT (m_darea.signal_key_press_event,
            MainWindow::darea_on_key_press);

        m_window.show_all();
};




// Les fonctions qui suivent sont des "callback" c'est-à-dire qu'elles
// seront appelées automatiquement lors d'un événement.
void MainWindow::repaint_darea()
{
    m_darea.queue_draw();   // force darea à se redessiner
};

void MainWindow::button_quit_on_clicked()
{
    Gtk::Main::quit();
};

void MainWindow::button_shuffle_on_clicked()
{
    std::cout << "Bouton 'Mélanger' cliqué" << std::endl;
    m_my->m_board.shuffle_dominos();
    m_my->m_board.align_dominos_on_grid();
    repaint_darea();
};

void MainWindow::button_restart_on_clicked()
{
    std::cout << "Bouton 'Recommencer' cliqué" << std::endl;
    m_my->m_board.restart_game();
    repaint_darea();
};

// Tous les dessins sont faits ici, à partir du contexte Cairo cr.
    // Pour faire un dessin, on crée un chemin, puis on appelle 
    // - stroke() : dessine et supprime le chemin courant,
    // - fill()   : remplit et supprime le chemin courant,
    // - stroke_preserve() : dessine et conserve le chemin courant,
    // - fill_preserve()   : remplit et conserve le chemin courant.
    bool MainWindow::darea_on_draw (const CairoContext& cr)
    {
        m_my->m_board.draw_magnetic_grid(cr, m_darea_width, m_darea_height);   
        m_my->m_board.draw_pieces(cr);
        
        return true;  // événement capté
    };
    
    
    bool MainWindow::darea_on_button_press (GdkEventButton *event)
    {

            //piece
            bool raise_on_top = false;
            if(m_my->m_board.find_piece_clicked(event->x, event->y)){
                m_my->m_board.m_pieces[m_my->m_board.piece_num_clicked()]->on_button_press(event->x, event->y, event->button, raise_on_top);
                if (raise_on_top) {
                    m_my->m_board.move_piece_to_top(m_my->m_board.piece_num_clicked());
                    m_my->m_board.piece_num_clicked(0);
                    repaint_darea();
                }
                m_my->m_old_pos_x = event->x;
                m_my->m_old_pos_y = event->y;
            }
            
            m_darea.grab_focus();  // prend le focus clavier chaque fois qu'on clique
            m_my->m_mouse_button_num_pressed = event->button;  // bouton pressé
            return true;  // événement capté
    };

bool MainWindow::darea_on_button_release (GdkEventButton *event)
{

        //piece
        if(m_my->m_board.piece_num_clicked() != -1){
            m_my->m_board.m_pieces[m_my->m_board.piece_num_clicked()]->on_button_release(event->x, event->y, event->button);
            repaint_darea();
            m_my->m_board.piece_num_clicked(-1);
        }

        m_my->m_mouse_button_num_pressed = 0;  // bouton relaché
        return true;  // événement capté
};

bool MainWindow::darea_on_motion_notify (GdkEventMotion *event)
{
 
        //piece
        if (m_my->m_mouse_button_num_pressed > 0 && m_my->m_board.piece_num_clicked() != -1){
            std::cout << "Moving piece " << m_my->m_board.piece_num_clicked() << std::endl;
            m_my->m_board.m_pieces[m_my->m_board.piece_num_clicked()]->on_motion_notify(event->x, event->y, m_my->m_old_pos_x, m_my->m_old_pos_y, m_my->m_mouse_button_num_pressed);
            repaint_darea();
            m_my->m_old_pos_x = event->x;
            m_my->m_old_pos_y = event->y;
        } 
   
        return true;  // événement capté
};

bool MainWindow::darea_on_scroll (GdkEventScroll *event)
{
        std::cout << __func__ << " " << event->direction << " " <<
            event->x << " " << event->y << std::endl;

        return true;  // événement capté
};

void MainWindow::darea_on_size_allocate (Gtk::Allocation& allocation)
{
    m_darea_width = allocation.get_width();
    m_darea_height = allocation.get_height();

    std::cout << __func__ << " " << allocation.get_width() << " " << 
        allocation.get_height() << std::endl;
};

bool MainWindow::darea_on_key_press (GdkEventKey *event)
{
        switch (event->keyval) {
            case GDK_KEY_a :
                std::cout << "Touche a pressée" << std::endl;
                break;

            case GDK_KEY_KP_Add :
            case GDK_KEY_plus :
                std::cout << "Touche + pressée" << std::endl;
                break;

            default : std::cout << "Touche pressée : GDK_KEY_" << 
                gdk_keyval_name(event->keyval) << std::endl;
        }
        return true;  // événement capté
};

