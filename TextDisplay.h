//
//  TextDisplay.hpp
//  cs_project5
//
//  Created by Xing Rui on 4/1/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#ifndef TextDisplay_h
#define TextDisplay_h

#include <vector>
#include <string>
#include <cstdlib>
#include <memory>
#include "ResidenceType.h"
#include "Pos.h"
#include "Tile.h"
using namespace std;

class TextDisplay{
    vector<vector<char>> Text;
    
    vector<shared_ptr<Pos>> tilePos;
    
public:
    void initText();
    void modifyText(int addr, string player, ResidenceType type);
    void modifyText(int path, string player);
    void modifyText(shared_ptr<Tile>, int tile);
    void modifyText(int geeseOld, int geeseNew);
    void print();
};


#endif
