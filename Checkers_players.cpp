player.cpp
Type
C++
Size
11 KB (11,027 bytes)
Storage used
27 KB (27,967 bytes)
Location
CHECKERS
Owner
me
Modified
Sep 30, 2019 by Theodor Panagiotakopoulos
Opened
11:48 PM by me
Created
Sep 26, 2019 with Google Drive Web
Add a description
Viewers can download

#include "player.hpp"
#include <cstdlib>
#include <algorithm>
#include <string>
#include <unordered_map>
namespace checkers
{
struct OutOfTimeException : public std::exception{
    const char* what() const throw(){
        return "Out of Time";
    }
};
std::unordered_map <std::string,std::vector<int>> Hashmap;
Cell our_player = CELL_RED;
Cell other_player = CELL_WHITE;
Deadline ourdeath;
Deadline now;
int round=0;
const int depthconst_limit = 15; // the limit of depth
const double time_thresh = 0.017; // the time threshhold
// creating the key of the hashmap
std::string state_to_string(const GameState &pState, int depth){
    std::string s(34, ' ');
    for(int i=0;i<32;i++){
        if(pState.at(i)&CELL_WHITE){
            if(pState.at(i)&CELL_KING){
                s[i]='W';
            }else{
                s[i]='w';
            }
        }else if(pState.at(i)&CELL_RED){
            if(pState.at(i)&CELL_KING){
                s[i]='R';
            }else{
                s[i]='r';
            }
        }else{
            s[i]=' ';
        }
        //s[i] =   pState.at(i) + ' ';
    }
    s[32] = pState.getNextPlayer() + ' ';
    s[33] = depth + ' ';
    return s;
}
//writing values to hashmap
void write_to_map(const GameState &pState, std::pair<int,int> ab, int depth, Cell Player, int alpha, int beta){
    std::vector<int> x(4,0);
    x[0] = ab.first;
    x[1] = ab.second;
    x[2] = depth;
    x[3] = (int) Player;
    x[4] = alpha;
    x[5] = beta;
    std::string statestr = state_to_string(pState,depth);
    auto search = Hashmap.find(statestr);
    if(search!= Hashmap.end()){
        //if(search->second[4] >= alpha && search->second[5] <= beta){
            Hashmap[statestr] = x;
        //}
    }else{
        Hashmap.insert({statestr,x});
    }
    
}
//reading from the hashmap
std::pair<int,int> read_from_map(const GameState &pState, int depth, Cell Player,  int alpha, int beta){
    auto search = Hashmap.find(state_to_string(pState,depth));
    if ( search != Hashmap.end()){
        //if(search->second[4] <= alpha && search->second[5] >= beta){
            return std::pair<int,int>(search->second[0],search->second[1]);
        //}
    }
    return std::pair<int,int>(-1,-1);
}
int heuristic(const GameState &pState, Cell pPlayer){ //does not look at player, look at all positions of board
    int v = 0;
    if((pPlayer == CELL_RED) && pState.isRedWin()){ //best case
        v = INT32_MAX;
    }else if((pPlayer == CELL_RED) && pState.isWhiteWin()){ //worst case
        v = INT32_MIN;
    }else if((pPlayer == CELL_WHITE) && pState.isWhiteWin()){ 
        v = INT32_MAX;
    }else if((pPlayer == CELL_WHITE) && pState.isRedWin()){
        v = INT32_MIN;
    }else if(pState.isDraw()){
        v = -99999;
    }else{
        if(pPlayer == CELL_WHITE)
        {
            for(int i=0;i<32;i++)
            {
                if(pState.at(i)&CELL_WHITE){
                    if(pState.at(i)&CELL_KING){
                        v+=3;
                    }else{
                        v+=1;
                    }
                }else if(pState.at(i)&CELL_RED){
                    if(pState.at(i)&CELL_KING){
                        v-=3;
                    }else{
                        v-=1;
                    }
                }
                // v += (int)(pState.at(i)&CELL_WHITE) - (int)(pState.at(i)&CELL_RED);
                //std::cerr << v << " ";
                //v += 10*((int)((pState.at(i)&CELL_KING) - (int)((pState.at(i)&CELL_KING)&CELL_RED));
                //std::cerr << v << " ";
            }
        }
        else
        {
            for(int i=0;i<32;i++)
            {
                if(pState.at(i)&CELL_RED){
                    if(pState.at(i)&CELL_KING){
                        v+=3;
                    }else{
                        v+=1;
                    }
                }else if(pState.at(i)&CELL_WHITE){
                    if(pState.at(i)&CELL_KING){
                        v-=3;
                    }else{
                        v-=1;
                    }
                }
            }
        }
    }
    return v;
}
//sortingrule
bool sortstagesfromhash(std::vector<int> x, std::vector<int> y){
    if(x[1] == y[1]){
        return x[0]>y[0];
    }else{
         return x[1]>y[1];
    }
}
//sorting function
void sortstages(std::vector<GameState> &moves, int depth, char x){
    std::vector<std::vector<int>> statevals (moves.size(), std::vector<int> (3));
    int count=0;
    for(unsigned int i=0;i<moves.size();i++){
        auto search = Hashmap.find(state_to_string(moves[i],depth));
        if ( search != Hashmap.end()){
            statevals[i][0] = search->second[0];
            statevals[i][1] = 1;
            statevals[i][2] = i;
            count++;
        }else{
            statevals[i][0] = 0;
            statevals[i][1] = 0;
            statevals[i][2] = i;
        }
    }
    sort(statevals.begin(),statevals.end(), sortstagesfromhash);
    std::vector<GameState> newmoves (moves.size());
    if(x == 'd'){
        for(unsigned int i=0;i<statevals.size();i++){
            newmoves[i] = moves[statevals[i][2]];
        }
    }else{
        for(unsigned int i=count;i-->0;){
            newmoves[i] = moves[statevals[i][2]];
        }
        for(unsigned int i=count; i<statevals.size();i++){
            newmoves[i] = moves[statevals[i][2]];
        }
    }
    moves.swap(newmoves);
}
//alpha beta returns the alphabeta value together with the depth of the tree
std::pair<int,int> alphabeta(const GameState &state, unsigned int depth, int alpha, int beta, Cell pPlayer ){
    
    if(state.isEOG() || depth == 0){
        now = checkers::Deadline::now();
        if(ourdeath.getSeconds() - now.getSeconds() < time_thresh){
            throw OutOfTimeException();
        }
        return std::make_pair(heuristic(state,our_player),0); //
    }
    int newalpha = alpha;
    int newbeta = beta;
    if(pPlayer == our_player){
        int v = INT32_MIN;
        std::vector<GameState> allnewstates;
        state.findPossibleMoves(allnewstates);
        if(depth>3){
            sortstages(allnewstates,depth-2,'d'); //sorting states based on the values from the previous iteration
        }
        unsigned int i=0;
        int step = 0;
        while(i<allnewstates.size()){
            std::pair<int, int> tmppair = read_from_map(allnewstates[i],depth-1,other_player,newalpha,newbeta);
            if(tmppair.first ==-1 && tmppair.second == -1){
                tmppair = alphabeta(allnewstates[i],depth-1,newalpha,newbeta,other_player);
                write_to_map(allnewstates[i],tmppair,depth-1,other_player,newalpha,newbeta);
            }
            int tempab = tmppair.first;
            if( tempab> v){
                v = tempab;
                step = tmppair.second + 1;
            }
            if(v> newalpha){
                //std::cerr << "alpha updated" << v; 
                newalpha = v;
            }
            if(newalpha >= newbeta){
                return std::make_pair(v,step);
            }
            i++;
        }
        return std::make_pair(v,step);
    }else{
        int v = INT32_MAX;
        std::vector<GameState> allnewstates;
        state.findPossibleMoves(allnewstates);
        if(depth>3){
            sortstages(allnewstates,depth-2,'a');    //sorting states based on the values from the previous iteration
        }
        unsigned int i=0;
        int step = 0;
        while(i<allnewstates.size()){
            std::pair<int, int> tmppair = read_from_map(allnewstates[i],depth-1,our_player,newalpha,newbeta); //try to get value from hashtable
            if(tmppair.first ==-1 && tmppair.second == -1){ //if it doesnt exist 
                tmppair = alphabeta(allnewstates[i],depth-1,newalpha,newbeta,our_player); //calculate it and
                write_to_map(allnewstates[i],tmppair,depth-1,our_player,newalpha,newbeta); // add it to the hashtable
            }
            int tempab = tmppair.first;
            if( tempab< v){
                v = tempab;
                step = tmppair.second + 1;
            }
            if(v< newbeta){
                newbeta = v;
            }
            if(newalpha >= newbeta){
                return std::make_pair(v,step);
            }
            i++;
        }
        return std::make_pair(v,step);
    }
    
}

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    int depthconst = 1; //starting depth
    Hashmap.clear(); //clear the hash
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates); //find all possible moves
    if(pState.getNextPlayer() == CELL_RED){ // if i am X player assign the values
        our_player = CELL_RED;
        other_player = CELL_WHITE;
    }else{  // if i am O player
        our_player = CELL_WHITE;
        other_player = CELL_RED;
    }
    
    //std::cerr << "I am " << our_player << std::endl;
    if (lNextStates.size() == 0) return GameState(pState, Move());
    if (lNextStates.size() == 1) return lNextStates[0]; //if we have only one posible move return it
    int best_steps=100;
    ourdeath = pDue;
    now = checkers::Deadline::now();
    while(ourdeath.getSeconds() - now.getSeconds() > time_thresh && depthconst < depthconst_limit){
        int best_value=INT32_MIN;
        sortstages(lNextStates,depthconst-1,'d'); //sort stages based on the previous depth
        for(unsigned int i=0;i<lNextStates.size();i++){ // do the first depth of the alphabetaprunning
            try{ // try calculating the alphabeta, if an outoftimeexception exception is being thrown go to the catch
                std::pair<int, int> abtemp = read_from_map(lNextStates[i],depthconst,other_player,best_value,INT32_MAX); //look for the hash before calculating the value
                if(abtemp.first ==-1 && abtemp.second == -1){ //if there is no value in the hash
                    abtemp = alphabeta(lNextStates[i],depthconst, best_value, INT32_MAX,other_player); // calculate the alphabeta
                    write_to_map(lNextStates[i],abtemp, depthconst,other_player,best_value,INT32_MAX); // save the alphabeta to the hashmap, so that if we find it again we wont have to calculate it
                }
                int tmpab = abtemp.first; // the ab value
                if (tmpab > best_value || (tmpab == best_value && abtemp.second < best_steps)){ //keeping max value of alphabeta
                    best_value = tmpab; //update the abvalue
                    best_steps = abtemp.second;
                }
            }catch(OutOfTimeException& e){ //if we are out of time return the current best state 
                //std::cerr << depthconst-1;
                return lNextStates[0];
            }
        }
        depthconst+=1;
        now = checkers::Deadline::now();
    }
    sortstages(lNextStates,depthconst-1,'d');   
    //std::cerr << ourdeath.getSeconds() - now.getSeconds() << " " << depthconst--<< std::endl;
    return lNextStates[0];
}

/*namespace checkers*/ }
