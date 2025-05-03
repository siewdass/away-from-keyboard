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

#endif // CHARACTER_H