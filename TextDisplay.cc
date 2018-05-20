//
//  TextDisplay.cpp
//  cs_project5
//
//  Created by Xing Rui on 4/1/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#include "TextDisplay.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <memory>
#include <string>
#include "ResidenceType.h"
#include "ResourceType.h"
#include "Pos.h"
#include "Info.h"
#include "Tile.h"
using namespace std;

void TextDisplay::initText() {
    //cout << "initt1" << endl;
    ifstream map{"MAP.txt"};
    char c;
    int r = 0;
    //cout << "initt2" << endl;
    while (r < 41) {
        int l = 0;
        vector<char> row;
        map.get(c);
        while (c != '\n') {
            row.emplace_back(c);
            //cout << c ;
            //cout << "row em" << endl;
            //save the position of the middle of the tile
            if ((r - 2) % 4 == 0 && l % 10 == 7) {
                if (c != ' ') {
                    shared_ptr<Pos> p = make_shared<Pos>(r, l);
                    tilePos.emplace_back(p);
                }
            }
            //cout << "level1" << endl;
            ++l;
            map.get(c);
        }
        Text.emplace_back(row);
        ++r;
        //cout << r;
    }
    //cout << "initt3" << endl;
}
    
    void TextDisplay::modifyText(int addr, string player, ResidenceType type) {
        char pl = player[0];
        char tp;
        int r, l;
        if  (type == ResidenceType::Basement) { tp = 'B'; }
        if  (type == ResidenceType::House) { tp = 'H'; }
        if  (type == ResidenceType::Tower) { tp = 'T'; }
        if (addr >= 6 && addr <= 47) {
            r = 8 + ((addr / 6) - 1) * 4;
            l = 1 + (addr % 6) * 10;
        } else if (addr == 0 || addr == 1 || addr == 52 || addr == 53) {
            if (addr < 2) r = 0;
            else r = 40;
            if (addr == 0 || addr == 52) l = 21;
            else l = 31;
        } else if (addr <= 5) {
            r = 4;
            l = (addr % 6) * 10 - 9;
        } else {
            r = 36;
            l = (addr % 6) * 10 + 11;
        }
        Text[r][l] = pl;
        Text[r][l + 1] = tp;
    }
    
    void TextDisplay::modifyText(int path, string player) {
        char pl = player[0];
        char tp = 'R';
        int r, l;
        if (path >= 9 && path <= 62) {
            int q = (path + 8) / 17;
            int w = (path + 8 - 17 * q);
            if (w < 3) {
                r = 8 + (q - 1) * 8;
                l = 6 + w * 20;
            } else if (w == 9 || w == 10) {
                r = 8 + (q - 1) * 8 + 4;
                l = 16 + (w - 9) * 20;
            } else {
                r = 8 + (q - 1) * 8 + 2 + ((w-1) / 8) * 4;
                l = 1 + ((w - 3) % 8) * 10;
            }
        } else if (path == 0 || path == 71) {
            l = 26;
            if (path == 0) r = 0;
            else r = 40;
        } else {
            int fir;
            int ini;
            int block = 10;
            if (path == 1 || path == 2 || path == 69 || path == 70) {
                ini = 21;
                if (path <= 2) { r = 2; fir = 1; }
                else { r = 38; fir = 69; }
            } else if (path == 3 || path == 4 || path == 67 || path == 68) {
                ini = 16;
                block = 20;
                if (path <= 4) { r = 4; fir = 3; }
                else { r = 36; fir = 67; }
            } else {
                ini = 11;
                if (path <= 8) { r = 6; fir = 5; }
                else { r = 34; fir = 63; }
            }
            l = ini + (path - fir) * block;
        }
        Text[r][l] = pl;
        Text[r][l + 1] = tp;
    }
    
    void TextDisplay::modifyText(shared_ptr<Tile> t, int tile) {
        Info i = t->getInfo();
        char beg = '0';
        // get the number of the tile
        int val = i.val; //get the value of the tile
        //find the location of the tile in Text
        //cout<< tile << "fxxk" << tilePos.size() << endl;
        int r = tilePos[tile]->r;
        int l = tilePos[tile]->c;
        //add Resource Type of the tile to Text
        if (i.rt == ResourceType::Brick) {
            Text[r + 1][l - 2] = 'B';
            Text[r + 1][l - 1] = 'R';
            Text[r + 1][l] = 'I';
            Text[r + 1][l + 1] = 'C';
            Text[r + 1][l + 2] = 'K';
        } else if (i.rt == ResourceType::Energy) {
            Text[r + 1][l - 3] = 'E';
            Text[r + 1][l - 2] = 'N';
            Text[r + 1][l - 1] = 'E';
            Text[r + 1][l] = 'R';
            Text[r + 1][l + 1] = 'G';
            Text[r + 1][l + 2] = 'Y';
        } else if (i.rt == ResourceType::Glass) {
            Text[r + 1][l - 2] = 'G';
            Text[r + 1][l - 1] = 'L';
            Text[r + 1][l] = 'A';
            Text[r + 1][l + 1] = 'S';
            Text[r + 1][l + 2] = 'S';
        } else if (i.rt == ResourceType::Heat) {
            Text[r + 1][l - 2] = 'H';
            Text[r + 1][l - 1] = 'E';
            Text[r + 1][l] = 'A';
            Text[r + 1][l + 1] = 'T';
        } else if (i.rt == ResourceType::Wifi) {
            Text[r + 1][l - 2] = 'W';
            Text[r + 1][l - 1] = 'I';
            Text[r + 1][l] = 'F';
            Text[r + 1][l + 1] = 'I';
        } else if (i.rt == ResourceType::Park) {
            Text[r + 1][l - 2] = 'P';
            Text[r + 1][l - 1] = 'A';
            Text[r + 1][l] = 'R';
            Text[r + 1][l + 1] = 'K';
        }
        //add value of the tile to Text
        if (val != 7) {
            if (val >= 10) Text[r + 2][l - 1] = beg + val / 10;
            Text[r + 2][l] = beg + val % 10;
        }
    }
void TextDisplay::modifyText(int geeseOld, int geeseNew) {
    //cout << geeseOld << " to " << geeseNew << endl;
    int r1 = tilePos[geeseOld]->r;
    int l1 = tilePos[geeseOld]->c;
    int r2 = tilePos[geeseNew]->r;
    int l2 = tilePos[geeseNew]->c;
    
    
    Text[r1 + 3][l1 - 2] = ' ';
    Text[r1 + 3][l1 - 1] = ' ';
    Text[r1 + 3][l1] = ' ';
    Text[r1 + 3][l1 + 1] = ' ';
    Text[r1 + 3][l1 + 2] = ' ';
    
    Text[r2 + 3][l2 - 2] = 'G';
    Text[r2 + 3][l2 - 1] = 'E';
    Text[r2 + 3][l2] = 'E';
    Text[r2 + 3][l2 + 1] = 'S';
    Text[r2 + 3][l2 + 2] = 'E';
}

void TextDisplay::print() {
  //cout << "TextDisplay::print()" << endl;
    int len = Text.size();
    for (int i = 0; i < len; ++i) {
      //cout<< len <<endl;
        int len1 = Text[i].size();
        for (int j = 0; j < len1; ++j) {
	  //cout <<len1 << endl;
            cout << Text[i][j];
        }
        cout << endl;
    }
}
