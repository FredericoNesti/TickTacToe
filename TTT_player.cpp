player.cpp
Type
C++
Size
3 KB (3,301 bytes)
Storage used
0 bytes
Location
TTT
Owner
Theodor Panagiotakopoulos
Modified
Sep 24, 2019 by Theodor Panagiotakopoulos
Created
Sep 24, 2019
Add a description
Viewers can download

#include "player.hpp"
#include <cstdlib>

namespace TICTACTOE
{
Cell our_player = CELL_X;
Cell other_player = CELL_O;
int depthconst = 4;
int heuristic(const GameState &pState, Cell pPlayer){
    if(((pPlayer == CELL_X) && pState.isXWin()) || ((pPlayer == CELL_O) && pState.isOWin())){ // if the pPlayer won
        return 1;
    }else if(((pPlayer == CELL_O) && pState.isXWin()) || ((pPlayer == CELL_X) && pState.isOWin())){ // if the pPlayer lost
        return -1;
    }else{
        return 0;
    }
}
// implementing the alpha beta pruning
int alphabeta(const GameState &state, unsigned int depth, int alpha, int beta, Cell pPlayer ){
    if(state.isEOG() || depth == 0){
        return heuristic(state,our_player);
    }
    int newalpha = alpha;
    int newbeta = beta;
    if(pPlayer == our_player){
        int v = INT32_MIN;
        std::vector<GameState> allnewstates;
        state.findPossibleMoves(allnewstates);
        unsigned int i=0;
        bool flag = false;
        while(i<allnewstates.size() && flag == false){
            int tempab = alphabeta(allnewstates[i],depth-1,newalpha,newbeta,other_player);
            if( tempab> v){
                v = tempab; 
            }
            if(v> newalpha){
                //std::cerr << "alpha updated" << v; 
                newalpha = v;
            }
            if(newalpha >= newbeta){
                flag = true;
            }
            i++;
        }
        return v;
    }else{
        int v = INT32_MAX;
        std::vector<GameState> allnewstates;
        state.findPossibleMoves(allnewstates);
        unsigned int i=0;
        bool flag = false;
        while(i<allnewstates.size() && flag == false){
            int tempab = alphabeta(allnewstates[i],depth-1,newalpha,newbeta,our_player);
            if( tempab< v){
                v = tempab; 
            }
            if(v< newbeta){
                newbeta = v;
            }
            if(newalpha >= newbeta){
               flag = true;
            }
            i++;
        }
        return v;
    }
    
}

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;
    // find which player i am!!!
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    if(pState.getNextPlayer() == CELL_X){ // if i am X player assign the values
        our_player = CELL_X;
        other_player = CELL_O;
    }else{  // if i am O player
        our_player = CELL_O;
        other_player = CELL_X;
    }
    //std::cerr << "I am " << our_player << std::endl;
    if (lNextStates.size() == 0) return GameState(pState, Move());
    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */
    int best_state=0;
    int best_value=INT32_MIN;
    for(unsigned int i=0;i<lNextStates.size();i++){
        int tmpab = alphabeta(lNextStates[i],depthconst, INT32_MIN, INT32_MAX,other_player);
        std::cerr << tmpab; // printing alpha values for each possible state 
        if (tmpab > best_value){
            best_state=i;
            best_value = tmpab;
            
        }
    }
    return lNextStates[best_state];
}

/*namespace TICTACTOE*/ }
