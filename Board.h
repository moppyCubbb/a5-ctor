//
//  Board.h
//  cs_project5
//
//  Created by Xing Rui on 3/30/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#ifndef Board_h
#define Board_h
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include "ResourceType.h"
#include "TextDisplay.h"
#include "Pos.h"
#include "Player.h"
#include "Address.h"
#include "Path.h"
#include "Random.h"
#include "Geese.h"

class Tile;
class Address;
class Path;

using namespace std;

class Board{
    vector<string> board;
    vector<shared_ptr<Tile>> tilelist;
    vector<shared_ptr<Address>> addrlist;
    vector<shared_ptr<Path>> ptlist;
    shared_ptr<TextDisplay> td;
    vector<Pos> add;
    vector<Pos> topLeft;
    struct Edge{
        int v1, v2, p;
    };
    std::vector<Edge> edge;
    bool hasRight(Pos);
    bool hasBot(Pos);
    int getNum(Pos);
    shared_ptr<Address> lookAdd(int);
    //Path* lookEdge(int);
    shared_ptr<Path> lookPath(int);
    int rttoint(ResourceType t);
    ResourceType inttort(int n);
    shared_ptr<Random> r;
    shared_ptr<Geese> g;
public:
    void init();
    void initBoard();
    void print();
    void randomBoard();
    Board();
    ~Board();
    void loadBoard(string data, vector<shared_ptr<Player>>);
    string saveBoard();

    void buildResidence(int addr, shared_ptr<Player> p);
    void buildRoad(int pt, shared_ptr<Player> p);
    void improve(int addr, shared_ptr<Player> p);
    void getResource(int val);
    string placeGeese(int tile, shared_ptr<Player> p);
    void searchAdd();
    void clearBoard();
    void setEdge();
    void setAdd();
    void initTile();
    void addNeighbour();
    void initialBuild(int i, shared_ptr<Player> p);
    void searchTile(int);
    void set(shared_ptr<Random> r, shared_ptr<Geese> g);
};






#endif 
