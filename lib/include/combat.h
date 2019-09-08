#ifndef COMBAT_H
#define COMBAT_H

/* duration of one combat round (seconds) */
#define CB_ROUND     10

/* shortcuts for combat rounds */
#define RH  (time() + CB_ROUND/2)  /* half round */
#define R1  (time() + CB_ROUND)    /* duration of one combat round */
#define R2  (time() + CB_ROUND*2)  /* two rounds, etc. */
#define R3  (time() + CB_ROUND*3)
#define R4  (time() + CB_ROUND*4)
#define R5  (time() + CB_ROUND*5)
#define R6  (time() + CB_ROUND*6)
#define R9  (time() + CB_ROUND*9)
#define R12 (time() + CB_ROUND*12)
#define R15 (time() + CB_ROUND*17)
#define R24 (time() + CB_ROUND*24)

/* attack data, mapping indices */
#define A_SKILL      1
#define A_OBMOD      2
#define A_DMOD       3
#define A_SKIN       4
#define A_S_LEATHER  5
#define A_R_LEATHER  6
#define A_CHAIN      7
#define A_PLATE      8
#define A_CRIT       9
#define A_FRANGE    10

/* cb_random_hitloc(), array indices */
#define HITLOC_SLOT     0
#define HITLOC_DESC     1
#define HITLOC_AT       2
#define HITLOC_CHANCE   3

/* combat boni (see cb_add_boni()) */
#define CB_OB_BONUS            1 /* % offensive bonus */
#define CB_DB_BONUS            2 /* % defensive bonus */
#define CB_STUNNED             3 /* how long stunned  */
#define CB_ACTION_MOD          4 /* % skills modified */
#define CB_RESIST_MEN          5
#define CB_RESIST_ESS          6	
#define CB_RESIST_CHA          7 
#define CB_RESIST_POISON       8 
#define CB_RESIST_DISEASE      9
#define CB_RESIST_HEAT	      10
#define CB_RESIST_COLD	      11
#define CB_RESIST_IMPACT      12
#define CB_RESIST_ELECTRICITY 13
#define CB_ELEMENTAL_DEFENSE  14

/* critical types */
#define CRUSH 		1
#define SLASH 		2
#define PUNCTURE	3
#define HEAT 		4
#define COLD 		5
#define ELECTRICITY 	6
#define UNBALANCE 	7
#define IMPACT	 	8

/* compute damage */
#define DAMAGE_LIVING(att, ao, diff, attack_data, hloc) \
call_other("/base/misc/damage", "damage_living", \
	   att, ao, diff, attack_data, hloc)

/* accomplish a critical */
#define CRITICAL(type, hitloc, target, player, diff, th) \
call_other("/base/critical/crit_" + type, "critical", \
	   hitloc, target, player, diff * 2 / th - 2)

/* accomplish a fumble */
#define FUMBLE(skill, weapon, player, target) \
call_other("/base/fumble/fumble", "fumble", skill, weapon, player, target)
    
/* armor, concussion damage modifier, > 0 means less damage */
#define DMOD_SKIN        0
#define DMOD_S_LEATHER   5 
#define DMOD_R_LEATHER  10
#define DMOD_CHAIN      15
#define DMOD_PLATE      20

/*
 * attack modifers:
 * concussion damage modifier,
 * critical threshold for each armortype,
 * critical types
 */
#define ATMOD_BARE_FIST      ({ 70,70,75,85,95,100, ({ CRUSH }) })
#define ATMOD_BASH           ({ 57,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_BEAK           ({ 34,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_BITE           ({ 25,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_CLAW           ({ 47,70,75,80,80,90, ({ CRUSH,SLASH }) })
#define ATMOD_GRAPPLE        ({ 50,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_HORN           ({ 29,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_STINGER        ({ 93,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_TINY_ANIMAL    ({ 83,70,75,80,80,90, ({ }) })
#define ATMOD_TRAMPLE        ({ 31,70,75,80,80,90, ({ CRUSH,IMPACT }) })
				      	       
#define ATMOD_BROADSWORD     ({ 28,70,75,80,80,90, ({ SLASH,PUNCTURE }) })
#define ATMOD_DAGGER         ({ 47,50,65,70,80,85, ({ SLASH,PUNCTURE }) })
#define ATMOD_FALCHION       ({ 23,70,75,80,80,90, ({ SLASH }) })
#define ATMOD_HAND_AXE       ({ 31,70,75,80,80,90, ({ SLASH }) })
#define ATMOD_LONGSWORD      ({ 26,50,70,75,85,90, ({ SLASH,PUNCTURE }) })
#define ATMOD_MAIN_GAUCHE    ({ 44,70,75,80,80,90, ({ SLASH,PUNCTURE }) })
#define ATMOD_RAPIER         ({ 53,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_SCIMITAR       ({ 25,70,75,80,80,90, ({ SLASH,PUNCTURE }) })
#define ATMOD_SHORTSWORD     ({ 38,70,75,80,80,90, ({ SLASH,PUNCTURE }) })
				      	       
#define ATMOD_CLUB           ({ 39,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_MACE           ({ 35,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_MORNING_STAR   ({ 25,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_WAR_HAMMER     ({ 32,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_WHIP           ({ 39,70,75,80,80,90, ({ SLASH }) })
				      	       
#define ATMOD_COMPOSITEBOW   ({ 29,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_HEAVY_CROSSBOW ({ 23,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_LIGHT_CROSSBOW ({ 28,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_LONGBOW        ({ 26,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_SHORTBOW       ({ 31,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_SLING          ({ 25,70,75,80,80,90, ({ CRUSH }) })
				      	       
#define ATMOD_2_HAND_SWORD   ({ 16,70,75,80,80,90, ({ SLASH,CRUSH }) })
#define ATMOD_BATTLE_AXE     ({ 15,70,75,80,80,90, ({ SLASH,CRUSH }) })
#define ATMOD_FLAIL          ({ 18,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_QUARTERSTAFF   ({ 28,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_WAR_MATTOCK    ({ 17,70,75,80,80,90, ({ CRUSH }) })
				      	       
#define ATMOD_JAVELIN        ({ 33,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_LANCE          ({ 14,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_POLE_ARM       ({ 18,70,75,80,80,90, ({ SLASH }) })
#define ATMOD_SPEAR          ({ 28,70,75,80,80,90, ({ PUNCTURE,CRUSH }) })
				      	       
#define ATMOD_BOLA           ({ 51,70,75,80,80,90, ({ CRUSH }) })
#define ATMOD_DAGGER_THR     ({ 58,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_HAND_AXE_THR   ({ 55,70,75,80,80,90, ({ SLASH }) })
#define ATMOD_JAVELIN_THR    ({ 28,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_SHURIKEN_THR   ({ 60,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_SPEAR_THR      ({ 26,70,75,80,80,90, ({ PUNCTURE }) })
#define ATMOD_THROW          ({ 77,70,75,80,80,90, ({ CRUSH }) })

/* ob attack base modification */
#define OBMOD_BARE_FIST      3
#define OBMOD_BASH           1
#define OBMOD_BEAK           4
#define OBMOD_BITE           1
#define OBMOD_CLAW           4
#define OBMOD_GRAPPLE        -4
#define OBMOD_HORN           2
#define OBMOD_STINGER        0
#define OBMOD_TINY_ANIMAL    -5
#define OBMOD_TRAMPLE        5
			     
#define OBMOD_BROADSWORD     0
#define OBMOD_DAGGER         5
#define OBMOD_FALCHION       0
#define OBMOD_HAND_AXE       4
#define OBMOD_LONGSWORD      -2
#define OBMOD_MAIN_GAUCHE    4
#define OBMOD_RAPIER         8
#define OBMOD_SCIMITAR       -3
#define OBMOD_SHORTSWORD     3
			     
#define OBMOD_CLUB           -1
#define OBMOD_MACE           -3
#define OBMOD_MORNING_STAR   -3
#define OBMOD_WAR_HAMMER     -4
#define OBMOD_WHIP           -2
			     
#define OBMOD_COMPOSITEBOW   0
#define OBMOD_HEAVY_CROSSBOW 2
#define OBMOD_LIGHT_CROSSBOW 2
#define OBMOD_LONGBOW        0
#define OBMOD_SHORTBOW       0
#define OBMOD_SLING          -2

#define OBMOD_2_HAND_SWORD   -3
#define OBMOD_BATTLE_AXE     -4
#define OBMOD_FLAIL          -1
#define OBMOD_QUARTERSTAFF   0
#define OBMOD_WAR_MATTOCK    -1
			     
#define OBMOD_JAVELIN        -1
#define OBMOD_LANCE          -7
#define OBMOD_POLE_ARM       -5
#define OBMOD_SPEAR          -2
			     
#define OBMOD_BOLA           -5
#define OBMOD_DAGGER_THR     2
#define OBMOD_HAND_AXE_THR   2
#define OBMOD_JAVELIN_THR    -3
#define OBMOD_SHURIKEN_THR   0
#define OBMOD_SPEAR_THR      -4
#define OBMOD_THROW          -7

#endif
