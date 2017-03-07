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
    srand(time(NULL));


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

    Move * m =  moves[rand() % moves.size()];
    m = new Move(m->getX(), m->getY());
    board->doMove(m, side);

    for (std::vector< Move* >::iterator i = moves.begin() ; i != moves.end(); ++i)
        delete (*i);
    moves.clear();


    return m;
}
