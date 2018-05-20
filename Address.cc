#include "Address.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <memory>
#include "Tile.h"
#include "ResidenceType.h"
#include "ResourceType.h"

class Player;


using namespace std;

void Address::attachAdd(shared_ptr<Address> a) {
    neighborAddr.emplace_back(a);
}

void Address::attachPath(shared_ptr<Path> p) {
    neighborPt.emplace_back(p);
}

int Address::addrhelp (ResidenceType check) {
    if (check == ResidenceType::Basement) {
        return 1;
    }
    else if (check == ResidenceType::House) {
        return 2;
    }
    else {
        return 3;
    }
}

bool Address::validitytobuild(shared_ptr<Player> p) {
    for (auto &a : neighborAddr) {
        if (!a->undeclare()) {
            //cout << "You cannot build here" << endl;
            return false;
        }
    }
    for (auto &pt : neighborPt) {
        if (pt->declare() && pt->belongTo() == p->getnam()) {
            return true;
        }
//        else {
//            cout << "You cannot build here" << endl;
//            return false;
//    }
    }
    return false;
}


bool Address::sufficiencyresource (ResidenceType target, shared_ptr<Player> p) {
    if (target == ResidenceType::Basement) {
        return (
        (p->checkresource(ResourceType::Brick) >= 1) &&
        (p->checkresource(ResourceType::Energy) >= 1) &&
        (p->checkresource(ResourceType::Glass) >= 1) &&
        (p->checkresource(ResourceType::Wifi) >= 1));
    }
    else if (target == ResidenceType::House) {
        return (
        (p->checkresource(ResourceType::Glass) >= 2) &&
        (p->checkresource(ResourceType::Heat) >= 3)
        );
    }
    else {
        return (
        (p->checkresource(ResourceType::Brick) >= 3) &&
        (p->checkresource(ResourceType::Energy) >= 2) &&
        (p->checkresource(ResourceType::Glass) >= 2) &&
        (p->checkresource(ResourceType::Heat) >= 1) &&
        (p->checkresource(ResourceType::Wifi) >= 1)
        );
    }
}

int Address::seenighboradd(int p) {
    return neighborAddr[p]->getnum();
}

ResidenceType Address::initbuild(shared_ptr<Player> p) {
    string w = "You cannot build here.";
    if (owner != nullptr) throw w;
    for (auto &a : neighborAddr) {
        if (a->belongTo() != "") {
            throw w;
        }
    }
        //cout << num << endl;
        owner = p;
        p->attachResidence(num, 'B');
        return ResidenceType::Basement;
}


int  Address::getnum() {
    return num;
}

ResidenceType Address::getType() {
    return Type;
}

string Address::belongTo() {
    if (owner == nullptr) return "";
    return owner->getnam();
}

bool Address::getResource(ResourceType t) {
    if (owner == nullptr) return false;
    else {
        int p = addrhelp(Type);
        owner->modifyResource(t, p, 1);
        string type;
        if (t == ResourceType::Brick) type = "BRICK";
        else if (t == ResourceType::Energy) type = "ENERGY";
        else if (t == ResourceType::Glass) type = "GLASS";
        else if (t == ResourceType::Heat) type = "HEAT";
        else type = "WIFI";
        cout << "Builder " << owner->getnam() << " gained:" << endl;
        cout << to_string(p) << " " << type << endl;
        return true;
    }
}

ResidenceType Address::buildResidence(shared_ptr<Player> p1) {
    string warning = "You cannot build here";
    string warning1 = "You do not have enough resources.";
    string warning2 = "Use #improve command instead!";
    //owner = p1;
    if (owner != nullptr && belongTo() != p1->getnam()) {
        throw warning;
    }
    else if (owner == nullptr) {
        if (validitytobuild(p1)) {
            if (sufficiencyresource (ResidenceType::Basement, p1)) {
                return build(1, p1);
            }
            else throw warning1;
        }
        else throw warning;
    }
    else throw warning2;
}



ResidenceType Address::improveRes(shared_ptr<Player> p1) {
    string warning = "You cannot build here";
    //string warning1 = "You do not have enough resources.";
    string n = "Use #build command instead!";
    if (owner != nullptr && belongTo() != p1->getnam()) {
        throw warning;
    }
    else if (owner == nullptr) {
        throw n;
    }
    else {
        int residencelevel = addrhelp(getType());
        return improve(residencelevel, p1);
    }
}

ResidenceType Address::improve(int current_level, shared_ptr<Player> p) {
    string warning1 = "You do not have enough resources.";
    string warning2 = "Already max level.";
    if (current_level == 1) {// level1 means already a basement
        //use helper to check for sufficiency of resources
        //owner = p;
        if (sufficiencyresource (ResidenceType::House, p)) {
            Type = ResidenceType::House;
            return build(2, p);
        }
        else {
            throw warning1;
        }
    }
    else if (current_level == 2) { //level 2 means already a house
        //use helper to check for sufficiency of resources
        if (sufficiencyresource (ResidenceType::Tower, p)) {
            Type = ResidenceType::Tower;
            return build (3,p);
        }
        else {
            throw warning1;
        }
    }
    else {
        throw warning2;
    }
}

ResidenceType Address::build(int target_level, shared_ptr<Player> p) {
    int residencenum = getnum();
    if (target_level == 1) {
        owner = p;
    // build basement;
    //1. player attach the residence into the residence list
    p->attachResidence(residencenum, 'B');
                        
                        
                        
    //2. player get respective resources minused from their resource bank
    p->modifyResource(ResourceType::Brick, -1, 1);
    p->modifyResource(ResourceType::Energy, -1, 1);
    p->modifyResource(ResourceType::Glass, -1, 1);
    p->modifyResource(ResourceType::Wifi, -1, 1);
                        
                        
                        
    //3. notify textdisplay --- place in the main
    
    
    //4. add buildpoint
    p->addbuildingpoint();
        
        
        return ResidenceType::Basement;
    }
    else if (target_level == 2) {
    //build house
    //p->attachResidence(residencenum, 'H'); ????!!!
    p->updateResidence(num, 'H');
    p->modifyResource(ResourceType::Glass, -2, 1);
    p->modifyResource(ResourceType::Heat, -3, 1);
    p->addbuildingpoint();
        return ResidenceType::House;
    }
    else {
    //build tower
    //p->attachResidence(residencenum, 'T');
    p->updateResidence(num, 'T');
    p->modifyResource(ResourceType::Brick, -3, 1);
    p->modifyResource(ResourceType::Energy, -2, 1);
    p->modifyResource(ResourceType::Glass, -2, 1);
    p->modifyResource(ResourceType::Wifi, -1, 1);
    p->modifyResource(ResourceType::Heat, -2, 1);
    p->addbuildingpoint();
        return ResidenceType::Tower;
    }
}

void Address::setNum(int n){
    num = n;
}

bool Address::undeclare() {
    if (owner == nullptr) return true;
    else return false;
}

void Address::load(shared_ptr<Player> p, ResidenceType rt) {
    owner = p;
    Type = rt;
}
bool Address::owner_has_resources() {
    if (owner != nullptr && owner->hasResources()) {
        return true;
    }
    return false;
}
