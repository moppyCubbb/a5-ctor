#ifndef Address_h
#define Address_h
#include <string>
#include <vector>
#include <cstdlib>
#include <memory>
#include "Tile.h"
#include "ResidenceType.h"
#include "ResourceType.h"
using namespace std;
class Player;
class Path;


class Address {
private:
    int num;
    int No;
    vector<shared_ptr<Address>> neighborAddr;
    vector<shared_ptr<Path>> neighborPt;
    shared_ptr<Player> owner = nullptr;
    ResidenceType Type;
    

    bool validitytobuild(shared_ptr<Player> p);
    bool valtoraod(shared_ptr<Player> p);
    bool sufficiencyresource (ResidenceType target, shared_ptr<Player> p);
    int addrhelp (ResidenceType check);
public:
    ResidenceType initbuild(shared_ptr<Player> p);
    int getnum();
    ResidenceType getType();
    string belongTo();
    bool getResource(ResourceType t);
    ResidenceType buildResidence(shared_ptr<Player> p1);
    ResidenceType improve(int current_level, shared_ptr<Player> p);
    ResidenceType build(int target_level, shared_ptr<Player> p);
    void setNum(int);
    void attachAdd(shared_ptr<Address>);
    void attachPath(shared_ptr<Path>);
    bool undeclare();
    int seenighboradd(int p);
    void load(shared_ptr<Player> p, ResidenceType rt);
    ResidenceType improveRes(shared_ptr<Player> p1);
    bool owner_has_resources();
};

#endif 
