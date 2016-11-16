//  main.cpp
//  projet_sandbox
//
//  Created by David Boyker on 02/11/2016.
//  Copyright © 2016 David Boyker. All rights reserved.
//
// Determine all the possible chains of proteins


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
#define n 10
#define N 2*n-1
using namespace std;
using std::cout;
using std::endl;
// ----- global variables -----
const int TIMERMSECS = 40;
int grid_size = 30;
float width = 92.0;
float height = 110.0;
char grid[N][N];




// ----- graphic interface -----

void init(float r, float g, float b)
{
    glClearColor(r,g,b,0.0);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0.0, width, 0.0, height);  // (left, right, bottom, top) dimension of the frame
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glViewport(100.0,100.0,500.0,500.0); // window's dimension
    glPointSize(1.0f);
    //draw title
    stringstream ss1;
    ss1<<"HP proteins - Eadi";
    glRasterPos2i(27, width);
    for (int i=0; i<ss1.str().size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ss1.str().at(i));
    }
    float factor = width/grid_size;
    //draw grid
    for(int i = 0; i <= grid_size; i ++)
    {
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex3f((float)i*factor, 0, 0.0);// vertical lines
        glVertex3f((float)i*factor, width, 0.0);// vertical lines
        glVertex3f(0, (float)i*factor, 0.0);  // horizontal lines
        glVertex3f(width, (float)i*factor, 0.0);  // horizontal lines
        glEnd();
        glLineWidth(1.0f);
    }
    //draw numbers
    div_t coord;
    string str;
    stringstream ss2;
    for(int i=0;i<grid_size;i++) {
        for (int j=0;j<grid_size;j++) {
            ss2<<grid[i][j];
        }
    }
    str=ss2.str();
    for (int i=0; i<str.size(); i++)
    {
        coord=div(i,9);
        if(str.substr(i,1)!="0")
        {
            glColor3f(0.0,0.0,0.0);
            glRasterPos2i(coord.rem*factor+5,coord.quot*factor+5);
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str.at(i));
        }
    }
    glutSwapBuffers();
}


void animate(int val)
{
    glutTimerFunc(TIMERMSECS,animate,0);
    draw();
}


// ----- chains' building -----

void open_file() {
    ifstream fichier ("./liste.txt");
    if(fichier.is_open()) {
        string mot;
        while(fichier >> mot)
            cout << mot.length() << "Chain: " << mot << endl;
        fichier.close();
    }
    else {
        cout << "Impossible d'ouvrir le fichier" << endl;
    }
}


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


void build_chain(int i, int current_x, int current_y) {
    if (i == n) {  // tous les éléments de la chaîne sont placés
        print_grid(grid);
    }
    else {
        int directions[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (int j = 0; j < 4; j++) {
            int new_x = current_x + directions[j][0];
            int new_y = current_y + directions[j][1];
            if (is_position_available(i, new_x, new_y) == 1) {
                //int score = get_position_score(chain[i], new_x, new_y, current_x, current_y);
                //grid[new_x][new_y] = chain[i];  // set protein
                //chain_score += score;
               // build_chain(chain, p_to_possibilities, p_to_best_score, i+1, new_x, new_y);  // continue
              //  grid[new_x][new_y] = ' ';  // track back
            //    chain_score -= score;
            }
        }
    }
}



void build_chain_main() {
    //open_file();
    int best_score = 0;
    // input: chain
    char chain[] = {'h','p','h','p','h','p','h','p','h','h'};
    // positions
    // on initialize la grid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = ' ';
        }
    }
    // on place le premier élément
    grid[n-1][n-1] = chain[0];
    // lancement de la procédure récursive
    int possibilities = 0;
    build_chain(1, n-1, n-1);
}


int mainxx(int argc, char * argv[]) {
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (700, 700);
    glutInitWindowPosition (200, 200);
    glutCreateWindow ("Eadi");
    init(255.0,255.0,255.0);
    thread t(build_chain_main);
    glutTimerFunc(TIMERMSECS,animate,0);
    glutDisplayFunc(draw);
    glutMainLoop();
    t.join();
    return 0;
}


// ---------------------------------------------------------------------------
// Entrée: lecture d'un fichier txt, gestion des erreurs?
// Quid des situations mirroirs?
// Amélioration de la function is_position_available
// calcul du score pendant la function test, quelle structure de donnée?
