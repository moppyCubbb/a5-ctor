#ifndef Path_h
#define Path_h
#include "Player.h"
#include "Tile.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <memory>

class Address;

class Path {
    int num;
    vector<shared_ptr<Address>> neighborAddr;
    vector<shared_ptr<Path>> neighborPt;
    shared_ptr<Player> owner;
    bool valbuild(shared_ptr<Player> p);
public:
    int getnum();
    string belongTo();
    void buildRoad(shared_ptr<Player> p);
    void setNum(int);
    void attachPath(shared_ptr<Path>);
    void attachAdd(shared_ptr<Address>);
    bool declare();
    void load(shared_ptr<Player> p);
};


#endif
