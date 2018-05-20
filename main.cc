//
//  main.cpp
//  cs_project5
//
//  Created by Xing Rui on 3/27/17.
//  Copyright © 2017 Xing Rui. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <memory>
#include "Player.h"
#include "Game.h"
#include "Board.h"
using namespace std;

int main(int argc, char *args[]) {
    cin.exceptions(ios::eofbit|ios::failbit);
    //set Game
    string Playersname[4] = {"Blue", "Red", "Orange", "Yellow"};
    Board b;
    vector<shared_ptr<Player>> players;
    for (int i = 0; i < 4; ++i) {
        //shared_ptr<Player> p{new Player{Playersname[i]}};
        auto p = make_shared<Player>(Playersname[i]);
        players.emplace_back(p);
    }
    Game g = Game{&b, players};
    string cmd = "";
    if (argc == 1) cmd = "-random-board";
    else {
        for(int i = 1; i < argc; ++i) {
            cmd = cmd + args[i] + " ";
        }	
    }
    try {
        while (true) {
            //start the game
            g.startGame(cmd);        //initialize Board, testdisplay, players
            if (!g.isLoad()) {
                g.beginningofGame();  //place 2 basements
            }
            //players' turns
            int turn = g.get_cur();
            while (true) {
                try {
                    g.eachTurn(players[turn]);
                }
                catch (string message) {
                    if (message.size() > 7) {
                        cout << message << endl;
                        return 0;
                    }
                    cout << message << " WON!" << endl;
                    break;
                }
                if (turn == 3) turn = 0;
                else ++turn;
            }
            cout << "Would you like to play again?" << endl;
            string ans;
            cin.clear();
            cin >> ans;
            while (ans != "yes" && ans != "no") {
                cout << "Please enter yes or no." <<endl;
                cin.clear();
                cin >> ans;
            }
            if (ans == "yes") g.startAgain();
            else return 0;
        }
    }
    catch (ios::failure &failure) {
        g.saveGame("backup.sv");
    }
    return 0;
}
