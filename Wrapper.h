// Copyright 2020 Patrick Müller

#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <gtest/gtest.h>

// this function is for getting the keys
int getInput();

// this function is for playing the game and takes commandline args for
// the undo function
void play(int argc, char** argv);

// reads the commandline args there wont be an error if there is
// no argument as its usually played without an undo function
int parseCommandLineArguments(int argc, char** argv);

#endif  // WRAPPER_H_
