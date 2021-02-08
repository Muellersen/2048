// Copyright 2020 Patrick Müller

#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include <gtest/gtest.h>
#include <array>
#include <unordered_map>
#include "./Graphic.h"

class GameLogic {
  // here i want to have everything the game needs apart from the graphic
  // or the input. this class will contain the functions that will
  // process the next states or process the user input etc.
  friend class Graphic;
 public:
  // constructor
  GameLogic();

  void printField();

  // this sets up the field as 4x4 (implements the vectors)
  void setupField();

  // this function generates a number ( 2 - 90% and 4 - 10%) at
  // a random square in the field
  void randomSquare();
  FRIEND_TEST(GameLogicTest, randomSquare);

  // this function takes an argument which represents the key arrows
  // and moves the squares with numbers in the given direction
  // according to the rules AND combines identical numbers
  bool moveAndCalculate(const int keyArrows);
  FRIEND_TEST(GameLogicTest, moveAndCalculate);

  // helper functions for moveAndCalculate
  bool moveRight();
  bool moveLeft();
  bool moveUp();
  bool moveDown();

  // this function processes the other inputs like n for restart or ESC
  // for end the game or u for undo the last steps (dependent on the
  // commandline argument x)
  int controlTheGame(const int input);

  // function that gets the x value for undo
  void getX(const int xValue);

  // undo function that undos the last x steps
  void undo(const bool xOrNot);
  FRIEND_TEST(GameLogicTest, undo);

  // this function checks if there still is a possible move to do
  // and if not the function returns TRUE
  bool isLost();
  FRIEND_TEST(GameLogicTest, isLost);

  // function that finds out the score
  void findScore();
  FRIEND_TEST(GameLogicTest, findScore);

 private:
  // this variable is used for counting how many times the squares were
  // moved
  int _stepCounter;

  // this is the field in which everything happens
  std::array<std::array<int, 4>, 4> _field;

  // this is for the score
  int _score;

  // for the undo function
  std::unordered_map<int, std::array<std::array<int, 4>, 4>> _states;

  // commandline argument x for the undofunction
  int _x;

  // this is a counter for the undo function so that there are only x
  // undos possible
  int _undoCounter;
};

#endif  // GAMELOGIC_H_
