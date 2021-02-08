// Copyright 2020 Patrick Müller

#include <ncurses.h>
#include <getopt.h>
#include <unistd.h>
#include "./Wrapper.h"
#include "./Graphic.h"

// ___________________________________________________________________________
int getInput() {
  int key = getch();
  return key;
}

// ___________________________________________________________________________
void play(int argc, char** argv) {
  GameLogic game;
  Graphic graphic;
  graphic.terminal();
  game.setupField();
  game.getX(parseCommandLineArguments(argc, argv));

  while (true) {
    graphic.menu();
    graphic.showState(game);
    graphic.showCounterAndScore(game);
    graphic.isWon(game);

    if (game.controlTheGame(getInput()) == 0) {
      break;
    }

    if (game.isLost()) {
      graphic.showState(game);
      graphic.showCounterAndScore(game);
      graphic.isOver(true);
      usleep(5000 * 1000);
      break;
    }

    game.findScore();
    clear();
  }
  endwin();
}

// ___________________________________________________________________________
int parseCommandLineArguments(int argc, char** argv) {
  struct option options[] = {
    { "undo-function", 1, NULL, 'x' },
    { NULL, 0, NULL, 0 }
  };

  int result = 0;
  optind = 1;

  while (true) {
    char c = getopt_long(argc, argv, "x:", options, NULL);

    if (c == -1) break;

    switch (c) {
      case 'x':
              result = atoi(optarg);
              break;
    }
  }
  return result;
}
