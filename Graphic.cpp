// Copyright 2020 Patrick Müller

#include <ncurses.h>
#include <vector>
#include "./Graphic.h"
#include "./GameLogic.h"

using std::vector;

class GameLogic;

// ___________________________________________________________________________
void Graphic::terminal() const {
  initscr();              // Initializiation.
  cbreak();               // Don't wait for RETURN.
  noecho();               // Don't echo key presses on screen.
  curs_set(false);        // Don't show the cursor.
  nodelay(stdscr, false);  // Don't wait until key pressed.
  keypad(stdscr, true);   // For KEY_LEFT, KEY_UP, etc.
  start_color();          // For the colors of the game

  // initialize all colorpairs

  init_pair(99, 240, 240);  // for the border
  init_pair(1, 0, 0);  // case 0
  init_pair(2, 188, 0);  // case 2
  init_pair(3, 187, 0);  // case 4
  init_pair(4, 186, 0);  // case 8
  init_pair(5, 185, 0);  // case 16
  init_pair(6, 184, 0);  // case 32
  init_pair(7, 118, 0);  // case 64
  init_pair(8, 119, 0);  // case 128
  init_pair(9, 120, 0);  // case 256
  init_pair(10, 121, 0);  // case 512
  init_pair(11, 122, 0);  // case 1024
  init_pair(12, 123, 0);  // case 2048
}

// ___________________________________________________________________________
void Graphic::menu() const {
  // border around 2048
  attron(A_REVERSE);
  attron(COLOR_PAIR(8));
  for (int i = 0; i <= 7; i++) {
    for (int j = 0; j <= 2; j++) {
      mvprintw(1 + j, 97 + i, " ");
    }
  }
  attroff(A_REVERSE);
  attroff(COLOR_PAIR(8));

  mvprintw(2, 99, "2048");

  mvprintw(8, 86, "ESC - End the game");
  mvprintw(9, 86, "n   - Restart the game");
  mvprintw(10, 86, "u   - Undo the last step");
  mvprintw(6, 86, "Use the Arrows for movement");
  mvprintw(40, 79, "by Patrick Müller");
}

// ___________________________________________________________________________
int Graphic::findColor(const int num) const {
  switch (num) {
    case 0:
            return 1;
    case 2:
            return 2;
    case 4:
            return 3;
    case 8:
            return 4;
    case 16:
            return 5;
    case 32:
            return 6;
    case 64:
            return 7;
    case 128:
            return 8;
    case 256:
            return 9;
    case 512:
            return 10;
    case 1024:
            return 11;
    case 2048:
            return 12;
  }
  return 0;
}

// ___________________________________________________________________________
void Graphic::printNum(int num, int zeile, int spalte) {
  vector<int> queue;
  float tempNum;
  int number = num;
  int sizeOfQueue;

  // here i separate the numbers into digits
  while (true) {
      if (number <= 0) break;

      tempNum = static_cast<float>(number) / 10;
      tempNum = (tempNum - (number / 10)) * 10;
      queue.push_back(static_cast<int>(tempNum + 0.5));
      number = number / 10;
    }

  // the digits are stored in a vector
  sizeOfQueue = queue.size();

  // this big for loops are used for printing the squares correctly
  // so that any number that can be achieved while playing, will be
  // printed correcly on the window
  // here i separated the number in its own digits and then i
  // printed them digit by digit on the square so the numbers
  // are always centered no matter how many digits (up to 7 digits)
  // so the size of the vector queue decides what will be done to
  // print the square correctly
  for (int i = 0; i <= 8; i++) {
    for (int j = 0; j <= 4; j++) {
      if (num < 10) {
        if (i == 4 && j == 2) {
          mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                   num);
        } else {
          mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, " ");
        }
      } else {
        if (sizeOfQueue == 2) {
          if (i == 4 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 5 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, " ");
          }
        } else if (sizeOfQueue == 3) {
          if (i == 3 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 4 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 5 && j == 2) {
             mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
          } else {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, " ");
          }
        } else if (sizeOfQueue == 4) {
          if (i == 3 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 4 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 5 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 6 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, " ");
          }
        } else if (sizeOfQueue == 5) {
          if (i == 2 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 3 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 4 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 5 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 6 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, " ");
          }
        } else if (sizeOfQueue == 6) {
          if (i == 2 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 3 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 4 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 5 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                    queue.back());
            queue.pop_back();
          } else if (i == 6 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 7 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, " ");
          }
        } else if (sizeOfQueue == 7) {
          if (i == 1 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 2 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 3 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 4 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 5 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 6 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else if (i == 7 && j == 2) {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, "%d",
                     queue.back());
            queue.pop_back();
          } else {
            mvprintw(16 + zeile * 6 + j, 80 + spalte * 11 + i, " ");
          }
        }
      }
    }
  }
}

// ___________________________________________________________________________
void Graphic::showState(GameLogic game) {
  // shows state in correct size and color

  // this is the border
  attron(A_REVERSE);
  attron(COLOR_PAIR(99));
  for (int x = 0; x <= 45; x++) {
    for (int y = 0; y <= 24; y++) {
      mvprintw(y + 15, x + 78, " ");
    }
  }
  attroff(COLOR_PAIR(99));
  attroff(A_REVERSE);

  // the for loops are used to get the 4x4 field
  for (int zeile = 0; zeile <= 3; zeile++) {
    for (int spalte = 0; spalte <= 3; spalte++) {
      // findcolor determines the color of the squares according to the number
      attron(COLOR_PAIR(findColor(game._field[zeile][spalte])));
      attron(A_REVERSE);

      // prints the square with the number
      printNum(game._field[zeile][spalte], zeile, spalte);

      attroff(COLOR_PAIR(findColor(game._field[zeile][spalte])));
      attroff(A_REVERSE);
    }
  }
}

// ___________________________________________________________________________
void Graphic::showCounterAndScore(GameLogic game) const {
  mvprintw(14, 83, "Your Steps: %d  Your Score: %d Undos: %d",
           game._stepCounter, game._score, game._undoCounter);
}

// ___________________________________________________________________________
void Graphic::isWon(GameLogic game) const {
  if (game._score == 2048) {
    mvprintw(13, 86 , "You Won!");
    refresh();
  }
}

// ___________________________________________________________________________
void Graphic::isOver(const bool lost) const {
  if (lost) {
    mvprintw(12, 86, "The Game is over!");
    refresh();
  }
}

