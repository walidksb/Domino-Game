// Created by Walid KESBI and ABDELHAKIM NAHILI on 12/11/2025.

#pragma once
#include "utils.hpp"
#include "my_data.hpp"

class MainWindow {
public:
    Gtk::Window m_window;
    Gtk::VBox m_vbox1;
    Gtk::HBox m_hbox1;
    Gtk::Button m_button_quit {"Quit"};
    Gtk::Button m_button_shuffle {"Shuffle"};
    Gtk::Button m_button_restart {"New Game"};
    Gtk::Frame m_frame1;
    Gtk::DrawingArea m_darea;
    MyData *m_my = nullptr;
    int m_darea_width;
    int m_darea_height;

    MainWindow(MyData& my);
    bool darea_on_button_press (GdkEventButton *event);
    void repaint_darea();
    void button_quit_on_clicked();
    void button_shuffle_on_clicked();
    void button_restart_on_clicked();
    bool darea_on_draw (const CairoContext& cr);
    bool darea_on_button_release (GdkEventButton *event);
    bool darea_on_motion_notify (GdkEventMotion *event);
    bool darea_on_scroll (GdkEventScroll *event);
    void darea_on_size_allocate (Gtk::Allocation& allocation);
    bool darea_on_key_press (GdkEventKey *event);
};  // MainWindow


