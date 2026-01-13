# Domino Game (GTKmm + Cairo) 🎲

A desktop **Domino game** implemented in **C++** using **GTKmm 3** for the graphical user interface and **Cairo** for 2D rendering.  
Players can interact with dominos using the mouse (drag & drop), while a **magnetic grid** helps align pieces precisely on the board.

---

## ✨ Features

* **GTKmm-based UI** with a main window and control buttons:

  * **Quit**
  * **Shuffle**
  * **New Game**
* **Cairo rendering** inside a `Gtk::DrawingArea`
* **Magnetic grid** (snap-to-grid behavior)
* **Drag & Drop interaction** using mouse events:

  * Button press / release
  * Motion notify
  * Scroll and keyboard events
* **Deck system** to reveal new dominos
* **Domino validation logic** based on:

  * A 2D marking grid (`Grid2D<int>`)
  * Bounding box computation (`BoundingBox`)
  * Collision detection (empty marking check)
  * Neighbor checking (attachment rules)

---

## Project Structure

Main files in the project:

* `main_window.hpp / main_window.cpp`
  Manages the GTK window, buttons, and all `DrawingArea` callbacks (draw, resize, mouse and keyboard events).

* `board.hpp / board.cpp`
  Core game logic: domino storage, drawing, shuffling, grid marking, validation, and neighbor checking.

* `domino.hpp`
  Domino representation (values, orientation, drawing of pips, validation state).

* `deck.hpp`
  Deck piece that allows revealing new dominos on the board.

* `piece.hpp`
  Base class for all interactive pieces (geometry, hit-testing, magnetic helpers).

* `utils.hpp / utils.tpp`
  Utility components:

  * `CairoContext` alias
  * `BoundingBox`
  * `Grid2D<T>` template
  * `shuffle_vector`
  * `get_magnet_coord`

* `Makefile`
  Build configuration using `pkg-config gtkmm-3.0`.

---

## ✅ Requirements

### Linux (Ubuntu / Debian)

Install the required development tools and libraries:

```bash
sudo apt update
sudo apt install -y g++ make pkg-config libgtkmm-3.0-dev
```

> Cairo is used through **cairomm**, which is pulled in automatically by GTKmm.

---

## 🚀 Build & Run

The project builds an executable named:

➡️ **`domino`**

### 1️⃣ Compile

```bash
make clean all
```

### 2️⃣ Run

```bash
./domino
```

---

## 🕹️ How to Play (Basics)

* Use the **mouse** to click and drag dominos on the board.
* Release the mouse button to place a domino; it will try to align with the **magnetic grid**.
* Use the buttons:

  * **Shuffle**: randomizes and repositions dominos
  * **New Game**: resets the game state
  * **Quit**: exits the application

---

## 🛠️ Developer Notes

* All drawing is performed in:

  ```cpp
  MainWindow::darea_on_draw(const CairoContext& cr)
  ```

* Rendering order:

  1. `Board::draw_magnetic_grid(...)`
  2. `Board::draw_pieces(...)`

* Domino placement validation relies on:

  * `Board::validate_domino_position(...)`
  * `Board::find_bounding_box(...)`
  * `Board::fill_marking_pieces(...)`
  * `Board::marking_is_empty(...)`
  * `Board::check_neighbours(...)`

This design keeps the logic modular, testable, and close to the project specification.

---
## 📄 License
This project was developed for educational purposes (university coursework).  
If you reuse or adapt this code, please credit the authors.

**Authors:** Walid KESBI & ABDELHAKIM NAHILI  
**Date:** 12/11/2025
