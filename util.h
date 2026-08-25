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
#include <map>
#include <unordered_map>
#include <filesystem>

#pragma once
#include <atomic>
#include <memory>
#include <random>
#include <cmath>
#include <queue>
#include <algorithm>

using namespace cv;
using namespace std::chrono;
using namespace std;

namespace Jsoncpp = Json;

#define Json _Json
#define Array std::vector

struct PathNode {
    Point pos;
    Point parent;
    int g;  // Cost from start

    PathNode(Point p, Point par, int cost) 
        : pos(p), parent(par), g(cost) {}
};

struct Waypoint {
	int x, y, z;
	string action; // walk, rope, stairs
	string direction;
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
	bool walking = true, attacking = false, looting = false;
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
	bool weapon, necklace, ring, offhand;
	bool lowcapacity = false;
	bool canWalk;
	bool nearPlayer;
	int position = 0;
};

class Control {
public:
    std::vector<std::shared_ptr<std::atomic<bool>>> runnings;
    std::vector<std::shared_ptr<std::atomic<bool>>> pauses;
    std::vector<std::shared_ptr<std::atomic<int>>> generations;
    
    Control() {
        for (int i = 0; i < 6; ++i) {
            runnings.push_back(std::make_shared<std::atomic<bool>>(false));
            pauses.push_back(std::make_shared<std::atomic<bool>>(false));
            generations.push_back(std::make_shared<std::atomic<int>>(0));
        }
    }
	
	bool getPause( int client ) {
		return *pauses[ client ];
	}
	void setPause( int client, bool p ) {
		*pauses[ client ] = p;
	}
	bool getRunning( int client ) {
		return *runnings[ client ];
	}
	void setRunning( int client, bool r ) {
		*runnings[ client ] = r;
	}
	int nextGeneration( int client ) {
		return (*generations[ client ])++;
	}
	int getGeneration( int client ) {
		return *generations[ client ];
	}
};

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
	vector<Point> reordened(vector<Point> myv) {
		vector<Point> newone = myv;
		unsigned seed = system_clock::now().time_since_epoch().count();
		shuffle(newone.begin(), newone.end(), mt19937(seed));
		return newone;
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
    vector<Waypoint> walkables;

    string quiver1 = "";
    string quiver2 = "";
    string ammunition1 = "";
	string ammunition2 = "";

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
        
        Json ga = paladin.readObject("GroupArrow");
        quiver1 = ga.readString("Quiver");
        ammunition1 = ga.readString("Ammunition");

        Json ta = paladin.readObject("TargetArrow");
        quiver2 = ta.readString("Quiver");
        ammunition2 = ta.readString("Ammunition");

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
		walkables.clear();

        Json configw;
        configw.open(w.c_str());

        for (Json waypoint : configw.readArray("Waypoints")) {
            Waypoint wp;
            wp.x = waypoint.readInteger("x");
            wp.y = waypoint.readInteger("y");
            wp.z = waypoint.readInteger("z");
            wp.action = waypoint.readString("action");
			wp.direction = waypoint.readString("direction");
            waypoints.push_back(wp);
        }

		for (Json walkable : configw.readArray("Walkable")) {
            Waypoint wp;
            wp.x = walkable.readInteger("x");
            wp.y = walkable.readInteger("y");
            wp.z = walkable.readInteger("z");
			walkables.push_back(wp);
        }
    }
};

class {
public:
	string process( string cmd ) {
		const int MAX_BUFFER = 2048;
		char buffer[ MAX_BUFFER ];
		FILE *stream = popen( cmd.c_str( ) , "r" );
		if ( stream ) {
			while ( !feof( stream ) ) {
				if ( fgets( buffer, MAX_BUFFER, stream ) != NULL) { }
			}
			pclose( stream );
		}
		string v{ buffer };
		v.pop_back( );
		return v;
	}
} client;

struct Hotkey {
    int priority;
    string key;
	string combination;

	Hotkey(int p, string k, string c = String()) : priority(p), key(k), combination(c) {}

    bool operator<(const Hotkey& other) const {
        return priority < other.priority;
    }
};

class Actions {
public:
    std::priority_queue<Hotkey> queue;
    std::mutex mtx;

    void add(const Hotkey& hotkey) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(hotkey);
    }

    Hotkey retrieve() {
        std::lock_guard<std::mutex> lock(mtx);
        Hotkey next = queue.top();
        queue.pop();
        return next;
    }

    bool isEmpty() {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};

#endif