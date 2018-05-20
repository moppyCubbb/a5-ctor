//
//  GameControl.hpp
//  cs_project5
//
//  Created by Xing Rui on 3/30/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#ifndef GameControl_hpp
#define GameControl_hpp
#include "Player.hpp"

class GameControl {
    Board* b;
    vector<Player*> players;
public:
    virtual void setname(string n) = 0;
    virtual bool isWon()= 0;
    virtual void attachResidence(int residencenum, char namerep);
    virtual void attachRoad(int roadnum);
    virtual int checkresource (ResourceType t);
    virtual string getnam();
    virtual void getStatus();
    virtual void modifyResource(ResourceType rt, int num, int sign);
    virtual int getMark();
    virtual void getResidence();
    virtual int roll (DiceType d);
    virtual void tradeResource(ResourceType r1, ResourceType r2, Player* p1, Player* p2);
    virtual void clear();
    virtual void addbuildingpoint();
    //board method...
};

#endif /* GameControl_hpp */
