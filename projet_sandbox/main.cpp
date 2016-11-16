//  main.cpp
//  projet_sandbox
//
//  Created by David Boyker on 02/11/2016.
//  Copyright © 2016 David Boyker. All rights reserved.
//
// Determine all the possible chains of proteins


#include <iostream>
#include <fstream>
#include <time.h>
#include <iostream>
#include <string>
#include <set>
#include <array>
#include <utility>
#include <random>
#include <fstream>
#include <sstream>
#include <climits>
#include <chrono>
#include <thread>
#include <iterator>
#define n 12
#define N 2*n-1
using namespace std;
using std::cout;
using std::endl;


// ----- global variables -----
int grid_size = 30;
float width = 92.0;
float height = 110.0;
char grid[N][N];
int position[N][2];
vector<string> chains;


// ----- chains' building -----


void print_grid(char grid[N][N]) {
    for (int col = 0; col < 2*N+1; col ++) {
        cout << '-';
    }
    cout << endl;
    for (int row = 0; row < N; row ++) {
        cout << '|';
        for (int col = 0; col < N; col ++) {
            cout << grid[row][col] << " ";
        }
        cout << '|' << endl;
    }
    for (int col = 0; col < 2*N+1; col ++) {
        cout << '-';
    }
    cout << endl;
}


int is_position_available(int i, int x, int y) {
    // check for occupied
    if (grid[x][y] != ' ') {
        return 0;  // occupied
    }
    return 1;
}


int get_position_score(char type ,int x, int y, int last_x, int last_y) {
    int score = 0;
    if (type == 'P') {
        return 0;
    }
    int directions[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (int i = 0; i < 4; i++) {
        int next_x = directions[i][0] + x;
        int next_y = directions[i][1] + y;
        if (next_x != last_x && next_y != last_y) {
            if (grid[next_x][next_y] == type) {
                score += 1;
            }
        }
    }
    return score;
}


void evaluate_chain(string chain, int i, int current_x, int current_y, int* max_score, int score) {
    if (i == chain.length()) {  // tous les éléments de la chaîne sont placés
        if (score > *max_score) {
            *max_score = score;
            //print_grid(grid);
        }
    }
    else {
        int directions[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (int j = 0; j < 4; j++) {
            int new_x = current_x + directions[j][0];
            int new_y = current_y + directions[j][1];
            if (is_position_available(i, new_x, new_y) == 1) {
                int new_score = score + get_position_score(chain[i], new_x, new_y, current_x, current_y);
                grid[new_x][new_y] = chain[i];  // set protein
                position[i][0] = new_x;
                position[i][1] = new_y;
                evaluate_chain(chain, i+1, new_x, new_y, max_score, new_score);  // continue
                grid[new_x][new_y] = ' ';  // track back
                position[i][0] = 0;
                position[i][1] = 0;
            }
        }
    }
}


int get_max_score(string chain) {
    int max_score = 0;
    int* positions[chain.length()][2];
    // initialize la grid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = ' ';
        }
    }
    // on place le premier élément
    grid[n-1][n-1] = chain[0];
    // initialize les positions
    for (int i = 0; i < N; i++) {
        positions[i][0] = 0;
        positions[i][1] = 0;
    }
    // lancement de la procédure récursive
    evaluate_chain(chain, 1, n-1, n-1, &max_score, 0);
    return max_score;
}


void get_chains_from_file() {
    ifstream fichier ("./liste.txt");
    if(fichier.is_open()) {
        string mot;
        while(fichier >> mot) {
            chains.push_back(mot);
        }
        fichier.close();
    }
    else {
        cout << "Impossible d'ouvrir le fichier" << endl;
    }
}


int main(int argc, char * argv[]) {
    get_chains_from_file();
    for(vector<int>::size_type i = 0; i != chains.size(); i++) {
        cout << "chain: " << chains[i] << endl;
        int max_score = get_max_score(chains[i]);
        cout << "max score: " << max_score << endl;
        cout << "--------------------" << endl;
    }
    return 0;
}


// ---------------------------------------------------------------------------
// Entrée: lecture d'un fichier txt, gestion des erreurs?
// Quid des situations mirroirs?
// Amélioration de la function is_position_available
// calcul du score pendant la function test, quelle structure de donnée?
