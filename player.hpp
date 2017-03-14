#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    Board * board;
    Side side;
    Side oppSide;

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *getBestMove(vector<Move*> moves);
    int gradeMove(Move* move);
    int worstResult(Side theSide, int depth, Board* brd);
    bool isSide(Move* move);
    bool isCorner(Move* move);
    Side nextSide (Side side);
    int grade(Board* b);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
