//
//  main.cpp
//  projet_sandbox
//
//  Created by David Boyker on 02/11/2016.
//  Copyright © 2016 David Boyker. All rights reserved.
//

// Determine all the possible chains with a fixed number of proteins (3)

#include <GLUT/glut.h>
#ifdef OSX
#include <GLUT/glut.h>
#elif LINUX
//#include <GL/glut.h>
#endif
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

using namespace std;
using namespace std;
using std::cout;
using std::endl;
#define n 10
#define N 2*n-1


void open_file() {
    ifstream fichier ("./liste.txt");
    if(fichier.is_open()) {
        string mot;
        while( fichier >> mot )
            cout << mot.length() << "Chain: " << mot << endl;
        fichier.close();
    }
    else {
        cout << "Impossible d'ouvrir le fichier" << endl;
    }
}


void print_grid(char grid[N][N], int score) {
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
    cout << "Score: " << score << endl;
}


int is_position_available(char grid[N][N], int i, int x, int y) {
    // check for occupied
    if (grid[x][y] != ' ') {
        return 0;  // occupied
    }
    return 1;
}


int is_position_score(char grid[N][N], char chain[], int i) {
    //protein = chain[i]
    return 0;
}


void test(char grid[N][N], char chain[], int *p_to_possibilities, int *p_to_best_score, int i, int current_x, int current_y) {
    if (i == n) {  // tous les éléments de la chaîne sont placés
        //print_grid(grid, *p_to_best_score);
        *p_to_possibilities += 1;
    }
    else {
        int directions[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (int j = 0; j < 4; j++) {
            int new_x = current_x + directions[j][0];
            int new_y = current_y + directions[j][1];
            if (is_position_available(grid, i, new_x, new_y) == 1) {
                grid[new_x][new_y] = chain[i];  // set protein
                test(grid, chain, p_to_possibilities, p_to_best_score, i+1, new_x, new_y);  // continue
                grid[new_x][new_y] = ' ';  // track back
            }
        }
    }
}


int main(int argc, const char * argv[]) {
    open_file();
    int best_score = 0;
    clock_t tStart = clock();
    // input: chain
    char chain[] = {'h','p','h','p','h','p','h','p','h','h'};
    // positions
    char grid[N][N];
    // on initialize la grid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = ' ';
        }
    }
    // on place lep remier élément
    grid[n-1][n-1] = chain[0];
    // lancement de la procédure récursive
    int possibilities = 0;
    test(grid, chain, &possibilities, &best_score, 1, n-1, n-1);
    // when done, print the number of grid generated = number of possible arrangement
    cout << "Possibilities: " << possibilities << endl;
    cout << "Best score: " << best_score << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}


    
// ---------------------------------------------------------------------------
// Entrée: lecture d'un fichier txt, gestion des erreurs?
// Quid des situations mirroirs?
// Amélioration de la function is_position_available
// calcul du score pendant la function test, quelle structure de donnée?
// choix de l'implémentation graphique
