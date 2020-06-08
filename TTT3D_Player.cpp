player.cpp
Type
C++
Size
8 KB (8,167 bytes)
Storage used
0 bytes
Location
TTT3D
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

namespace TICTACTOE3D
{
Cell our_player = CELL_X;
Cell other_player = CELL_O;
int depthconst = 1;
int rate_move(const GameState &state){
    int dim = 4;
    int sum=0;
    int sumour=0;
    int sumother=0;
    uint8_t tmp;
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            sumour=0;
            sumother=0;
            for(int k=0;k<dim;k++){
                tmp = state.at(i,j,k);
                if( tmp == our_player){
                    sumour++;
                }else if(tmp == other_player){
                    sumother++;
                }
            }
            if(sumour==0 || sumother==0){
                sum+=sumour-sumother;
            }
            sumour=0;
            sumother=0;
            for(int k=0;k<dim;k++){
                tmp = state.at(i,k,j);
                if( tmp == our_player){
                    sumour++;
                }else if(tmp == other_player){
                    sumother++;
                }
            }
            if(sumour==0 || sumother==0){
                sum+=sumour-sumother;
            }
            sumour=0;
            sumother=0;
            for(int k=0;k<dim;k++){
                tmp = state.at(k,j,i);
                if( tmp == our_player){
                    sumour++;
                }else if(tmp == other_player){
                    sumother++;
                }
            }
            if(sumour==0 || sumother==0){
                sum+=sumour-sumother;
            }
        }
    }
    //diagonals
    for(int i=0;i<dim;i++){
        sumour=0;
        sumother=0;
        for(int j=0;j<dim;j++){
            tmp = state.at(i,j,j);
            if( tmp == our_player){
                sumour++;
            }else if(tmp == other_player){
                sumother++;
            }  
        }
        if(sumour==0 || sumother==0){
            sum+=sumour-sumother;
        }

        sumour=0;
        sumother=0;
        for(int j=0;j<dim;j++){
            tmp = state.at(i,j,3-j);
            if( tmp == our_player){
                sumour++;
            }else if(tmp == other_player){
                sumother++;
            }
        }
        if(sumour==0 || sumother==0){
            sum+=sumour-sumother;
        }
        sumour=0;
        sumother=0;
        for(int j=0;j<dim;j++){
            tmp = state.at(j,j,i);
            if( tmp == our_player){
                sumour++;
            }else if(tmp == other_player){
                sumother++;
            }
        }
        if(sumour==0 || sumother==0){
            sum+=sumour-sumother;
        }
        sumour=0;
        sumother=0;
        for(int j=0;j<dim;j++){
            tmp = state.at(j,3-j,i);
            if( tmp == our_player){
                sumour++;
            }else if(tmp == other_player){
                sumother++;
            }
        }
        if(sumour==0 || sumother==0){
            sum+=sumour-sumother;
        }
        sumour=0;
        sumother=0;
        for(int j=0;j<dim;j++){
            tmp = state.at(j,i,j);
            if( tmp == our_player){
                sumour++;
            }else if(tmp == other_player){
                sumother++;
            }
        }
        if(sumour==0 || sumother==0){
            sum+=sumour-sumother;
        }
        sumour=0;
        sumother=0;
        for(int j=0;j<dim;j++){
            tmp = state.at(j,i,3-j);
            if( tmp == our_player){
                sumour++;
            }else if(tmp == other_player){
                sumother++;
            }
        }
        if(sumour==0 || sumother==0){
            sum+=sumour-sumother;
        }
    }
    sumour=0;
    sumother=0;
    for(int i=0;i<dim;i++){
        tmp = state.at(i,i,i);
        if( tmp == our_player){
            sumour++;
        }else if(tmp == other_player){
            sumother++;
        }
    }
    if(sumour==0 || sumother==0){
        sum+=sumour-sumother;
    }
    sumour=0;
    sumother=0;
    for(int i=0;i<dim;i++){
        tmp = state.at(i,i,3-i);
        if( tmp == our_player){
            sumour++;
        }else if(tmp == other_player){
            sumother++;
        }
    }
    if(sumour==0 || sumother==0){
        sum+=sumour-sumother;
    }
    sumour=0;
    sumother=0;
    for(int i=0;i<dim;i++){
        tmp = state.at(3-i,i,i);
        if( tmp == our_player){
            sumour++;
        }else if(tmp == other_player){
            sumother++;
        }
    }
    if(sumour==0 || sumother==0){
        sum+=sumour-sumother;
    }
    sumour=0;
    sumother=0;
    for(int i=0;i<dim;i++){
        tmp = state.at(i,3-i,i);
        if( tmp == our_player){
            sumour++;
        }else if(tmp == other_player){
            sumother++;
        }
    }
    if(sumour==0 || sumother==0){
        sum+=sumour-sumother;
    }
    return sum;
}
int heuristic(const GameState &pState, Cell pPlayer){
    if(((pPlayer == CELL_X) && pState.isXWin()) || ((pPlayer == CELL_O) && pState.isOWin())){ // if the pPlayer won
        return 100000;
    }else if(((pPlayer == CELL_O) && pState.isXWin()) || ((pPlayer == CELL_X) && pState.isOWin())){ // if the pPlayer lost
        return -100000;
    }else{
        return rate_move(pState);
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
    if(lNextStates.size() <= 30){
        depthconst = 2;
    }
    if(lNextStates.size() <= 10){
        depthconst = 4;
    }
    int best_state=0;
    int best_value=INT32_MIN;
    for(unsigned int i=0;i<lNextStates.size();i++){
        int tmpab = alphabeta(lNextStates[i],depthconst, INT32_MIN, INT32_MAX,other_player);
       // std::cerr << tmpab; // printing alpha values for each possible state 
        if (tmpab > best_value){
            best_state=i;
            best_value = tmpab;
            
        }
    }
    return lNextStates[best_state];
}

/*namespace TICTACTOE3D*/ }
