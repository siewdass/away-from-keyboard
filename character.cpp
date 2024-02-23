#include "character.h"
#include "utils.h"

void Character::load( const char* c, const char* w ) {

	Json config;
	config.open( c );

	Json configw;
	configw.open( w );

	vocation = config.readString( "Vocation" ).c_str( );

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
	quiver = paladin.readString( "Quiver" );
	throwing = paladin.readString( "Throwing" );

	Json h1 = config.readObject( "Healing1" );
	healing1 = h1.readString( "Hotkey" );
	healing1percent = h1.readInteger( "Percent" );

	Json h2 = config.readObject( "Healing2" );
	healing2 = h2.readString( "Hotkey" );
	healing2percent = h2.readInteger( "Percent" );

	Json h3 = config.readObject( "Healing3" );
	healing3 = h3.readString( "Hotkey" );
	healing3percent = h3.readInteger( "Percent" );

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
	
	for ( Json waypoint: configw.readArray( "Waypoints" ) ) {
		Waypoint wp;
		wp.x = waypoint.readInteger( "x" );
		wp.y = waypoint.readInteger( "y" );
		wp.z = waypoint.readInteger( "z" );
		wp.action = waypoint.readString( "action" );
		waypoints.push_back( wp );
	}

}

