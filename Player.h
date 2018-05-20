//
//  Player.hpp
//  cs_project5
//
//  Created by Xing Rui on 3/27/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#ifndef Player_h
#define Player_h
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include "ResourceType.h"
#include "Tile.h"
#include "Address.h"
#include "DiceType.h"
#include "Random.h"

using namespace std;


class Player {
    string name;
    int Resources[5];
    //vector<string> Resrname = {"Brick", "}
    int total;
    vector <pair <int,char>> Residences;
    vector <int> Roads;
    int buildingpoint = 0;
    shared_ptr<Random> r;
    
    string resourcename[5] = {"BRICK", "ENERGY", "GLASS", "HEAT", "WIFI"};
    
public:
    Player(string name);
    ~Player();
    void setname(string n);
    int steal();
    bool isWon();
    void attachResidence(int residencenum, char namerep);
    void attachRoad(int roadnum);
    int checkresource (ResourceType t);
    string getnam();
    void getStatus();
    void modifyResource(ResourceType rt, int num, int sign);
    int getMark();
    void getResidence();
    int roll (DiceType d);
    void tradeResource(ResourceType r1, ResourceType r2, shared_ptr<Player> p1, shared_ptr<Player> p2);
    void clearPlayer();
    void addbuildingpoint();
    void lost();
    string savePlayer();
    void loadPlayer(string data);
    ResourceType h(int n);
    int helper(ResourceType t);
    void set(shared_ptr<Random> r);
    bool hasResources();
    void updateResidence(int num, char t);
    int seeReslen();
};




#endif 
