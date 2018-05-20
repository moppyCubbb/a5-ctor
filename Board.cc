//
//  Board.cpp
//  cs_project5
//
//  Created by Xing Rui on 3/30/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#include "Board.h"
#include "Tile.h"
#include "ResourceType.h"
#include "Pos.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <memory>
#include "Address.h"
#include "Random.h"

class Geese;
using namespace std;

void Board::randomBoard() {
    //initialize 2 array;
    
    initBoard();
    
    init();
    
    int rt[6] = {4,4,4,3,3};
    int val[13] = {0,0,1,2,2,2,2,1,2,2,2,2,1};
    int t;
    int v;
    //random val;
    for (int i = 0; i < 19; ++i) {
        while (true) {
            v = r->randomInt(0, 12);
            
            if (val[v] == 0) continue;
            else {
                
                tilelist[i]->setVal(v); //helper: get the specific value;
                val[v]--;
                break;
            }
            
        }
    }
    
    //random Resource;
    for (int i = 0; i < 19; ++i) {
        while (true) {
            if (tilelist[i]->getVal() == 7) {
                tilelist[i]->setRsrc(ResourceType::Park);
                break;
            }
            t = r->randomInt(0, 4);
            if (rt[t] == 0) continue;
            else {
                tilelist[i]->setRsrc(inttort(t));  //helper:  convert to resource type;
                rt[t]--;
                break;
            }
        }
    }
    for (int i = 0; i < 19; ++i) {
        td->modifyText(tilelist[i], i);
    }
}



void Board::init(){
    ifstream f("MAP.txt");
    string s;
    while(getline(f,s)){
        board.emplace_back(s);
    }
    searchAdd();
    setAdd();
    setEdge();
    addNeighbour();
}

bool Board::hasRight(Pos p) {
    int width = board[p.r].size();
    if(p.c + 2 >= width) return false;
    //cout << board[p.r][p.c] << board[p.r][p.c + 2] << endl;
    if(board[p.r][p.c + 2] == '-') return true;
    else return false;
}

bool Board::hasBot(Pos p) {
    int height = board.size();
    if(p.r + 1 >= height) return false;
    int width = board[p.r + 1].size();
    if(p.c >= width) return false;
    
    if(board[p.r + 1][p.c] == '|') return true;
    else return false;
}

int Board::getNum(Pos p) {
    int rv = 0;
    char c = board[p.r][p.c];
    //cout << c << endl;
    if(c >= '0' && c <= '9') {
        rv = c - '0';
    }
    c = board[p.r][p.c - 1];
    if(c >= '0' && c <= '9') {
        rv += 10 * (c - '0');
    }
    return rv;
}

void Board::searchAdd(){
    Pos p;
    Edge e;
    int height = board.size();
    for(int i = 0; i < height; i ++) {
        int width = board[i].size();
        for(int j = 0; j < width; j ++) {
            if(board[i][j] == '|' && j + 3 < width && board[i][j + 3] == '|') {
                p.r = i;
                p.c = j + 2;
                add.emplace_back(p);
            }
        }
    }
    //cout << "add: " << add.size() << endl;
    for(auto &v: add) {
        //cout << "add: " << v.r << v.c << board[v.r][v.c] << endl;
        if(hasRight(v) && hasBot(v)) {
            p = v;
            p.r += 4;
            if(hasBot(p))
                topLeft.emplace_back(p);
        }
        if(hasRight(v)) {
            p = v;
            e.v1 = getNum(p);
            p.c += 5;
            e.p = getNum(p);
            p.c += 5;
            e.v2 = getNum(p);
            edge.emplace_back(e);
        }
        if(hasBot(v)) {
            p = v;
            e.v1 = getNum(p);
            p.r += 2;
            e.p = getNum(p);
            p.r += 2;
            e.v2 = getNum(p);
            edge.emplace_back(e);
        }
    }
    //cout << "edge: " << edge.size() << endl;
}

void Board::setEdge(){
    int len = edge.size();
    for (int i =0; i < len; i++){
        shared_ptr<Path> p = make_shared<Path>();
        int n = edge[i].p;
        p->setNum(n);
        ptlist.emplace_back(p);
    }
}

void Board::setAdd(){
    int len = add.size();
    for (int i=0; i< len; i++){
        shared_ptr<Address> a = make_shared<Address>();
        a->setNum(i);
        //cout << a->getnum()<<endl;
        addrlist.emplace_back(a);
    }
}


shared_ptr<Address> Board::lookAdd(int n){
    int len = addrlist.size();
    for (int i= 0; i< len; i++){
        if(addrlist[i]->getnum() == n){
            return addrlist[i];
        }
    }
    return nullptr;
}
shared_ptr<Path> Board::lookPath(int n){
    int len = ptlist.size();
    for(int i= 0; i< len; i++){
        if(ptlist[i]->getnum() == n){
            return ptlist[i];
        }
    }
    return nullptr;
}
void Board::addNeighbour(){
    int len = edge.size();
    //cout << len << endl;
    for (int i = 0; i< len; i++){
        shared_ptr<Address> temp1 = lookAdd(edge[i].v1);
        shared_ptr<Address> temp2 = lookAdd(edge[i].v2);
        temp1->attachAdd(temp2);
        temp2->attachAdd(temp1);
        //cout << temp1-> getnum() << " " << temp2-> getnum() << endl;
        shared_ptr<Path> p = make_shared<Path>();
        p = lookPath(edge[i].p);
        temp1->attachPath(p);
        temp2->attachPath(p);
        p->attachAdd(temp1);
        p->attachAdd(temp2);
    }
    for(int j= 0; j< len; j++){
        for(int k= j+1; k< len; k++){
            int v1= edge[j].v1;
            int v2= edge[j].v2;
            int v3= edge[k].v1;
            int v4= edge[k].v2;
            shared_ptr<Path> temp1 = lookPath(edge[j].p);
            shared_ptr<Path> temp2 = lookPath(edge[k].p);
            if(v1 == v3){
                temp1->attachPath(temp2);
                temp2->attachPath(temp1);
            }
            if(v1 == v4){
                temp1->attachPath(temp2);
                temp2->attachPath(temp1);
            }
            if(v2 == v3){
                temp1->attachPath(temp2);
                temp2->attachPath(temp1);
            }
            if(v2 == v4){
                temp1->attachPath(temp2);
                temp2->attachPath(temp1);
            }
        }
    }
}
ResourceType Board::inttort(int n) {
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

int Board::rttoint(ResourceType t) {
    if (t == ResourceType::Brick) {
        return 0;
    } else if (t == ResourceType::Energy) {
        return 1;
    } else if (t == ResourceType::Glass) {
        return 2;
    } else if (t == ResourceType::Heat) {
        return 3;
    } else if (t == ResourceType::Wifi) {
        return 4;
    } else {
        return 5;
    }
}

string Board::saveBoard() {
    string data = "";
    for (auto &tile: tilelist) {
        Info info = tile->getInfo();
        data = data + to_string(rttoint(info.rt)) + " " + to_string(info.val) + " ";
    }
    data = data + "\n";
    return data;
}

void Board::loadBoard(string data, vector<shared_ptr<Player>> players) {
    initBoard();
    init();
    istringstream iss{data};
    int rt, val;
    for (int i = 0; i < 19; ++i) {
        iss >> rt >> val;
        tilelist[i]->initTile(inttort(rt), val);
        td->modifyText(tilelist[i], i);
    }
    string n;
    shared_ptr<Player> p;
    while (iss >> n) {
        if (n == "Blue") p = players[0];
        else if (n == "Red") p = players[1];
        else if (n == "Orange") p = players[2];
        else p = players[3];
        char t;
        while (iss >> t) {
            if (t == 'r') {
                int pt;
                while (iss >> pt) {
                    ptlist[pt]->load(p);
                    td->modifyText(pt, p->getnam());
                }
                iss.clear();
                iss.ignore();
                int addr;
                char type;
                while (iss >> addr && addr != -1) {
                    iss >> type;
                    ResidenceType rt;
                    if (type == 'B') rt = ResidenceType::Basement;
                    else if (type == 'H') rt = ResidenceType::House;
                    else  rt = ResidenceType::Tower;
                    addrlist[addr]->load(p, rt);
                    td->modifyText(addr, p->getnam(), rt);
                }
                break;
            }
        }
    }
}


Board::Board(): td{nullptr} {
}

Board::~Board() {}



void Board::initBoard() {
    //cout << "1" << endl;
    td = make_shared<TextDisplay>();
    td->initText();
    //cout << "1+" << endl;
    for (int i = 0; i < 19; ++i) {
        shared_ptr<Tile> p{new Tile};
        tilelist.emplace_back(p);
    }
    for (int i = 0; i < 54; ++i) {
        auto p = make_shared<Address>();
        p->setNum(i);
        addrlist.emplace_back(p);
    }
    for (int i = 0; i < 72; ++i) {
        shared_ptr<Path> p{new Path};
        p->setNum(i);
        ptlist.emplace_back(p);
    }
    //cout << "2" <<endl;
    int p_t = 0;
    int p = 0;
    int i;
    int t;
    for (t = 0; t <= 18; t++ ) {
        if (t == 0 ) {
            tilelist[t]->addAddr(addrlist[0]);
            tilelist[t]->addAddr(addrlist[1]);
            tilelist[t]->addAddr(addrlist[3]);
            tilelist[t]->addAddr(addrlist[4]);
            tilelist[t]->addAddr(addrlist[8]);
            tilelist[t]->addAddr(addrlist[9]);
            
            tilelist[t]->addPath(ptlist[0]);
            tilelist[t]->addPath(ptlist[1]);
            tilelist[t]->addPath(ptlist[2]);
            tilelist[t]->addPath(ptlist[6]);
            tilelist[t]->addPath(ptlist[7]);
            tilelist[t]->addPath(ptlist[10]);
        }
        else if (t == 18) {
            tilelist[t]->addAddr(addrlist[44]);
            tilelist[t]->addAddr(addrlist[45]);
            tilelist[t]->addAddr(addrlist[49]);
            tilelist[t]->addAddr(addrlist[50]);
            tilelist[t]->addAddr(addrlist[52]);
            tilelist[t]->addAddr(addrlist[53]);

            tilelist[t]->addPath(ptlist[61]);
            tilelist[t]->addPath(ptlist[64]);
            tilelist[t]->addPath(ptlist[65]);
            tilelist[t]->addPath(ptlist[69]);
            tilelist[t]->addPath(ptlist[70]);
            tilelist[t]->addPath(ptlist[71]);
            
        }
        else if (t >= 3 && t <= 15 && t != 6 && t!= 7 && t!= 11 && t!= 12) {
            // case 1 : 3, 4, 5;
            if (t >= 3 && t <= 5) {
                i = t*2;
                p_t = t+6;                
            }
            // case 2: 8, 9, 10;
            if (t >= 8 && t <= 10) {
                i = t*2+2;
                p_t = t + 18;
            }
            // case 3: 13, 14, 15
            if (t >= 13 && t<=15) {
                i = t*2+4;
                p_t = t + 30;
            }
            if ( t == 3 || t == 8 || t == 13) {
                p = p_t + 3;
            }
            if ( t == 4 || t == 9 || t == 14) {
                p = p_t + 4;
            }
            if ( t== 5 || t == 10 || t == 15) {
                p = p_t + 5;
            }
            tilelist[t]->addAddr(addrlist[i]);
            tilelist[t]->addAddr(addrlist[i+1]);
            tilelist[t]->addAddr(addrlist[i+6]);
            tilelist[t]->addAddr(addrlist[i+7]);
            tilelist[t]->addAddr(addrlist[i+12]);
            tilelist[t]->addAddr(addrlist[i+13]);
            
            tilelist[t]->addPath(ptlist[p_t]);
            tilelist[t]->addPath(ptlist[p]);
            tilelist[t]->addPath(ptlist[p+1]);
            tilelist[t]->addPath(ptlist[p+8]);
            tilelist[t]->addPath(ptlist[p+9]);
            tilelist[t]->addPath(ptlist[p_t + 17]);
        }
        else if (t >= 3 && t <= 15) {
            // case 1
            if (t == 6 || t == 7) {
                i = t*2+1;
                p_t = t + 12;
            }
            // case 2
            if (t == 11 || t == 12) {
                i = t*2+3;
                p_t = t + 24;
            }
            if (t == 6 || t == 11) {
                p = p_t + 3;
            }
            if (t == 7 || t == 12) {
                p = p_t +4;
            }
            
            tilelist[t]->addAddr(addrlist[i]);
            tilelist[t]->addAddr(addrlist[i+1]);
            tilelist[t]->addAddr(addrlist[i+6]);
            tilelist[t]->addAddr(addrlist[i+7]);
            tilelist[t]->addAddr(addrlist[i+12]);
            tilelist[t]->addAddr(addrlist[i+13]);
            
            tilelist[t]->addPath(ptlist[p_t]);
            tilelist[t]->addPath(ptlist[p]);
            tilelist[t]->addPath(ptlist[p+1]);
            tilelist[t]->addPath(ptlist[p+9]);
            tilelist[t]->addPath(ptlist[p+10]);
            tilelist[t]->addPath(ptlist[p_t + 17]);
        }
        else if (t == 1 || t == 2) {
            i = t*2;
            p_t = t +2;
            if (t == 1) {
                p = p_t + 2;
            }
            if (t == 2) {
                p = p_t + 3;
            }
            tilelist[t]->addAddr(addrlist[i]);
            tilelist[t]->addAddr(addrlist[i+1]);
            tilelist[t]->addAddr(addrlist[i+5]);
            tilelist[t]->addAddr(addrlist[i+6]);
            tilelist[t]->addAddr(addrlist[i+11]);
            tilelist[t]->addAddr(addrlist[i+12]);
            
            tilelist[t]->addPath(ptlist[p_t]);
            tilelist[t]->addPath(ptlist[p]);
            tilelist[t]->addPath(ptlist[p+1]);
            tilelist[t]->addPath(ptlist[p+8]);
            tilelist[t]->addPath(ptlist[p+9]);
            tilelist[t]->addPath(ptlist[p_t + 15]);
        }
        else {
            i = t*2+5;
            p_t = t + 36;
            if (t == 16) {
                p = p_t + 3;
            }
            if (t == 17) {
                p = p_t + 4;
            }
            tilelist[t]->addAddr(addrlist[i]);
            tilelist[t]->addAddr(addrlist[i+1]);
            tilelist[t]->addAddr(addrlist[i+6]);
            tilelist[t]->addAddr(addrlist[i+7]);
            tilelist[t]->addAddr(addrlist[i+11]);
            tilelist[t]->addAddr(addrlist[i+12]);
            
            tilelist[t]->addPath(ptlist[p_t]);
            tilelist[t]->addPath(ptlist[p]);
            tilelist[t]->addPath(ptlist[p+1]);
            tilelist[t]->addPath(ptlist[p+8]);
            tilelist[t]->addPath(ptlist[p+9]);
            tilelist[t]->addPath(ptlist[p_t + 15]);
        }
    }
    //cout << "3" << endl;
}

void Board::print() {
  //  cout << "Board::print" << endl;
    td->print();
}

void Board::buildResidence(int addr, shared_ptr<Player> p) {
    //cout << "step1: go to Address class" << endl;
    ResidenceType rt = addrlist[addr]->buildResidence(p);
    td->modifyText(addr, p->getnam(), rt);
}

void Board::buildRoad(int pt, shared_ptr<Player> p) {
    ptlist[pt]->buildRoad(p);
    td->modifyText(pt, p->getnam());
}

void Board::improve(int addr, shared_ptr<Player> p) {
    //buildResidence(addr, p);
    //improveRes(p);
    ResidenceType rt = addrlist[addr]->improveRes(p);
    td->modifyText(addr, p->getnam(), rt);
}

void Board::getResource(int val) {
    for (auto &n: tilelist) {
        if (n->getVal() == val) {
            //cout << val << endl;
            n->getResource();
        }
    }
}
string Board::placeGeese(int tile, shared_ptr<Player> p) {
    int geeseOld = tile;
    if (g->whereIsit() != -1) {
        tilelist[g->whereIsit()]->setGeese(p);
        geeseOld = g->whereIsit();
    }
    g->moveGeese(tile);
    td->modifyText(geeseOld, tile);
    return tilelist[tile]->setGeese(p);
}

void Board::clearBoard() {
    board.clear();
    tilelist.clear();
    addrlist.clear();
    ptlist.clear();
}

void Board::initialBuild(int i, shared_ptr<Player> p) {
    ResidenceType rt = addrlist[i]->initbuild(p);
    //cout << addrlist[i]->getnum() << endl;
    p->addbuildingpoint();
    td->modifyText(i, p->getnam(), rt);
}

void Board::set(shared_ptr<Random> r, shared_ptr<Geese> g) {
    this->r = r;
    this->g = g;
}
