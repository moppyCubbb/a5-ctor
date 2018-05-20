//
//  Game.hpp
//  cs_project5
//
//  Created by Xing Rui on 4/1/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#ifndef Game_h
#define Game_h
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "ResourceType.h"
#include "Random.h"


class Geese;
using namespace std;

class Game{
    Board *b;
    vector<shared_ptr<Player>> players;
    bool load;
    bool rev;
    shared_ptr<Random> r;
    shared_ptr<Geese> g;
    
    int current_turn;
    ResourceType stoRt(string name);
    int pntoint(string p);
    ResourceType inttort(int n);
    string inttostr(int n);
public:
    Game(Board *b, vector<shared_ptr<Player>> &players);
    //start game
    void startGame(string cmd);
    
    //load game from file
    void loadGame(string file);
    
    //load board from file
    void loadBoard(string file);
    
    //save game to file
    void saveGame(string file);
    
    //beginning of the game
    void beginningofGame();
    
    //during the game
    void eachTurn(shared_ptr<Player> p);
    
    void startAgain();

    int get_cur();
    
    bool isLoad();
};


#endif 
