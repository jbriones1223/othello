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

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
