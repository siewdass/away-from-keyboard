#include "status.h"
#include "utils.h"
#include "screenshot.h"
#include "acknow.h"
#include "converter.h"
#include <vector>
#include <iostream>

using namespace std;

Screenshot screenshot;
Converter converter;
Acknow acknow;
Mat HP, MP, BD, FK, minimap, BATTLE, CHAT, WEAPON;
vector < Pixel > HealthPoints;
vector < Pixel > ManaPoints;
vector < Pixel > BenefitsAndDamages;
vector < Point > Flask;
vector < Pixel > battle;
vector < Pixel > chat;
vector < Pixel > weapon;

tuple < int, int, bool, bool, bool, bool, int, int, int, string, vector < Point >, bool, bool > Status::update( vector < Position > positions ) {

    bool weaponEquipped = false;

    WEAPON = screenshot.get( id, 861, 186, 1, 1 );
    weapon = acknow.inspect( WEAPON );

    for ( Pixel pixel : weapon ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 94 && g == 96 && b == 98 ) {
            weaponEquipped = true;
            break;
        }
    }

    bool alarm = false;

    BATTLE = screenshot.get( id, 26, 30, 1, 1 );
    battle = acknow.inspect( BATTLE );

    for ( Pixel pixel : battle ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 0 && g == 0 && b == 0 ) {
            alarm = true;
            break;
        }
    }

    CHAT = screenshot.get( id, 189, 575, 1, 42 );
    chat = acknow.inspect( CHAT );

    for ( Pixel pixel : chat ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 96 && g == 248 && b == 248 ) {
            alarm = true;
            break;
        }
    }

    HP = screenshot.get( id, 188, 5, 267, 1 );
    HealthPoints = acknow.inspect( HP );
    health = 267;

    for ( Pixel pixel : HealthPoints ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r >= 32 && r <= 46 && g >= 32 && g <= 46 && b >= 32 && b <= 46 ) {
            health -= 1;
        }
    }

    MP = screenshot.get( id, 539, 5, 267, 1 );
    ManaPoints = acknow.inspect( MP );
    mana = 0;

    for ( Pixel pixel : ManaPoints ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 0 & g == 41 & b == 91 ) {
            mana += 1;
        } else if ( r == 0 & g == 44 & b == 91 ) {
            mana += 1;
        }
    }

    BD = screenshot.get( id, 480, 4, 53, 27 );
    BenefitsAndDamages = acknow.inspect( BD );
    hungry = false;
    paralyze = false;
    poison = false;
    haste = false;

    for ( Pixel pixel : BenefitsAndDamages ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r >= 220 & r <= 250 & g >= 200 & g <= 220 & b >= 141 & b <= 160 ) {
            hungry = true;
        } else if ( r == 255 & g == 0 & b == 0 ) {
            paralyze = true;
        } else if ( r >= 60 & r <= 100 & g >= 190 & g <= 255 & b >= 80 & b <= 165 ) {
            poison = true;
        } else if ( r >= 60 & r <= 100 & g >= 190 & g <= 255 & b >= 80 & b <= 165 ) {
            poison = true;
        } else if ( r == 176 && g == 139 && b == 80 ) {
            haste = true;
        }
    }

    FK = screenshot.get( id, 836, 304, 176, 296 );
    Flask = acknow.flask( FK );
    /*isTargeting = false;

    for ( Pixel pixel : Battle ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 255 & g == 0 & b == 0 ) {
            isTargeting = true;
            break;
        } else if ( r == 255 & g == 128 & b == 128 ) {
            isTargeting = true;
            break;
        }
    }*/

    Mat MAP = screenshot.get( id, 845, 5, 106, 109 );

    string map = converter.matToBase64( MAP );

    int x = 0, y = 0, z = 0;

    for ( int i = 0; i < positions.size( ); i++ ) {
        //std::cout << "\nAAAAA\n" << map << "\nBBBBB\n" << positions[ i ].img << "\nCCCCC\n" << std::endl;
        if ( map == positions[ i ].img ) {
            x = positions[ i ].x;
            y = positions[ i ].y;
            z = positions[ i ].z;
        }
    }
    
    //simwrite( "screenshot.png", MAP );

    return make_tuple( health, mana, hungry, paralyze, poison, haste, x, y, z, map, Flask, alarm, weaponEquipped );
}