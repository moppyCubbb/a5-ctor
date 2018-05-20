#include "Game.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "DiceType.h"
#include "Board.h"
#include "Geese.h"
using namespace std;


Game::Game(Board *b, vector<shared_ptr<Player>> &players)
: b{b}, players{players}, current_turn{0}, load{false}, rev{false} {
    r = make_shared<Random>();
    g = make_shared<Geese>();
    b->set(r, g);
    for (auto &p: players) {
        p->set(r);
    }
}

void Game::startGame(string cmds) {
    //string cmd;
    //cout << "Please input a command, input done to start the game!" << endl;
    //cin.clear();
    //cin >> cmd;
    istringstream iss{cmds};
    string cmd;
    //    cout << cmds << endl;
    while (iss >> cmd) {
      //cout << cmd << endl;
        if (cmd == "-seed") {
            unsigned int seed;
            iss.clear();
	    //	    cout << "here" << endl;
            iss >> seed;
	    //cout << seed << endl;
//random seed lets see if we need a class to store the randam method
            r->setSeed(seed);
	    //cout << "here" << endl;
        } else if (cmd == "-load") {
            string file;
            iss.clear();
            iss >> file;
            loadGame(file);
        } else if (cmd == "-board") {
            string file;
            iss.clear();
            iss >> file;
            loadBoard(file);
        } else if (cmd == "-random-board") {
            b->randomBoard();
        }
        //else cout << "Invalid command." << endl;
        //cin.clear();
	//	cout << "finished" << endl;
    }
    //    cout << "here" << endl;
    //cout << b << endl;
    b-> print();
    //    cout << "here" << endl;
}

void Game::loadGame(string file) {
    //cout << "load success!" << endl;
    ifstream f{file};
    string data;
    getline(f,data);
    istringstream iss{data};
    iss >> current_turn;
    string boardinfo = "";
    for (int i = 0; i < 4; ++i) {
        getline(f, data);
        boardinfo = boardinfo + " " + players[i]->getnam() + " ";
        int pos = 0;
        string c;
        while (pos < data.size()) {
            c = data[pos];
            if (c == "r" ) break;
            else ++pos;
        }
        boardinfo = boardinfo + data.substr(pos) + " -1";
        players[i]->loadPlayer(data);
    }
    getline(f, data);
    boardinfo = data + boardinfo;
    b->loadBoard(boardinfo, players);
    load = true;
    if (current_turn == 3) {
        if (players[3]->seeReslen() == 1) {
           load = false;
           rev = true;
        }
    } else if (players[current_turn + 1]->seeReslen() == 2) {
        load = false;
        rev = true;
    } else if (players[current_turn]->seeReslen() == 1 && 
               players[current_turn + 1]->seeReslen() == 0) {
        ++current_turn;
        load = false;
        rev = false;
    } else if (players[current_turn]->seeReslen() == 0) {
        load = false;
        rev = false;
    }
 /*   if (players[3]->seeReslen() == 1) {
        current_turn = 3;
        load = false;
        rev= true;
    } else {
        for (int i = 0; i < 3; ++i) {
            if (players[i]->seeReslen() < 2) {
                if (players[i + 1] == 0) {
                    current_turn = i;
                    load = false;
                    rev = false;
                    break;
                } else if (players[i + 1]->seeReslen() == 2) {
                    current_turn = i;
                    rev = true;
                    break;
                }
            }
        }
    }*/
}

//load board from file
void Game::loadBoard(string file) {
    ifstream f{file};
    string data;
    getline(f, data);
    b->loadBoard(data, players);
}

//save game to file
void Game::saveGame(string file) {
    ofstream f{file};
    f << current_turn << endl;
    for (int i = 0; i < 4; ++i) {
        f << players[i]->savePlayer();
    }
    f << b->saveBoard();
}

//beginning of the game
void Game::beginningofGame() {
    while (current_turn >= 0) {
        shared_ptr<Player> p = players[current_turn];
        int addr;
        cout <<"Builder " << p->getnam();
        cout  << ", where do you want to build a basement?" << endl;
        cin.clear();
        cin >> addr;
        while (addr < 0 || addr > 53) {
            cout << "Out of range." << endl;
            cin.clear();
            cin >> addr;
        }
        while (addr >= 0 && addr <= 53) {
            try {
                b->initialBuild(addr, p);
                break;
            }
            catch (string w) {
                cout << w << endl;
            }
            cin.clear();
            cin >> addr;
        }
        
        if (current_turn == 3 && !rev) {
            rev = true;
        } else if (rev) {
            --current_turn;
        } else {
            ++current_turn;
        }
    }
    current_turn = 0;
    b->print();
}

//during one player's turn
void Game::eachTurn(shared_ptr<Player>p) {
    //beginning of turn (rolling dice)
    cout << "Builder " << p->getnam() <<"'s turn." << endl;
    p->getStatus();
    string s;
    cout << "Set your dice:" <<endl;
    cin.clear();
    cin >> s;
    DiceType dt;
    while (s != "load"  && s != "fair") {
        cout << "Invalid command." << endl;
        cin.clear();
        cin >> s;
    }
    if (s == "load") dt = DiceType::Loaded;
    else if (s == "fair") dt = DiceType::Fair;
    cin >> s;
    while (s != "roll") {
        cout << "Invalid command." << endl;
        cin >> s;
    }
    int diceval = p->roll(dt);
    if (diceval != 7) {
        b->getResource(diceval); //print message???
    } else {
        for (int i = 0; i < 4; ++i) {
            players[i]->lost();
        }
        cout << "Choose where to place the GEESE." << endl;
        int address = 0;
        cin.clear();
        cin >> address;
        while (cin.fail() || address == g->whereIsit()) {
            if (cin.fail()) {
                cout << "Invalid Command" << endl;
                cin.clear();
                cin.ignore();
                cin >> address;
            }
            if (!cin.fail() && !(address >= 0 && address <= 18)){
                cout << "out of range";
                cin.clear();
                cin >> address;
                continue;
            }
            if (!cin.fail() && address == g->whereIsit() ) {
                cout << "You need to move the GEESE to somewhere else." << endl;
                cout << "Choose where to place the GEESE." << endl;
                cin.clear();
                cin >> address; continue;
            }
        }
        //cin >> address;//what if fail
        
        try {
            
            string player = b->placeGeese(address, p);
            int No = pntoint(player);
            int type;
            try {
                type = players[No]->steal();
            }
            catch (string n) { throw n; }
            ResourceType rt = inttort(type);
            p->modifyResource(rt, 1, 1);//successful steal
            string resource = inttostr(type);
            cout << "Builder " << p->getnam() << " steals " << resource << " from " << player << "." << endl;
        }
        catch (string n) {
            if (n.size() > 7) cout << n << endl;
            else cout << "Builder " << n << " has no builders to steal from." << endl;
        }
    }
    
    
    //during the turn
    cout << "Enter your command." << endl;
    cin.clear();
    cin >> s;
    
    while (s != "next") { //next player's turn
        if (s == "board") { b->print(); } //print the board
        else if (s == "status") { //get the status of each player
            for (int i = 0; i < 4; ++i) {
                players[i]->getStatus();
            }
        } else if (s == "residences") { //print out the res info of the current player
            p->getResidence();
        } else if (s == "build-road") { //build road
            int path;
            cin.clear();
            cin >> path;
            while (path < 0 || path > 71) {
                cin.clear();
                cin >> path;
            }
            try{
                b->buildRoad(path, p);
                //cout << "game build path success!" << endl;
            }
            catch (string n) {
                cout << n << endl;
            }
        } else if (s == "build-res") { //build residence
            int addr;
            cin.clear();
            cin >> addr;
            while (addr < 0 || addr > 53) {
                cin.clear();
                cin >> addr;
            }
            try {
                b->buildResidence(addr, p);
            }
            catch (string n) {
                cout << n << endl;
            }
        } else if (s == "improve") { //improve residence
            int addr;
            cin.clear();
            cin >> addr;
            try {
                b->improve(addr,p);
            }
            catch (string n) {
                cout << n << endl;
            }
        } else if (s == "trade") { //trade with another player
            string player, give, take, ans;
            cin.clear();
            cin >> player >> give >> take;
            ResourceType gt, tt;
            while (true) {
                try {
                    gt = stoRt(give);
                    tt = stoRt(take);
                    break;
                }
                catch (string warning) {
                    cout << warning << endl;
                }
                cin.clear();
                cin >> give >> take;
            }
            
            cout << p->getnam() << " offers " << player;
            cout << " one " << give << " for one " << take << "." << endl;
            cout << "Does " << player << " accept this offer?" << endl;
            cin.clear();
            cin >> ans;
            while (ans != "yes" && ans != "no") {
                cout << "Please enter yes or no." <<endl;
                cin.clear();
                cin >> ans;
            }
            
            shared_ptr<Player> p1 = players[pntoint(player)];
            if (ans == "yes") {
                gt = stoRt(give);
                tt = stoRt(take);
                p->tradeResource(gt, tt, p, p1);
            }
        } else if (s == "save") {
            string file;
            cin.clear();
            cin >> file;
            saveGame(file);
            string message = "save file done";
            throw message;
            //save the game to <file>
        } else if (s == "help") {
            cout << "Valid commands:\nboard\nstatus\nresidences" << endl;
            cout << "build-road <path#>\nbuild-res <housing#>" << endl;
            cout << "improve <housing#>\ntrade <colour> <give> <taken>" << endl;
            cout << "next\nsave <file>\nhelp" << endl;
        } else {
            cout << "Invalid command" << endl;
        }
        //check isWon
        if (p->isWon()) {
            throw p->getnam();
        }
        cin.clear();
        cin >> s;
    }
    if (current_turn == 3) current_turn = 0;
    else ++current_turn;
}

ResourceType Game::stoRt(string rname) {
    if (rname == "BRICK") {
        return ResourceType::Brick;
    } else if (rname == "ENERGY") {
        return ResourceType::Energy;
    } else if (rname == "GLASS") {
        return ResourceType::Glass;
    } else if (rname == "HEAT") {
        return ResourceType::Heat;
    } else if (rname == "WIFI") {
        return ResourceType::Wifi;
    } else {
        throw "Please enter a valid resource.";
    }
}

void Game::startAgain() {
    b->clearBoard();
    for (auto &p: players) {
        p->clearPlayer();//clear player
    }
    current_turn = 0;
    load = false;
    b->randomBoard();
}


int Game::pntoint(string player) {
    if (player == "Blue") {
        return 0;
    } else if (player == "Red") {
        return 1;
    } else if (player == "Orange") {
        return 2;
    } else {
        return 3;
    }
}

ResourceType Game::inttort(int n) {
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

int Game::get_cur() { return current_turn; }

string Game::inttostr(int n) {
    string rt;
    if (n == 0) rt = "BRICK";
    else if (n == 1) rt = "ENERGY";
    else if (n == 2) rt = "GLASS";
    else if (n == 3) rt = "HEAT";
    else rt = "WIFI";
    return rt;
}

bool Game::isLoad() {
    return load;
}
