// Copyright 2020 Patrick Müller

#include <gtest/gtest.h>
#include <ncurses.h>
#include <stdlib.h>
#include <iostream>
#include <array>
#include "./GameLogic.h"

TEST(GameLogicTest, moveAndCalculate) {
  GameLogic game;
  game.moveUp();
  for (int zeile = 0; zeile <= 3; zeile++) {
    for (int spalte = 0; spalte <= 3; spalte++) {
      ASSERT_EQ(0, game._field[zeile][spalte]);
    }
  }

  game._field[0][0] = 2;
  game.moveRight();
  ASSERT_EQ(2, game._field[0][3]);
  // game.printField();

  game.moveDown();
  ASSERT_EQ(2, game._field[3][3]);
  // game.printField();

  game._field[3][0] = 2;
  game.moveLeft();
  ASSERT_EQ(4, game._field[3][0]);
  ASSERT_EQ(0, game._field[3][3]);
  // game.printField();

  game._field[2][0] = 4;
  game._field[1][0] = 4;
  game._field[0][0] = 4;
  // game.printField();
  game.moveUp();
  // game.printField();
  ASSERT_EQ(8, game._field[0][0]);
  ASSERT_EQ(8, game._field[1][0]);

  game.moveUp();
  ASSERT_EQ(16, game._field[0][0]);
  ASSERT_EQ(0, game._field[1][0]);
  // game.printField();

  game._field[0][1] = 32;
  // game.printField();
  game.moveRight();
  game.moveRight();
  ASSERT_EQ(32, game._field[0][3]);
  ASSERT_EQ(16, game._field[0][2]);
  // game.printField();

  game.moveDown();
  ASSERT_EQ(32, game._field[3][3]);
  ASSERT_EQ(16, game._field[3][2]);
  // game.printField();

  game._field[3][1] = 8;
  game._field[3][0] = 8;
  // game.printField();
  game.moveLeft();
  ASSERT_EQ(16, game._field[3][0]);
  ASSERT_EQ(16, game._field[3][1]);
  ASSERT_EQ(32, game._field[3][2]);
  // game.printField();

  game.moveRight();
  ASSERT_EQ(32, game._field[3][2]);
  ASSERT_EQ(32, game._field[3][3]);
  // game.printField();

  game._field[0][3] = 2;
  game.moveLeft();
  ASSERT_EQ(2, game._field[0][0]);
  ASSERT_EQ(64, game._field[3][0]);
  // game.printField();
}

// find a way to test this properly
TEST(GameLogicTest, randomSquare) {
  GameLogic game;
  int count = 0;
  for (int i = 0; i <= 3; i++) game._field[i][1] = 8;
  game.randomSquare();
  for (int zeile = 0; zeile <= 3; zeile++) {
    for (int spalte = 0; spalte <= 3; spalte++) {
      if (game._field[zeile][spalte] != 0) {
        count++;
      }
    }
  }
  ASSERT_EQ(5, count);
  game.controlTheGame(261);
  count = 0;
  for (int zeile = 0; zeile <= 3; zeile++) {
    for (int spalte = 0; spalte <= 3; spalte++) {
      if (game._field[zeile][spalte] != 0) {
        count++;
      }
    }
  }
  ASSERT_EQ(6, count);

  for (int zeile = 0; zeile <= 3; zeile++) {
    for (int spalte = 0; spalte <= 3; spalte++) {
      game._field[zeile][spalte] = 0;
    }
  }

  game._field[0][0] = 2;
  count = 0;
  game.controlTheGame(259);
  game.controlTheGame(260);

  for (int zeile = 0; zeile <= 3; zeile++) {
    for (int spalte = 0; spalte <= 3; spalte++) {
      if (game._field[zeile][spalte] != 0) {
        count++;
      }
    }
  }
  ASSERT_EQ(1, count);
}

TEST(GameLogicTest, undo) {
  GameLogic game;
  game._field[0][0] = 2;
  game._states[0] = game._field;
  ASSERT_EQ(0, game._stepCounter);
  game.moveAndCalculate(KEY_DOWN);
  ASSERT_EQ(1, game._stepCounter);
  game.undo(false);
  ASSERT_EQ(2, game._field[0][0]);
}

TEST(GameLogicTest, isLost) {
  GameLogic game;
  game._field[0][0] = 2;
  game._field[0][1] = 4;
  game._field[0][2] = 8;
  game._field[0][3] = 16;
  game._field[1][0] = 32;
  game._field[1][1] = 64;
  game._field[1][2] = 128;
  game._field[1][3] = 256;
  game._field[2][0] = 2;
  game._field[2][1] = 4;
  game._field[2][2] = 8;
  game._field[2][3] = 16;
  game._field[3][0] = 32;
  game._field[3][1] = 64;
  game._field[3][2] = 128;
  game._field[3][3] = 256;

  ASSERT_EQ(true, game.isLost());
  ASSERT_EQ(0, game._stepCounter);
}

TEST(GameLogicTest, findScore) {
  GameLogic game;
  game._field[3][3] = 64;
  game.findScore();
  ASSERT_EQ(64, game._score);
  game._field[0][0] = 128;
  game.findScore();
  ASSERT_EQ(128, game._score);
}
