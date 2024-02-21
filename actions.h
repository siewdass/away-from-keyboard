#ifndef ACTIONS_H
#define ACTIONS_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "shared.h"
#include "character.h"
#include "utils.h"
#include "inputs.h"
#include "path.h"
#include "global.h"

using namespace cv;
using namespace std;

class Actions {
public:
    bool onOff = false;
    double potionCooldown = 0,
        autoAttackCooldown = 0,
        healCooldown = 0,
        healCooldown1 = 0,
        healCooldown2 = 0,
        healCooldown3 = 0,
        attackCooldown = 0,
        attackCooldown1 = 0,
        attackCooldown2 = 0,
        attackCooldown3 = 0,
        attackCooldown4 = 0,
        attackCooldown5 = 0,
        attackCooldown6 = 0,
        attackCooldown7 = 0,
        walkCooldown = 0,
        supportCooldown = 0;

    bool single = false;
    bool keepAttacking = false;
    bool avoidWalking = false;

    string old = "";
    int count = 0;
    int runPosition = 0;    
    int walkCoolDown = 0;
    int x = 0, y = 0, z = 0;
    //int position = 0;
    int left = 898, top = 59, pixel = 4;
    Point from, to;

    bool looter( );
    bool walker( Values, vector < Waypoint >, vector < vector < vector < int > > > );
    bool runaway( Values, vector < Waypoint >, vector < vector < vector < int > > > );
    bool lootway( Values, vector < vector < vector < int > > >, Point);
    tuple < bool, string, string, string, string, int, int, int > potions( int, int );
    tuple < bool, string, string, string, string, int, int, int > supporter( Values );
    bool attacker( Values );
    void autoAttack( vector < Point > );
    tuple < int, int > gameWindow( int, int );
    tuple < int, int > mapWindow( int, int );
    Time time;
    Inputs inputs;
    Path path;
    int cout = 0;

    tuple < int, int > avoidWalls( vector < vector < int > > map, int x, int y ) {
        int X = x, Y = y;

        if ( map[ y - 1 ][ x - 1 ] == 0 ) {
            Y += 1;
        } else if ( map[ y - 1 ][ x ] == 0 ) {
            Y += 1;
        } else if ( map[ y - 1 ][ x + 1 ] == 0 ) {
            Y += 1;
        } 

        if ( map[ y ][ x - 1 ] == 0 ) {
            X += 1;
        } else if ( map[ y ][ x + 1 ] == 0 ) {
            X -= 1;
        }

        if ( map[ y + 1 ][ x - 1 ] == 0 ) {
            Y -= 1;
        } else if ( map[ y + 1 ][ x ] == 0 ) {
            Y -= 1;
        } else if ( map[ y + 1 ][ x + 1 ] == 0 ) {
            Y -= 1;
        }
        
        return make_tuple( X, Y );
    }
    void adjustMap( );
};

#endif // ACTIONS_H