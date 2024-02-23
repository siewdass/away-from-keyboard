#include "acknow.h"

#include "stats.h"
#include <vector>

using namespace std;

Acknow acknows;

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

int AlarmCount = 0;

tuple < int, int, bool, bool, bool, bool, vector < Point >, bool, bool, bool, bool, bool > Stats::update( Mat CAPACITY, Mat WEAPON, Mat AMULET, Mat RING, Mat BATTLE, Mat CHAT, Mat HP, Mat MP, Mat BD, Mat FK ) {

    //1012 642
    capacity = acknows.inspect( CAPACITY );

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

    weapon = acknows.inspect( WEAPON );

    for ( Pixel pixel : weapon ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 94 && g == 96 && b == 98 ) {
            weaponEquipped = true;
            break;
        }
    }

    bool amuletEquipped = true;

    amulet = acknows.inspect( AMULET );

    for ( Pixel pixel : amulet ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 71 && g == 73 && b == 76 ) {
            amuletEquipped = false;
            break;
        }
    }

    bool ringEquipped = true;

    ring = acknows.inspect( RING );

    for ( Pixel pixel : ring ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 88 && g == 91 && b == 94 ) {
            ringEquipped = false;
            break;
        }
    }

    bool alarm = false;

    battle = acknows.inspect( BATTLE );

    for ( Pixel pixel : battle ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 0 && g == 0 && b == 0 ) {   
            //std::cout << r << " " << g << " " << b << std::endl;         
            alarm = true;
        }
    }

    if ( AlarmCount >= 4 ) { AlarmCount = 0; alarm = true; }

    chat = acknows.inspect( CHAT );

    for ( Pixel pixel : chat ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 96 && g == 248 && b == 248 ) {
            alarm = true;
            break;
        }
    }

    HealthPoints = acknows.inspect( HP );
    health = middle-188-40;

    for ( Pixel pixel : HealthPoints ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r >= 32 && r <= 46 && g >= 32 && g <= 46 && b >= 32 && b <= 46 ) {
            health -= 1;
        }
    }

    ManaPoints = acknows.inspect( MP );
    mana = 0;

    for ( Pixel pixel : ManaPoints ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 0 & g == 41 & b == 91 ) {
            mana += 1;
        } else if ( r == 0 & g == 44 & b == 91 ) {
            mana += 1;
        }
    }

    BenefitsAndDamages = acknows.inspect( BD );
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

    Flask = acknows.flask( FK );
    
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

    return make_tuple(
        health,
        mana,
        hungry,
        paralyze,
        poison,
        haste,
        Flask,
        alarm,
        weaponEquipped,
        amuletEquipped,
        ringEquipped,
        lowcap
    );
}

bool Stats::valuableLoot( Mat LOOTCHAT ) {

    bool valuableLoot = false;

    lootchat = acknows.inspect( LOOTCHAT );

    for ( Pixel pixel : lootchat ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
        if ( r == 0 && g == 240 && b == 0 || r == 32 && g == 160 && b == 255 || r == 255 && g == 104 && b == 255 || r == 240 && g == 240 && 0 ) {
            valuableLoot = true;
            break;
        }
    }

    return valuableLoot;
}