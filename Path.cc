//
//  Path.cpp
//  cs_project5
//
//  Created by Xing Rui on 3/27/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#include "Path.h"
#include <cstdlib>
#include <vector>
#include <memory>

void Path::attachAdd(shared_ptr<Address>a) {
    neighborAddr.emplace_back(a);
}

void Path::attachPath(shared_ptr<Path>a) {
    neighborPt.emplace_back(a);
}


bool Path::valbuild(shared_ptr<Player> p) {
    for (auto &a : neighborAddr) {
        if (!a->undeclare() && a->belongTo() == p->getnam()) {
            return true;
        }
    }
    for (auto &a : neighborPt) {
        //cout << num << ":";
        //cout << a->num << " ";
        //cout << a->belongTo();
        //cout << p->getnam();
        if (a->declare() && a->belongTo() == p->getnam()) {
            //cout <<"lalalalal";
            return true;
        }
    }
    //cout << endl;
    //cout << "return false" << endl;
    return false;
}

bool valbuildr(shared_ptr<Player> p) {
    if (p->checkresource(ResourceType::Heat) < 1 ||
        p->checkresource(ResourceType::Wifi) < 1) {
        return false;
    }
    else return true;
}

int Path::getnum() {
    return num;
}


string Path::belongTo() {
    return owner->getnam();
}

void Path::buildRoad(shared_ptr<Player> p) {
    string v = "error from path file";
    //owner = p;
    if (valbuild(p) && valbuildr(p)) {
        owner = p;
        p->attachRoad(getnum());
        p->modifyResource(ResourceType::Heat, -1, 1);
        p->modifyResource(ResourceType::Wifi, -1, 1);
    }
    else {
        if (!valbuild(p)) {
            string n = "You cannot build here.";
            throw n;
        }
        else if (!valbuildr(p)) {
            string n = "You do not have enough resources.";
            throw n;
        }
        else {
            throw v;
        }
    }
}
void Path::setNum(int n){
    num = n;
}

void Path::load(shared_ptr<Player> p) {
    owner = p;
}

bool Path::declare() {
    if (owner != nullptr) {
        return true;
    }
    else return false;
}
