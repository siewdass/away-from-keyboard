#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <thread>
#include <jsoncpp/json/json.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

#pragma once
#include <atomic>
#include <memory>
#include <random>
#include <cmath>
#include <queue>

using namespace cv;
using namespace std::chrono;
using namespace std;

struct PathNode {
    Point pos;
    Point parent;
    int g;  // Cost from start

    PathNode(Point p, Point par, int cost) 
        : pos(p), parent(par), g(cost) {}
};

struct Waypoint {
	int x, y, z;
	string action = "attack"; // walk, rope, stairs
	int direction = 1;
};

struct Position {
	int x, y, z;
	string img;
};

struct Pixel {
	int r, g, b;
};

class Player {
public:
	Player( vector < Waypoint > waypoints ) {
		x = waypoints.size() > 0 ? waypoints[0].x : 0;
		y = waypoints.size() > 0 ? waypoints[0].y : 0;
	}
	int validSqm;
	int health = 267, mana = 267;
	bool hungry, paralyze, poison, haste;
	bool isTargeting, isAttacking, isWalking, isLooting, visibleMonsters;
	int x = 0, y = 0, z = 0;
	string minimap;
	int box;
	vector < Point > monsters;
	string side;
	vector < Point > flasks;
	Point targetPosition;
	bool target;
	bool alarm;
	bool weapon, necklace, ring;
	bool lowcapacity = false;
	bool canWalk;
	bool nearPlayer;
};

class Mode {
public:
	bool walking = true, attacking = false, looting = false;
};

namespace Jsoncpp = Json;

#define Json _Json
#define Array std::vector

class {
	public:
		void sleep( int ms ) {
            std::this_thread::sleep_for( milliseconds( ms ) );
        };
		double clock( ) {
            return duration_cast < milliseconds > ( 
                time_point_cast < milliseconds > ( 
                    high_resolution_clock::now( ) 
                ).time_since_epoch( ) 
            ).count( );
        }
} timer;

class {
public:
	template < typename ...Args >
	void log( Args... args ) {
		( ( cout << args << " " ), ... );
		cout << endl;
	}
} console;

class Json {
	public:

	Jsoncpp::Reader reader; 
	Jsoncpp::Value data; 
	const char* last;

	std::string str;
	string type;
    
	void open( const char* f ) {
		std::ifstream file ( f );
		if ( file.is_open( ) ) {
			string txt;
			while ( getline ( file, txt ) ) {
				str += txt;
			}
		}
		file.close( );
		reader.parse( str, data );
	}
		
	void save( const char* f ) {
		Jsoncpp::StyledWriter writer;
		string to = writer.write( data );
		ofstream save;
		save.open( f );
		save << to;
		save.close( );
	}

	int readInteger( ) {
		return data.asInt( );
	}
   
	bool readBoolean( ) {
		return data.asBool( );
	}
     
	string readString( ) {
		return data.asString( );
	}

	int readInteger( const char* key ) {
		return data[ key ].asInt( );
	}

	bool readBoolean( const char* key ) {
		return data[ key ].asBool( );
	}
        
	string readString( const char* key ) {
		return data[ key ].asString( );
	}
    
	Json readObject( const char* key ) {
		Json tmp;
		tmp.data = data[ key ];
		return tmp;
	}

	vector < Json > readArray( const char* key ) {
		vector < Json > vtmp;
    	
		Json tmp;
		tmp.data = data[ key ];
    	
		for ( int i = 0; i < tmp.data.size( ); i++ ) { 
			Json t;
			t.data = tmp.data[ i ];
			vtmp.push_back( t );
		}
			
		return vtmp;
	}

	vector < Json > readArray( ) {
		vector < Json > vtmp;

		for ( int i = 0; i < data.size( ); i++ ) {
			Json tmp;
			tmp.data = data[ i ];
			vtmp.push_back( tmp );
		}
			
		return vtmp;
	}

	void writeInteger( const char* key, int value ) {
		last = key;
		type = "integer";
		data[ key ] = value;
	}
    
	void writeBoolean( const char* key, bool value ) {
		last = key;
		type = "boolean";
		data[ key ] = value;
	}
		
	void writeString( const char* key, const char* value ) {
		last = key;
		type = "string";
		data[ key ] = value;
	}
		
	void writeObject( const char* key, Json value ) {
		Jsoncpp::Value item; 
		if ( value.type == "integer" ) {	
			int v = value.readInteger( value.last );
			item[ value.last ] = v;
			data[ key ] = item;
		} else if ( value.type == "boolean" ) {
			bool v = value.readBoolean( value.last );
			item[ value.last ] = v;
			data[ key ] = item;
		} else if ( value.type == "string" ) {
			string v = value.readString( value.last );
			item[ value.last ] = v;
			data[ key ] = item;
		}
	}
		
	void writeArray( const char* key, Json value ) {
		Jsoncpp::Value item; 
		if ( value.type == "integer" ) {
			int v = value.readInteger( value.last );
			item[ value.last ] = v;
			data[ key ].append( item );
		} else if ( value.type == "boolean" ) {
			bool v = value.readBoolean( value.last );
			item[ value.last ] = v;
			data[ key ].append( item );
		} else if ( value.type == "string" ) {
			string v = value.readString( value.last );
			item[ value.last ] = v;
			data[ key ].append( item );
		}
	}

	void writeArray( const char* key, int value ) {
		data[ key ].append( value );
	}

	void writeArray( const char* key, bool value ) {
		data[ key ].append( value );
	}
	
	void writeArray( const char* key, const char* value ) {
		string tmp = value;
		data[ key ].append( tmp );
	}
};

class {
public:
	int plusMinus() {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(-1, 1);
		return dis(gen);
	}
} other;


class Character {
public:
    string vocation = "";
    string bloodrage = "";
    string challenge = "";
    string chivalrouschallenge = "";
    string berserker = "";
    string fierceberserker = "";
    string frontsweep = "";
    string groundshaker = "";
    string annihilation = "";
    string brutalstrike = "";
    string whirlwindthrow = "";
    string sharpshooter = "";
    string divinedazzle = "";
    string divinecaldera = "";
    string arearune = "";
    string strongetherealspear = "";
    string etherealspear = "";
    string divinemissile = "";
    string curecurse = "";
    string quiver1 = "";
    string quiver2 = "";
    string throwing = "";
    string healing = "";
    int healingpercent = 0;
    string healthpotion = "";
    int healthpotionpercent = 0;
    string manapotion = "";
    int manapotionpercent = 0;
    string recovery = "";
    string haste = "";
    string curepoison = "";
    string food = "";
    string paralyze = "";
    string summon = "";
    string skillpotion = "";
    string necklace = "";
    string ring = "";
    string shovel = "";
    string pick = "";
    string rope = "";
    string hunt = "";
    string leave = "";
    vector<Waypoint> waypoints;
    vector<Waypoint> walkable;
    string ammunition = "";

    Character(string c, string w) {
        Json config;
        config.open(c.c_str());

        vocation = config.readString("Vocation");

        Json knight = config.readObject("Knight");
        bloodrage = knight.readString("BloodRage");
        challenge = knight.readString("Challenge");
        chivalrouschallenge = knight.readString("ChivalrousChallenge");
        berserker = knight.readString("Berserker");
        fierceberserker = knight.readString("FierceBerserker");
        frontsweep = knight.readString("FrontSweep");
        groundshaker = knight.readString("Groundshaker");
        annihilation = knight.readString("Annihilation");
        brutalstrike = knight.readString("BrutalStrike");
        whirlwindthrow = knight.readString("WhirlwindThrow");

        Json paladin = config.readObject("Paladin");
        sharpshooter = paladin.readString("Sharpshooter");
        divinedazzle = paladin.readString("DivineDazzle");
        divinecaldera = paladin.readString("DivineCaldera");
        arearune = paladin.readString("AreaRune");
        strongetherealspear = paladin.readString("StrongEtherealSpear");
        etherealspear = paladin.readString("EtherealSpear");
        divinemissile = paladin.readString("DivineMissile");
        curecurse = paladin.readString("CureCurse");
        throwing = paladin.readString("Throwing");
        ammunition = paladin.readString("Ammunition");
        
        Json qv = paladin.readObject("Quiver");
        quiver1 = qv.readString("First");
        quiver2 = qv.readString("Second");

        Json h = config.readObject("Healing");
        healing = h.readString("Hotkey");
        healingpercent = h.readInteger("Percent");

        Json hp = config.readObject("HealthPotion");
        healthpotion = hp.readString("Hotkey");
        healthpotionpercent = hp.readInteger("Percent");

        Json mp = config.readObject("ManaPotion");
        manapotion = mp.readString("Hotkey");
        manapotionpercent = mp.readInteger("Percent");

        recovery = config.readString("Recovery");
        haste = config.readString("Haste");
        curepoison = config.readString("CurePoison");
        food = config.readString("Food");
        paralyze = config.readString("Paralyze");
        summon = config.readString("Summon");
        skillpotion = config.readString("SkillPotion");
        necklace = config.readString("Necklace");
        ring = config.readString("Ring");
        shovel = config.readString("Shovel");
        pick = config.readString("Pick");
        rope = config.readString("Rope");
        hunt = config.readString("Hunt");
        leave = config.readString("Leave");

        waypoints.clear();

        Json configw;
        configw.open(w.c_str());

        for (Json waypoint : configw.readArray("Waypoints")) {
            Waypoint wp;
            wp.x = waypoint.readInteger("x");
            wp.y = waypoint.readInteger("y");
            wp.z = waypoint.readInteger("z");
            wp.action = waypoint.readString("action");
            waypoints.push_back(wp);
        }
    }
};

#endif
