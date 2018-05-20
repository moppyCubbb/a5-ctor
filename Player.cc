//
//  Player.cpp
//  cs_project5
//
//  Created by Xing Rui on 3/27/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#include "Player.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <iterator>
#include <sstream>
#include <utility>
#include "ResourceType.h"
#include "Tile.h"
#include "Address.h"
#include "Random.h"

using namespace std;


// determine which num the ResourceType target to
int Player :: helper (ResourceType r) {
    if (r == ResourceType::Brick) return 0;
    if (r == ResourceType::Energy) return 1;
    if (r == ResourceType::Glass) return 2;
    if (r == ResourceType::Heat) return 3;
    else return 4;
};

//helper use int to return ResourceType;
ResourceType Player :: h(int n) {
    if (n == 0) {
        return ResourceType::Brick;
    } else if (n == 1) {
        return ResourceType::Energy;
    } else if (n == 2) {
        return ResourceType::Glass;
    } else if (n == 3) {
        return ResourceType::Heat;
    } else if (n == 4) {
        return ResourceType::Wifi;
    } else {
        return ResourceType::Park;
    }
}


Player::Player(string name)
: name{name}, Resources{}, total{0} {};

Player::~Player() {};

void Player::setname(string n) {
    name = n;
}

bool Player::isWon() {
    return (buildingpoint>=10);
}

void Player::attachResidence(int residencenum, char namerep) {
    Residences.emplace_back(residencenum, namerep);
}

void Player:: updateResidence(int num, char t) {
    for (auto &n : Residences) {
        if (get<0>(n) == num) {
            get<1>(n) = t;
        }
    }
}

void Player::attachRoad(int roadnum) {
    Roads.emplace_back(roadnum);
}


int Player::checkresource (ResourceType t) {
    if (t == ResourceType::Brick) {
        return Resources[0];
    }
    else if (t == ResourceType::Energy) {
        return Resources[1];
    }
    else if (t == ResourceType::Glass) {
        return Resources[2];
    }
    else if (t == ResourceType::Heat) {
        return Resources[3];
    }
    else {
        return Resources[4];
    }
}

string Player::getnam() {
    return name;
}

void Player::getStatus() {
    int numBrick = Resources[0];
    int numEnergy = Resources[1];
    int numGlass = Resources[2];
    int numHeat = Resources[3];
    int numWifi = Resources[4];
    cout << name << " has " << buildingpoint << " building points, " << numBrick
    <<" brick, " << numEnergy << " energy," << endl;
    cout << numGlass << " glass, "
    << numHeat << " heat, and " << numWifi << " WiFi." << endl;
}

void Player::modifyResource(ResourceType rt, int num, int sign) {
    // if sign is 1, it means with no geese invloved
    if (sign == 1) {
        int i;
        if (rt == ResourceType::Brick) i = 0;
        else if (rt == ResourceType::Energy) i = 1;
        else if (rt == ResourceType::Glass) i = 2;
        else if (rt == ResourceType::Heat) i = 3;
        else i = 4;
        Resources[i] += num;
    }
    total += num;
}

void Player::lost() {
    if (total >= 10) {
        int lost = total / 2;
        total = total - lost;
        cout << "Builder " << getnam() << " loses " << lost << " resources to the geese. They lose:" << endl;
        int steal[5] = {};
        for (int i = lost; i > 0; --i) {
            while (true) {
                int t = r->randomInt(0, 4);
                if (Resources[t] == 0) continue;
                else {
                    ++steal[t];
                    --Resources[t];
                    break;
                }
            }
        }
        for (int i = 0; i < 5; ++i) {
            if (steal[i] != 0) {
                cout << steal[i] << " " << resourcename[i] << endl;
            }
        }
    }
}

int Player::steal() {
    if (total == 0) throw "This builder has nothing to steal from.";
    int i = r->randomInt(0, 4);
    while (Resources[i] == 0) {
        i = r->randomInt(0, 4);
    }
    Resources[i]--;
    return i;
}

int Player::getMark() {
    return buildingpoint;
}

void Player::getResidence() {
    //check the instructions
    int len = Residences.size();
    if (len == 0) {
        cout << "No residence is completed." << endl;
        return;
    }
    for (auto &n : Residences) {
        cout << to_string(get<0>(n)) << " " << get<1>(n) << " ";
    }
    cout << endl;
}



int Player::roll (DiceType d) {
    if (d == DiceType::Fair) {
        int d1 = r->randomInt(1, 6);
        int d2 = r->randomInt(1, 6);
        int sum = d1 + d2;
        cout << "You got a " << sum << endl;
        return sum;
    }
    else {
        int loaded;
        cout << "Input a roll between 2 and 12:" << endl;
        string s;
        while (1) {
            cin.clear();
            cin >> s;
            stringstream ss {s};
            if(ss >> loaded && loaded >= 2 && loaded <= 12) break;
            cout << "Invalid roll." << endl;
        }
        cout << "You got a " << loaded << endl;
        return loaded;
    }
}



void Player::tradeResource(ResourceType r1, ResourceType r2, shared_ptr<Player> p1, shared_ptr<Player> p2) {
    if ((p1->checkresource (r1) < 1) || (p2->checkresource(r2) < 1)) {
        cout << "You do not have enough resources." << endl;
        return;
    }
    else {
        p1->modifyResource(r1, -1, 1);
        p1->modifyResource(r2, 1, 1);
        p2->modifyResource(r1, 1, 1);
        p2->modifyResource(r2, -1, 1);
        cout << "Trade Successful" << endl;
    }
}

void Player::clearPlayer() {
    for (auto &n: Resources) {
        n = 0;
    }
    total = 0;
    Residences.clear();
    Roads.clear();
    buildingpoint = 0;
    
}

void Player::addbuildingpoint() {
    buildingpoint++;
}


string Player::savePlayer() {
    string data = "";
    for (auto &n: Resources) {
        data = data + to_string(n) + " ";
    }
    data = data + "r ";
    for (auto &n: Roads) {
        data = data + to_string(n) + " ";
    }
    data = data + "h ";
    for (auto &p: Residences) {
        data = data + to_string(get<0>(p)) + " " + get<1>(p) + " ";
    }
    data = data + "\n";
    return data;
}

void Player::loadPlayer(string data) {
    istringstream iss{data};
    int num;
    for (int i = 0; i < 5; ++i) {
        iss >> num;
        Resources[i] = num;
    }
    char c;
    iss.clear();
    while (iss >> c) {
        if (c == 'r') {
            while (iss >> num) {
                Roads.emplace_back(num);
            }
            iss.clear();
            iss.ignore();
            while (iss >> num) {
                char t;
                iss >> t;
                Residences.emplace_back(num, t);
            }
            iss.clear();
            iss.ignore();
        }
    }
    for (auto &n : Resources) {
        total += n;
    }
    for (auto &p : Residences) {
        if (get<1>(p) == 'B') buildingpoint += 1;
        else if (get<1>(p) == 'H') buildingpoint += 2;
        else buildingpoint += 3;
    }
}

void Player::set(shared_ptr<Random> r) {
    this->r = r;
}

bool Player::hasResources() {
    if (total > 0) {
        return true;
    }
    return false;
}

int Player::seeReslen() { return Residences.size(); }


