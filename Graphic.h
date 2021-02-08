// Copyright 2020 Patrick Müller

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <ncurses.h>
#include <gtest/gtest.h>
#include "./GameLogic.h"

class GameLogic;

class Graphic {
  // this contains everything about the graphics of the game

 public:
  // sets up ncurses just like we have done it with the game of life game
  void terminal() const;

  // prints instructions on the window
  void menu() const;

  // takes a number from the game field and sets the init_pair
  int findColor(const int num) const;

  //  this is for printing out the numbers in the correct place of the square
  void printNum(int num, int zeile, int spalte);

  // shows the state with the correct size and colors
  void showState(GameLogic game);
  FRIEND_TEST(GraphicTest, showState);

  // shows the score and the counter of turns
  void showCounterAndScore(GameLogic game) const;

  // shows if game is won
  void isWon(GameLogic game) const;

  // shows if game is over
  void isOver(const bool) const;
};

#endif  // GRAPHIC_H_
