// Created by Walid KESBI and ABDELHAKIM NAHILI on 12/11/2025.

#include "my_data.hpp"
#include "main_window.hpp"


int main (int argc, char** argv)
{
    // Initialise GTK et enlève de argc,argv les options reconnues
    Gtk::Main app (argc, argv);

    MyData my;
    MainWindow main_window (my);

    app.run (main_window.m_window);
}