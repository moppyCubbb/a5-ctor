//
//  Tile.hpp
//  cs_project5
//
//  Created by Xing Rui on 3/27/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#ifndef Tile_h
#define Tile_h
#include <vector>
#include <string>
#include <cstdlib>
#include <memory>
#include "Address.h"
#include "Path.h"
#include "Info.h"
#include "Player.h"

using namespace std;

class Tile {
    vector<shared_ptr<Address>> Addr;
    vector<shared_ptr<Path>> Ph;
    int val;
    int boardNum;
    ResourceType Rsrc;
    bool Geese;
public:
    Tile();
    ~Tile();
    void initTile(ResourceType rt, int val);
    void addAddr(shared_ptr<Address> addr);
    void addPath(shared_ptr<Path> pt);
    void setRsrc(ResourceType rt);
    int getVal();
    string setGeese(shared_ptr<Player> p);
    bool hasGeese();
    void getResource();
    Info getInfo();
    void setVal(int val);
    void setBoardNum(int);
    //ResourceType getResourceType();
};
//board need a method to find the tile

#endif 
