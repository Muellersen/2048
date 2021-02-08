// Copyright 2020 Patrick Müller

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unordered_map>
#include <array>
#include <vector>
#include <iostream>
#include "./GameLogic.h"

using std::vector;
using std::array;
using std::cout;
using std::endl;

// ___________________________________________________________________________
GameLogic::GameLogic() {
  _stepCounter = 0;
  _score = 0;
  for (int zeile = 0; zeile <= 3; zeile++) {
    for (int spalte = 0; spalte <= 3; spalte++) {
      _field[zeile][spalte] = 0;
    }
  }
  _states[_stepCounter] = _field;
}

// ___________________________________________________________________________
void GameLogic::printField() {
  // was used for testing
  cout << endl;
  cout << _field[0][0] << " " << _field[0][1] << " " << _field[0][2];
  cout << " " << _field[0][3] << endl;
  cout << _field[1][0] << " " << _field[1][1] << " ";
  cout << _field[1][2] << " " << _field[1][3] << endl;
  cout << _field[2][0] << " " << _field[2][1] << " ";
  cout << _field[2][2] << " " << _field[2][3] << endl;
  cout << _field[3][0] << " " << _field[3][1] << " ";
  cout << _field[3][2] << " " << _field[3][3] << endl;
}

// ___________________________________________________________________________
void GameLogic::setupField() {
  randomSquare();
  randomSquare();
  _states[0] = _field;
}

// ___________________________________________________________________________
void GameLogic::randomSquare() {
  // 90% 2 and 10% 4 at a random place in the field
  // finds the empty field and then selects a random one
  time_t timer;
  srand48(time(&timer));
  vector<array<int, 2>> squaresLeft;

  for (int zeile = 0; zeile <= 3; zeile++) {
    for (int spalte = 0; spalte  <= 3; spalte++) {
      if (_field[zeile][spalte] == 0) {
        array<int, 2> temp;
        temp[0] = zeile;
        temp[1] = spalte;
        squaresLeft.push_back(temp);
      }
    }
  }
  int randIndex = lrand48() % squaresLeft.size();
  srand48(time(&timer));
  if (drand48() <= 0.9) {
    _field[squaresLeft[randIndex][0]][squaresLeft[randIndex][1]] = 2;
  } else {
    _field[squaresLeft[randIndex][0]][squaresLeft[randIndex][1]] = 4;
  }
}

// now the move functions will show up
// each function does the same but in an other direction
// for up and down the functions iterate through the columns
// for left and right the functions iterate through the rows
// for each row or column it counts how many of the squares are not 0
// then it decides what to do depending on the count and how they were
// positioned and what numbers they had
// if there was no move possible a counter goes up and if the counter
// is 4 for the 4 rows or colums then the function will return false
// because no move was possible

// ___________________________________________________________________________
bool GameLogic::moveRight() {
  int counter = 0;
  // check how many squares are not 0
  for (int zeile = 0; zeile <= 3; zeile++) {
    int count = 0;
    for (int spalte = 0; spalte <= 3; spalte++) {
      if (_field[zeile][spalte] != 0) {
        count++;
      }
    }

    if (count == 0) counter++;  // there is nothing to move

    // if there is one square with a number
    if (count == 1) {
      bool moved = false;
      for (int spalte = 0; spalte <= 2; spalte++) {
        if (_field[zeile][spalte] != 0) {
          _field[zeile][3] = _field[zeile][spalte];
          _field[zeile][spalte] = 0;
          moved = true;
        }
      }
      if (!moved) counter++;  // nothing moved
      continue;
    }

    // 2 squares
    if (count == 2) {
      array<int, 2> positions;
      int num = 0;
      for (int spalte = 0; spalte <= 3; spalte++) {
        if (_field[zeile][spalte] != 0) {
          positions[num] = _field[zeile][spalte];
          num++;
        }
      }
      // if the 2 squares are the same
      if (positions[0] == positions[1]) {
        _field[zeile][3] = positions[0] + positions[1];
        _field[zeile][0] = 0;
        _field[zeile][1] = 0;
        _field[zeile][2] = 0;
      } else if (positions[1] == _field[zeile][3]
                 && positions[0] == _field[zeile][2]) {  // nothing moved
        counter++;
      } else {  // if they are different
        _field[zeile][2] = positions[0];
        _field[zeile][3] = positions[1];
        _field[zeile][0] = 0;
        _field[zeile][1] = 0;
      }
      continue;
    }

    // if there are 3 squares
    if (count == 3) {
      array<int, 3> positions;
      int num = 0;
      for (int spalte = 0; spalte <= 3; spalte++) {
        if (_field[zeile][spalte] != 0) {
          positions[num] = _field[zeile][spalte];
          num++;
        }
      }
      // according to the rules the squares on the right have to be checked
      // first
      if (positions[1] == positions[2]) {
        _field[zeile][3] = positions[1] + positions[2];
        _field[zeile][2] = positions[0];
        _field[zeile][1] = 0;
        _field[zeile][0] = 0;
      } else if (positions[0] == positions[1]) {
        _field[zeile][3] = positions[2];
        _field[zeile][2] = positions[0] + positions[1];
        _field[zeile][1] = 0;
        _field[zeile][0] = 0;
      } else if (positions[2] == _field[zeile][3]
                 && positions[1] == _field[zeile][2]
                 && positions[0] == _field[zeile][1]) {  // nothing moved
        counter++;
      } else {
        _field[zeile][3] = positions[2];
        _field[zeile][2] = positions[1];
        _field[zeile][1] = positions[0];
        _field[zeile][0] = 0;
      }
      continue;
    }

    // 4 squares
    if (count == 4) {
      if (_field[zeile][0] == _field[zeile][1]
          && _field[zeile][2] == _field[zeile][3]) {
        _field[zeile][3] = _field[zeile][2] + _field[zeile][3];
        _field[zeile][2] = _field[zeile][0] + _field[zeile][1];
        _field[zeile][1] = 0;
        _field[zeile][0] = 0;
      } else if (_field[zeile][2] == _field[zeile][3]) {
        _field[zeile][3] = _field[zeile][2] + _field[zeile][3];
        _field[zeile][2] = _field[zeile][1];
        _field[zeile][1] = _field[zeile][0];
        _field[zeile][0] = 0;
      } else if (_field[zeile][1] == _field[zeile][2]) {
        _field[zeile][2] = _field[zeile][1] + _field[zeile][2];
        _field[zeile][1] = _field[zeile][0];
        _field[zeile][0] = 0;
      } else if (_field[zeile][0] == _field[zeile][1]) {
        _field[zeile][1] = _field[zeile][0] + _field[zeile][1];
        _field[zeile][0] = 0;
      } else {
        counter++;  // nothing moved
      }
      continue;
    }
  }
  if (counter >= 4) return false;  // nothing moved
  return true;
}

// ___________________________________________________________________________
bool GameLogic::moveLeft() {
  int counter = 0;
  // check how many squares are not 0
  for (int zeile = 0; zeile <= 3; zeile++) {
    int count = 0;
    for (int spalte = 0; spalte <= 3; spalte++) {
      if (_field[zeile][spalte] != 0) {
        count++;
      }
    }

    if (count == 0) counter++;  // there is nothing to move

    // if there is one square with a number
    if (count == 1) {
      bool moved = false;
      for (int spalte = 1; spalte <= 3; spalte++) {
        if (_field[zeile][spalte] != 0) {
          _field[zeile][0] = _field[zeile][spalte];
          _field[zeile][spalte] = 0;
          moved = true;
        }
      }
      if (!moved) counter++;  // nothing moved
      continue;
    }

    // 2 squares
    if (count == 2) {
      array<int, 2> positions;
      int num = 0;
      for (int spalte = 0; spalte <= 3; spalte++) {
        if (_field[zeile][spalte] != 0) {
          positions[num] = _field[zeile][spalte];
          num++;
        }
      }
      // if the 2 squares are the same
      if (positions[0] == positions[1]) {
        _field[zeile][0] = positions[0] + positions[1];
        _field[zeile][3] = 0;
        _field[zeile][1] = 0;
        _field[zeile][2] = 0;
      } else if (positions[0] == _field[zeile][0]
                 && positions[1] == _field[zeile][1]) {  // nothing moved
        counter++;
      } else {  // if they are different
        _field[zeile][0] = positions[0];
        _field[zeile][1] = positions[1];
        _field[zeile][2] = 0;
        _field[zeile][3] = 0;
      }
      continue;
    }

    // if there are 3 squares
    if (count == 3) {
      array<int, 3> positions;
      int num = 0;
      for (int spalte = 0; spalte <= 3; spalte++) {
        if (_field[zeile][spalte] != 0) {
          positions[num] = _field[zeile][spalte];
          num++;
        }
      }
      // according to the rules the squares on the left have to be checked
      // first
      if (positions[0] == positions[1]) {
        _field[zeile][0] = positions[0] + positions[1];
        _field[zeile][1] = positions[2];
        _field[zeile][2] = 0;
        _field[zeile][3] = 0;
      } else if (positions[1] == positions[2]) {
        _field[zeile][0] = positions[0];
        _field[zeile][1] = positions[1] + positions[2];
        _field[zeile][2] = 0;
        _field[zeile][3] = 0;
      } else if (positions[0] == _field[zeile][0]
                 && positions[1] == _field[zeile][1]
                 && positions[2] == _field[zeile][2]) {  // nothing moved
        counter++;
      } else {
        _field[zeile][0] = positions[0];
        _field[zeile][1] = positions[1];
        _field[zeile][2] = positions[2];
        _field[zeile][3] = 0;
      }
      continue;
    }

    // 4 squares
    if (count == 4) {
      if (_field[zeile][0] == _field[zeile][1]
          && _field[zeile][2] == _field[zeile][3]) {
        _field[zeile][0] = _field[zeile][0] + _field[zeile][1];
        _field[zeile][1] = _field[zeile][2] + _field[zeile][3];
        _field[zeile][2] = 0;
        _field[zeile][3] = 0;
      } else if (_field[zeile][0] == _field[zeile][1]) {
        _field[zeile][0] = _field[zeile][0] + _field[zeile][1];
        _field[zeile][1] = _field[zeile][2];
        _field[zeile][2] = _field[zeile][3];
        _field[zeile][3] = 0;
      } else if (_field[zeile][1] == _field[zeile][2]) {
        _field[zeile][1] = _field[zeile][1] + _field[zeile][2];
        _field[zeile][2] = _field[zeile][3];
        _field[zeile][3] = 0;
      } else if (_field[zeile][2] == _field[zeile][3]) {
        _field[zeile][2] = _field[zeile][2] + _field[zeile][3];
        _field[zeile][3] = 0;
      } else {
        counter++;  // nothing moved
      }
    }
    continue;
  }
  if (counter == 4) return false;  // nothing moved
  return true;
}

// ___________________________________________________________________________
bool GameLogic::moveUp() {
  int counter = 0;
  // check how many squares are not 0
  for (int spalte = 0; spalte <= 3; spalte++) {
    int count = 0;
    for (int zeile = 0; zeile <= 3; zeile++) {
      if (_field[zeile][spalte] != 0) {
        count++;
      }
    }

    if (count == 0) counter++;  // there is nothing to move

    // if there is one square with a number
    if (count == 1) {
      bool moved = false;
      for (int zeile = 1; zeile <= 3; zeile++) {
        if (_field[zeile][spalte] != 0) {
          _field[0][spalte] = _field[zeile][spalte];
          _field[zeile][spalte] = 0;
          moved = true;
        }
      }
      if (!moved) counter++;  // nothing moved
      continue;
    }

    // 2 squares
    if (count == 2) {
      array<int, 2> positions;
      int num = 0;
      for (int zeile = 0; zeile <= 3; zeile++) {
        if (_field[zeile][spalte] != 0) {
          positions[num] = _field[zeile][spalte];
          num++;
        }
      }
      // if the 2 squares are the same
      if (positions[0] == positions[1]) {
        _field[0][spalte] = positions[0] + positions[1];
        _field[3][spalte] = 0;
        _field[1][spalte] = 0;
        _field[2][spalte] = 0;
      } else if (positions[0] == _field[0][spalte]
                 && positions[1] == _field[1][spalte]) {  // nothing moved
        counter++;
      } else {  // if they are different
        _field[1][spalte] = positions[1];
        _field[0][spalte] = positions[0];
        _field[3][spalte] = 0;
        _field[2][spalte] = 0;
      }
      continue;
    }

    // if there are 3 squares
    if (count == 3) {
      array<int, 3> positions;
      int num = 0;
      for (int zeile = 0; zeile <= 3; zeile++) {
        if (_field[zeile][spalte] != 0) {
          positions[num] = _field[zeile][spalte];
          num++;
        }
      }
      // according to the rules the squares on the upper have to be checked
      // first
      if (positions[0] == positions[1]) {
        _field[0][spalte] = positions[0] + positions[1];
        _field[1][spalte] = positions[2];
        _field[2][spalte] = 0;
        _field[3][spalte] = 0;
      } else if (positions[1] == positions[2]) {
        _field[0][spalte] = positions[0];
        _field[1][spalte] = positions[1] + positions[2];
        _field[2][spalte] = 0;
        _field[3][spalte] = 0;
      } else if (positions[0] == _field[0][spalte]
                 && positions[1] == _field[1][spalte]
                 && positions[2] == _field[2][spalte]) {  // nothing moved
        counter++;
      } else {
        _field[0][spalte] = positions[0];
        _field[1][spalte] = positions[1];
        _field[2][spalte] = positions[2];
        _field[3][spalte] = 0;
      }
      continue;
    }

    // 4 squares
    if (count == 4) {
      if (_field[0][spalte] == _field[1][spalte]
          && _field[2][spalte] == _field[3][spalte]) {
        _field[0][spalte] = _field[0][spalte] + _field[1][spalte];
        _field[1][spalte] = _field[2][spalte] + _field[3][spalte];
        _field[2][spalte] = 0;
        _field[3][spalte] = 0;
      } else if (_field[0][spalte] == _field[1][spalte]) {
        _field[0][spalte] = _field[0][spalte] + _field[1][spalte];
        _field[1][spalte] = _field[2][spalte];
        _field[2][spalte] = _field[3][spalte];
        _field[3][spalte] = 0;
      } else if (_field[1][spalte] == _field[2][spalte]) {
        _field[1][spalte] = _field[1][spalte] + _field[2][spalte];
        _field[2][spalte] = _field[3][spalte];
        _field[3][spalte] = 0;
      } else if (_field[2][spalte] == _field[3][spalte]) {
        _field[2][spalte] = _field[2][spalte] + _field[3][spalte];
        _field[3][spalte] = 0;
      } else {
        counter++;  // nothing moved
      }
    }
    continue;
  }
  if (counter == 4) return false;  // nothing moved
  return true;
}

// ___________________________________________________________________________
bool GameLogic::moveDown() {
  int counter = 0;
  // check how many squares are not 0
  for (int spalte = 0; spalte <= 3; spalte++) {
    int count = 0;
    for (int zeile = 0; zeile <= 3; zeile++) {
      if (_field[zeile][spalte] != 0) {
        count++;
      }
    }

    if (count == 0) counter++;  // nothing to move there

    // if there is one square with a number
    if (count == 1) {
      bool moved = false;
      for (int zeile = 0; zeile <= 2; zeile++) {
        if (_field[zeile][spalte] != 0) {
          _field[3][spalte] = _field[zeile][spalte];
          _field[zeile][spalte] = 0;
          moved = true;
        }
      }
      if (!moved) counter++;  // nothing moved
      continue;
    }

    // 2 squares
    if (count == 2) {
      array<int, 2> positions;
      int num = 0;
      for (int zeile = 0; zeile <= 3; zeile++) {
        if (_field[zeile][spalte] != 0) {
          positions[num] = _field[zeile][spalte];
          num++;
        }
      }
      // if the 2 squares are the same
      if (positions[0] == positions[1]) {
        _field[3][spalte] = positions[0] + positions[1];
        _field[0][spalte] = 0;
        _field[1][spalte] = 0;
        _field[2][spalte] = 0;
      } else if (positions[1] == _field[3][spalte]
                 && positions[0] == _field[2][spalte]) {  // nothing moved
        counter++;
      } else {  // if they are different
        _field[3][spalte] = positions[1];
        _field[2][spalte] = positions[0];
        _field[1][spalte] = 0;
        _field[0][spalte] = 0;
      }
      continue;
    }

    // if there are 3 squares
    if (count == 3) {
      array<int, 3> positions;
      int num = 0;
      for (int zeile = 0; zeile <= 3; zeile++) {
        if (_field[zeile][spalte] != 0) {
          positions[num] = _field[zeile][spalte];
          num++;
        }
      }
      // according to the rules the squares on the lower have to be checked
      // first
      if (positions[1] == positions[2]) {
        _field[3][spalte] = positions[1] + positions[2];
        _field[2][spalte] = positions[0];
        _field[1][spalte] = 0;
        _field[0][spalte] = 0;
      } else if (positions[0] == positions[1]) {
        _field[3][spalte] = positions[2];
        _field[2][spalte] = positions[0] + positions[1];
        _field[1][spalte] = 0;
        _field[0][spalte] = 0;
      } else if (positions[2] == _field[3][spalte]
                 && positions[1] == _field[2][spalte]
                 && positions[0] == _field[1][spalte]) {  // nothing moved
        counter++;
      } else {
        _field[3][spalte] = positions[2];
        _field[2][spalte] = positions[1];
        _field[1][spalte] = positions[0];
        _field[0][spalte] = 0;
      }
      continue;
    }
    // 4 squares
    if (count == 4) {
      if (_field[0][spalte] == _field[1][spalte]
          && _field[2][spalte] == _field[3][spalte]) {
        _field[3][spalte] = _field[2][spalte] + _field[3][spalte];
        _field[2][spalte] = _field[0][spalte] + _field[1][spalte];
        _field[1][spalte] = 0;
        _field[0][spalte] = 0;
      } else if (_field[2][spalte] == _field[3][spalte]) {
        _field[3][spalte] = _field[2][spalte] + _field[3][spalte];
        _field[2][spalte] = _field[1][spalte];
        _field[1][spalte] = _field[0][spalte];
        _field[0][spalte] = 0;
      } else if (_field[1][spalte] == _field[2][spalte]) {
        _field[2][spalte] = _field[1][spalte] + _field[2][spalte];
        _field[1][spalte] = _field[0][spalte];
        _field[0][spalte] = 0;
      } else if (_field[0][spalte] == _field[1][spalte]) {
        _field[1][spalte] = _field[0][spalte] + _field[1][spalte];
        _field[0][spalte] = 0;
      } else {
        counter++;  // nothing moved
      }
    }
    continue;
  }
  if (counter == 4) return false;  // nothing moved
  return true;
}


// ___________________________________________________________________________
bool GameLogic::moveAndCalculate(const int keyArrows) {
  // returns false if the move was not possible for the isLost() function
  bool moveOrNot1 = false;
  bool moveOrNot2 = false;
  bool moveOrNot3 = false;
  bool moveOrNot4 = false;
  switch (keyArrows) {
    case 259:
           moveOrNot1 = moveUp();
           if (moveOrNot1) {
             _stepCounter++;
             randomSquare();
             for (int zeile = 0; zeile <= 3; zeile++) {
               for (int spalte = 0; spalte <= 3; spalte++) {
                 _states[_stepCounter][zeile][spalte] = _field[zeile][spalte];
               }
             }
           }
           break;

    case 258:
           moveOrNot2 = moveDown();
           if (moveOrNot2) {
             _stepCounter++;
             randomSquare();
             for (int zeile = 0; zeile <= 3; zeile++) {
               for (int spalte = 0; spalte <= 3; spalte++) {
                 _states[_stepCounter][zeile][spalte] = _field[zeile][spalte];
               }
             }
           }
           break;

    case 261:
           moveOrNot3 = moveRight();
           if (moveOrNot3) {
             _stepCounter++;
             randomSquare();
             for (int zeile = 0; zeile <= 3; zeile++) {
               for (int spalte = 0; spalte <= 3; spalte++) {
                 _states[_stepCounter][zeile][spalte] = _field[zeile][spalte];
               }
             }
           }
           break;

    case 260:
           moveOrNot4 = moveLeft();
           if (moveOrNot4) {
             _stepCounter++;
             randomSquare();
             for (int zeile = 0; zeile <= 3; zeile++) {
               for (int spalte = 0; spalte <= 3; spalte++) {
                 _states[_stepCounter][zeile][spalte] = _field[zeile][spalte];
               }
             }
           }
           break;
  }

  // if nothing is moveable return false for the isLost() function
  if (!moveOrNot1 && !moveOrNot2 && !moveOrNot3 && !moveOrNot4) {
    return false;
  } else {
    return true;
  }
}

// ___________________________________________________________________________
int GameLogic::controlTheGame(const int input) {
  // controller
  switch (input) {
    case (110):
            _stepCounter = 0;
            _score = 0;
            for (int zeile = 0; zeile <= 3; zeile++) {
              for (int spalte = 0; spalte <= 3; spalte++) {
                _field[zeile][spalte] = 0;
              }
            }
            _undoCounter = 0;
            randomSquare();
            randomSquare();
            break;

    case (27):
            return 0;  // will be interpreted in the wrapper class as restart
            break;
    case (117):
            undo(true);
            return 1;
    case (259):
            if (moveAndCalculate(259)) {
              if (_undoCounter < _x) _undoCounter++;  // fills up the undos
            }  // Key up
            break;
    case (258):
            if (moveAndCalculate(258)) {
              if (_undoCounter < _x) _undoCounter++;  // fills up the undos
            }  // Key down
            break;
    case (260):
            if (moveAndCalculate(260)) {
              if (_undoCounter < _x) _undoCounter++;  // fills up the undos
            }  // Key left
            break;
    case (261):
            if (moveAndCalculate(261)) {
              if (_undoCounter < _x) _undoCounter++;  // fills up the undos
            }  // Key right
            break;
  }
  return 1;
}

// ___________________________________________________________________________
void GameLogic::getX(const int xValue) {
  _x = xValue;
  _undoCounter = 0;
}

// ___________________________________________________________________________
void GameLogic::undo(const bool xOrNot) {
  if (xOrNot && _stepCounter - 1 >= 0 && _undoCounter > 0) {
    for (int zeile = 0; zeile <= 3; zeile++) {
      for (int spalte = 0; spalte <= 3; spalte++) {
        _field[zeile][spalte] = _states[_stepCounter - 1][zeile][spalte];
      }
    }
    _undoCounter--;
    _stepCounter--;
  } else if (!xOrNot && _stepCounter - 1 >= 0) {
    for (int zeile = 0; zeile <= 3; zeile++) {
      for (int spalte = 0; spalte <= 3; spalte++) {
        _field[zeile][spalte] = _states[_stepCounter - 1][zeile][spalte];
      }
    }
    _stepCounter--;
  }
}

// ___________________________________________________________________________
bool GameLogic::isLost() {
  // find out if the game is lost
  int counter = 0;
  if (!moveAndCalculate(261)) {  // Key right
    counter++;
  } else {
    undo(false);
  }

  if (!moveAndCalculate(260)) {  // Key left
    counter++;
  } else {
    undo(false);
  }

  if (!moveAndCalculate(259)) {  // Key up
    counter++;
  } else {
    undo(false);
  }

  if (!moveAndCalculate(258)) {  // Key down
    counter++;
  } else {
    undo(false);
  }

  if (counter == 4) {
    return true;
  } else {
    return false;
  }
}

// ___________________________________________________________________________
void GameLogic::findScore() {
  _score = 0;
  for (int zeile = 0; zeile <= 3; zeile++) {
    for (int spalte = 0; spalte <= 3; spalte++) {
      if (_field[zeile][spalte] > _score) {
        _score = _field[zeile][spalte];
      }
    }
  }
}
