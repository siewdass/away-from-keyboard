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
Mat HP, MP, BD, FK, minimap, BATTLE, CHAT, LOOTCHAT, WEAPON, RING, AMULET, CAPACITY;

vector < Point > Flask;

vector < Pixel > HealthPoints;
vector < Pixel > ManaPoints;
vector < Pixel > BenefitsAndDamages;
vector < Pixel > battle;
vector < Pixel > chat;
vector < Pixel > lootchat;
vector < Pixel > weapon;
vector < Pixel > ring;
vector < Pixel > amulet;
vector < Pixel > capacity;

Pixels TEST; // ARRAY OF PIXELS

bool AlarmCount = 0;

tuple < int, int, bool, bool, bool, bool, int, int, int, string, vector < Point >, bool, bool, bool, bool, bool > Status::update( vector < Position > positions ) {

    CAPACITY = screenshot.get( id, 919, 255, 11, 1 );
    capacity = acknow.inspect( CAPACITY );

    int cap = 0;
    bool lowcap = false;

    for ( Pixel pixel : capacity ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r <= 55 && g <= 55 && b <= 55 ) {
            cap += 1;   
        }
    }

    if ( cap >= 11 ) {
        lowcap = true;
    }

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

    bool amuletEquipped = true;

    AMULET = screenshot.get( id, 862, 158, 1, 1 );
    amulet = acknow.inspect( AMULET );

    for ( Pixel pixel : amulet ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 71 && g == 73 && b == 76 ) {
            amuletEquipped = false;
            break;
        }
    }

    bool ringEquipped = true;

    RING = screenshot.get( id, 861, 220, 1, 1 );
    ring = acknow.inspect( RING );

    for ( Pixel pixel : ring ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 88 && g == 91 && b == 94 ) {
            ringEquipped = false;
            break;
        }
    }

    bool alarm = false;

    BATTLE = screenshot.get( id, 26, 30, 1, 1 );
    battle = acknow.inspect( BATTLE );

    for ( Pixel pixel : battle ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 0 && g == 0 && b == 0 ) {
            AlarmCount += 1;
            break;
        }
    }

    if ( AlarmCount >= 4 ) {
        alarm = true;
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

    imwrite( "screenshot.png", CAPACITY );

    return make_tuple(
        health,
        mana,
        hungry,
        paralyze,
        poison,
        haste,
        x, y, z,
        map,
        Flask,
        alarm,
        weaponEquipped,
        amuletEquipped,
        ringEquipped,
        lowcap
    );
}

 bool Status::valuableLoot( ) {

    bool valuableLoot = false;

	LOOTCHAT = screenshot.get( id, 509, 575, 305, 42 );
    lootchat = acknow.inspect( LOOTCHAT );

    for ( Pixel pixel : lootchat ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 0 && g == 240 && b == 0 || r == 32 && g == 160 && b == 255 || r == 255 && g == 104 && b == 255 || r == 240 && g == 240 && 0 ) {
            valuableLoot = true;
            break;
        }
    }

    return valuableLoot;
}