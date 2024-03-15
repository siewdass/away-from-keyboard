#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;

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
	string quiver = "";
	string throwing = "";
	string healing1 = "";
	int healing1percent = 0;
	string healing2 = "";
	int healing2percent = 0;
	string healing3 = "";
	int healing3percent = 0;
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
	vector < Waypoint > waypoints;
	vector < Waypoint > walkable;
	void load( const char*, const char* );
};

#endif // CHARACTER_H