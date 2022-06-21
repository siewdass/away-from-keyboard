#ifndef PALADIN_H
#define PALADIN_H

void attackPaladinBox( ) {

	bool keepAttacking;
	double global = 2100;
	double attackCooldown = 0,
		attackCooldown1 = 0,
		attackCooldown2 = 0,
		attackCooldown3 = 0,
		attackCooldown4 = 0,
		attackCooldown5 = 0,
		attackCooldown6 = 0,
		attackCooldown7 = 0;

	time.sleep( 3000 );

	while ( running ) {
		
		if ( Pause ) {
			try {
				if ( mode.attacking ) {

					if ( values.mana > 10 ) {
						if ( values.box >= 3 || keepAttacking ) {
							keepAttacking = true;
							if ( time.clock( ) > attackCooldown ) {
								if ( character.annihilation != "" && time.clock( ) > attackCooldown1 && values.boxTarget && values.box == 1 ) {
									inputs.keyboard( character.annihilation );
									attackCooldown = time.clock( ) + global*2;
									attackCooldown1 = time.clock( ) + 31500;
								} else if ( character.brutalstrike != "" && time.clock( ) > attackCooldown2 && values.boxTarget && values.box == 1 ) {
									inputs.keyboard( character.brutalstrike );
									attackCooldown = time.clock( ) + global;
									attackCooldown2 = time.clock( ) + 6300;
								} else if ( character.whirlwindthrow != "" && time.clock( ) > attackCooldown3 && values.boxTarget && values.box == 1 ) {
									inputs.keyboard( character.whirlwindthrow );
									attackCooldown = time.clock( ) + global;
									attackCooldown3 = time.clock( ) + 6300;
								} else if ( character.groundshaker != "" && time.clock( ) > attackCooldown4 && values.box <= 3 && values.monsters.size( ) >= 5 ) {
									inputs.keyboard( character.groundshaker );
									attackCooldown = time.clock( ) + global;
									attackCooldown4 = time.clock( ) + 8400;
								} else if ( character.berserker != "" && time.clock( ) > attackCooldown5 && values.box >= 2 ) {
									inputs.keyboard( character.berserker );
									attackCooldown = time.clock( ) + global;
									attackCooldown5 = time.clock( ) + 4200;
								} else if ( character.fierceberserker != "" && time.clock( ) > attackCooldown6 && values.box >= 2 ) {
									inputs.keyboard( character.fierceberserker );
									attackCooldown = time.clock( ) + global;
									attackCooldown6 = time.clock( ) + 6300;
								} else if ( character.frontsweep != "" && time.clock( ) > attackCooldown7 && values.box >= 2 ) {
									string modifier = "control+";
									modifier.append( values.side );
									inputs.keywith( modifier.c_str( ) );
									time.sleep( 100 );
									inputs.keyboard( character.frontsweep );
									attackCooldown = time.clock( ) + global;
									attackCooldown7 = time.clock( ) + 6300;
								}
							}
						}
					}
					time.sleep( 100 );
				} else {
					keepAttacking = false;
					time.sleep( 1000 );
				}

			} catch ( const std::exception& e ) {
				console.log( "ERROR:", e.what( ) );
			}

		} else {
  		time.sleep( 2000 );
		}
	}
}

#endif // PALADIN_H