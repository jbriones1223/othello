/**
 * @file player.cpp
 * @author Jack Briones <<jbriones@caltech.edu>>
 * @author Michelle Fan <<mfan@caltech.edu>>
 * @version 1.0
 * @date 2017
 * 
 */

#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    this->side = side;
    board = new Board();
    oppSide = (side == BLACK) ? WHITE : BLACK;


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
    vector<Move *> moves;

    if(!board->hasMoves(side)){ 
        return nullptr;
    }

    for(int i = 0; i < 8; i ++) {
        for(int j = 0; j < 8; j++) {
            Move * move = new Move(i, j);
            if(board->checkMove(move, side)) 
                moves.push_back(move);
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

    for (vector< Move* >::iterator i = moves.begin() + 1; i != moves.end(); ++i)
        best = (Player::gradeMove(*i) > Player::gradeMove(best)) ? *i : best;

    return best;
}
/*
 * Gives a move a score based on number of pieces each side after simulating the move.
 * x2 if the move is on a side; x3 if the move is on a corner
 */
int Player::gradeMove(Move* move){

    Board* copy = board->copy();
    copy->doMove(move, side);
    int toReturn = copy->count(side) - copy->count(oppSide);

    toReturn = (Player::isCorner(move)) ? toReturn * 3 : (Player::isSide(move)) ? toReturn * 2 : toReturn;

    delete copy;
    return toReturn;
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

