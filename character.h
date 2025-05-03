#ifndef CHARACTER_H
#define CHARACTER_H

#include "util.h"
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
	vector < Waypoint > waypoints;
	vector < Waypoint > walkable;
	string ammunition = "";
	Character(string, string);
};

#endif // CHARACTER_H