#include "character.h"
#include "new/util.h"

Character::Character( string c, string w ) {

	Json config;
	config.open( c.c_str( ) );

	vocation = config.readString( "Vocation" );

	Json knight = config.readObject( "Knight" );
	bloodrage = knight.readString( "BloodRage" );
	challenge = knight.readString( "Challenge" );
	chivalrouschallenge = knight.readString( "ChivalrousChallenge" );
	berserker = knight.readString( "Berserker" );
	fierceberserker = knight.readString( "FierceBerserker" );
	frontsweep = knight.readString( "FrontSweep" );
	groundshaker = knight.readString( "Groundshaker" );
	annihilation = knight.readString( "Annihilation" );
	brutalstrike = knight.readString( "BrutalStrike" );
	whirlwindthrow = knight.readString( "WhirlwindThrow" );

	Json paladin = config.readObject( "Paladin" );
	sharpshooter = paladin.readString( "Sharpshooter" );
	divinedazzle = paladin.readString( "DivineDazzle" );
	divinecaldera = paladin.readString( "DivineCaldera" );
	arearune = paladin.readString( "AreaRune" );
	strongetherealspear = paladin.readString( "StrongEtherealSpear" );
	etherealspear = paladin.readString( "EtherealSpear" );
	divinemissile = paladin.readString( "DivineMissile" );
	curecurse = paladin.readString( "CureCurse" );
	throwing = paladin.readString( "Throwing" );
	ammunition = paladin.readString( "Ammunition" );
	
	Json qv =  paladin.readObject( "Quiver" );
	quiver1 = qv.readString( "First" );
	quiver2 = qv.readString( "Second" );

	Json h = config.readObject( "Healing" );
	healing = h.readString( "Hotkey" );
	healingpercent = h.readInteger( "Percent" );

	Json hp = config.readObject( "HealthPotion" );
	healthpotion = hp.readString( "Hotkey" );
	healthpotionpercent = hp.readInteger( "Percent" );

	Json mp = config.readObject( "ManaPotion" );
	manapotion = mp.readString( "Hotkey" );
	manapotionpercent = mp.readInteger( "Percent" );

	recovery = config.readString( "Recovery" );
	haste = config.readString( "Haste" );
	curepoison = config.readString( "CurePoison" );
	food = config.readString( "Food" );
	paralyze = config.readString( "Paralyze" );
	summon = config.readString( "Summon" );
	skillpotion = config.readString( "SkillPotion" );
	necklace = config.readString( "Necklace" );
	ring = config.readString( "Ring" );
	shovel = config.readString( "Shovel" );
	pick = config.readString( "Pick" );
	rope = config.readString( "Rope" );
	hunt = config.readString( "Hunt" );
	leave = config.readString( "Leave" );

	waypoints.clear( );

	Json configw;
	configw.open( w.c_str( ) );

	for ( Json waypoint: configw.readArray( "Waypoints" ) ) {
		Waypoint wp;
		wp.x = waypoint.readInteger( "x" );
		wp.y = waypoint.readInteger( "y" );
		wp.z = waypoint.readInteger( "z" );
		wp.action = waypoint.readString( "action" );
		waypoints.push_back( wp );
	}

}

