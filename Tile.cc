//
//  Tile.cpp
//  cs_project5
//
//  Created by Xing Rui on 3/27/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#include "Tile.h"
#include <vector>
#include <cstdlib>
#include <memory>

void Tile::initTile(ResourceType rt, int val) {
    setRsrc(rt);
    setVal(val);
}

void Tile::setVal(int v){
    val = v;
    
}

Info Tile::getInfo() {
    return { val,Geese,Rsrc};
}

Tile::Tile():val{7},Rsrc{}, Geese{false} {}

Tile::~Tile() {}

void Tile::addAddr(shared_ptr<Address> addr) {
    Addr.emplace_back(addr);
}

void Tile::addPath(shared_ptr<Path> p) {
    Ph.emplace_back(p);
}

void Tile::setRsrc(ResourceType rt) {
    Rsrc = rt;
}

string Tile::setGeese(shared_ptr<Player> p) {
    vector<string> name;
    if (Geese) {
        Geese = false;
        return "";
    }
    else {
        Geese = true;
    }
    //steal action
    
    for (auto &a: Addr) {
        if (!a->undeclare() && a->belongTo() != p->getnam() && a->owner_has_resources()) {
            name.emplace_back(a->belongTo());
        }
    }
    int len = name.size();
    if (len == 0) {
        throw p->getnam();
    }
    cout << "Builder " << p->getnam() << " can choose to steal from ";
    if (len == 1) cout << name[0] << endl;
    else if (len == 2) {
        if (name[0] == name[1]) cout << name[0] << endl;
        else cout << name[0] << ", " << name[1] << endl;
    }
    else {
        if (name[0] == name[1]) {
            cout << name[0] << endl;;
            if (name[0] == name[2]) {
                cout << endl;
            }
            else {
                cout << ", " << name[2] << " ." << endl;
            }
        }
        else if (name[0] == name[2]) {
            cout << name[0] << ", " << name[1] << " ." << endl;
        }
        else if (name[1] == name[2]) {
            cout << name[0] << ", " << name[1] << " ." << endl;
        }
        else {
            cout << name[0] << ", " << name[1] << ", " << name[2] << " ." << endl;
        }
    }
    string player;
    cin.clear();
    cin >> player;
    return player;
}

bool Tile::hasGeese() {
    if (Geese) return true;
    else return false;
}

int Tile::getVal() {
    return val;
}



void Tile::getResource() {
    ResourceType t = Rsrc;
    string s = "No builders gained resources.";
    bool someone = false;
    if (hasGeese()) {
        cout << s << endl;
        return;
    }
    for (auto it : Addr) {
        if (it->getResource(t)) {
            someone = true;
        }
    }
    if (!someone) cout << s << endl;
}

void Tile::setBoardNum(int n){
    boardNum = n;
}
