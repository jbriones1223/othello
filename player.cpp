/**
 * @file player.cpp
 * @author Jack Briones <<jbriones@caltech.edu>>
 * @author Michelle Fan <<mfan@caltech.edu>>
 * @version 1.0
 * @date 2017
 * 
 */

#include "player.hpp"

#define CORNER_BONUS        (3)
#define SIDE_BONUS          (2)
#define ONE_AWAY_CORNER     (-3)
#define ONE_AWAY_SIDE       (-1)
#define MAX_DEPTH           (31)
/*
 * To determine these constants, several sets of constants were tested over 25
 * games each against SimlePlayer. The results are as follows:
 * (CORNER_BONUS, SIDE_BONUS, ONE_AWAY_CORNER, ONE_AWAY_SIDE) --> W/L/T
 * (3, 2, -2, -1) --> 19/05/01
 * (3, 2, -3, -2) --> 15/10/00
 * (3, 2, -3, -1) --> 24/01/00
 */


/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = true;
    this->side = side;
    board = new Board();
    oppSide = nextSide(side);


    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    
    board->doMove(opponentsMove, oppSide);
    vector<Move *> moves; // a vector of all possible moves

    if(!board->hasMoves(side)){ 
        return nullptr;
    }

    for(int i = 0; i < 8; i ++) {
        for(int j = 0; j < 8; j++) {
            Move * move = new Move(i, j);
            if(board->checkMove(move, side)) {
                moves.push_back(move);
            } else {
                delete move;
            }
        }
    }

    Move * m =  Player::getBestMove(moves);
    m = new Move(m->getX(), m->getY());
    board->doMove(m, side);

    for (vector< Move* >::iterator i = moves.begin(); i != moves.end(); ++i)
        delete (*i);
    moves.clear();


    return m;
}
/*
 * Returns the move with the highest heuristic score
 */
Move* Player::getBestMove(vector<Move*> moves) {

    Move* best = moves[0];

    if (testingMinimax) {

        int best_heur = -999999;
        for (vector<Move*>::iterator i = moves.begin(); i != moves.end(); ++i) {
            Board* copy = board->copy();
            copy->doMove(*i, side);
            int heur = Player::worstResult(oppSide, MAX_DEPTH, copy);
            if (heur > best_heur) best = *i; 
            delete copy;
        }

    } else {
        for (vector< Move* >::iterator i = moves.begin() + 1; i != moves.end(); ++i)
            best = (Player::gradeMove(*i) > Player::gradeMove(best)) ? *i : best;
    }

    return best;
}
/*
 * Gives a move a score based on the change in the difference between numbers of
 * pieces. It multiplies the score by an integer based on its position; moves
 * on corners and sides have a bonus, but moves that are one away from a corner
 * or side are punished. At most one multiplier will be used, and this function
 * prioritizes negative multipliers (i.e., a move to (1,0) will have a negative
 * multiplier because it is adjacent to (0,0)).
 */
int Player::gradeMove(Move* move){
    // we want the change in score
    int startScore = board->count(side) - board->count(oppSide);
    Board* copy = board->copy();
    copy->doMove(move, side);
    int endScore = copy->count(side) - copy->count(oppSide);
    int scoreChange = endScore - startScore;
    int x = move->getX();
    int y = move->getY();

    if (Player::isCorner(move)) {
        scoreChange *= CORNER_BONUS;
    } else if (Player::isSide(move)) {
        if (x == 1 || x == 6 || y == 1 || y == 6) {
            // we are one off from a corner
            scoreChange *= ONE_AWAY_CORNER;
        } else {
            // a normal side position
            scoreChange *= SIDE_BONUS;
        }
    } else {
        // not on a side
        if (x == 1 || x == 6 || y == 1 || y == 6) {
            // one away from a side
            if ((x == 1 || x == 6) && (y == 1 || y == 6)) {
                // one away from a corner
                scoreChange *= ONE_AWAY_CORNER;
            } else {
                scoreChange *= ONE_AWAY_SIDE;
            }
        } // if it's not in the outer two rows, nothing special
    }

    delete copy;
    return scoreChange;
}

/*
 * Given a move, this function examines all possible moves in the next iteration.
 * Of the moves it considers, it finds a heuristic for each. It then returns the
 * minimum heuristic value, also known as the worst case. This is for use in the
 * minimax method of playing the game.
 */
int Player::worstResult(Side theSide, int depth, Board* brd) {

    if (brd->isDone() || depth == MAX_DEPTH) {
        return grade(brd);
    }

    vector<Move*> moves; // vector of all possible resulting moves
    int score = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move * newMove = new Move(i, j);
            if (brd->checkMove(newMove, theSide)) {
                moves.push_back(newMove);
            } else {
                delete newMove;
            } // for each position, if the move is possible, add it to the vec
        }
    }

    for (vector<Move*>::iterator i = moves.begin(); i != moves.end(); i++) {
        Board * copy = brd->copy();
        copy->doMove(*i, theSide);
        score = worstResult(nextSide(theSide), depth + 1, copy);
        delete copy;
        delete (*i);

    }

    moves.clear();

    return score;
}

/*
 * Returns true if the move is on a board edge
 */
bool Player::isSide(Move* move){
    return move->getX() == 0 || move->getX() == 7 || move->getY() == 0 || move->getY() == 7;
}

/*
 * Returns true if the move is on a board corner
 */
bool Player::isCorner(Move* move){
    return (move->getX() == 0 && move->getY() == 0) || (move->getX() == 0 && move->getY() == 7) 
    || (move->getX() == 7 && move->getY() == 0) || (move->getX() == 7 && move->getY() == 7); 
}

Side Player::nextSide(Side side) {
    return (side == BLACK) ? WHITE : BLACK;
}

int Player::grade(Board* b) {
    return b->count(side) - b->count(oppSide);
}
