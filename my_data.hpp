// Created by Walid KESBI and ABDELHAKIM NAHILI on 12/11/2025.

#pragma once
#include "board.hpp"

class MyData {
public:
    int win_width = 1400;
    int win_height = 1500;
    Board m_board;
    double m_old_pos_x = 0;
    double m_old_pos_y = 0;
    bool m_domino_clicked = false;
    int m_mouse_button_num_pressed = 0;
    

    MyData ();

};