/*
 * <learning first step> + ( <learning next steps> << 10 )
 */

#define SK_BASE_COSTS1 \
([ \
 /* athletic */\
 SK_ACROBATICS			: 	(20 + (0	<< 10) ),\
 SK_BODY_DEVELOP		: 	(40 + (0	<< 10) ),\
 SK_CLIMB			: 	(30 + (0	<< 10) ),\
 SK_CONTORTIONS		        : 	(30 + (0	<< 10) ),\
 SK_DISTANCE_RUN		: 	(25 + (0	<< 10) ),\
 SK_JUMPING			: 	(25 + (0	<< 10) ),\
 SK_SPRINTING			: 	(25 + (0	<< 10) ),\
 SK_SWIM			: 	(25 + (0	<< 10) ),\
 /* academic */\
 SK_DEMON_LORE			: 	(20 + (0	<< 10) ),\
 SK_FAUNA_LORE			: 	(15 + (0	<< 10) ),\
 SK_FLORA_LORE			: 	(15 + (0	<< 10) ),\
 SK_HERB_LORE			: 	(15 + (0	<< 10) ),\
 SK_LOCK_LORE			: 	(20 + (0	<< 10) ),\
 SK_MAPPING			: 	(35 + (0	<< 10) ),\
 SK_MATHEMATICS		        : 	(20 + (0	<< 10) ),\
 SK_NAVIGATION			: 	(20 + (0	<< 10) ),\
 SK_POISON_LORE		        : 	(20 + (0	<< 10) ),\
 SK_WARDING_LORE		: 	(30 + (0	<< 10) ),\
 SK_WEATHER_WATCH		: 	(20 + (0	<< 10) ),\
 SK_STONE_LORE     		: 	(15 + (0	<< 10) ),\
 /* combat */\
 SK_ADRENAL_DEFENSE		: 	(170 + (0	<< 10) ),\
 SK_DISARM_ARMED		: 	(45 + (0	<< 10) ),\
 SK_DISARM_UNARMED		: 	(50 + (0	<< 10) ),\
 SK_FRENZY			: 	(30 + (0	<< 10) ),\
 SK_MA_STRIKE 			: 	(40 + (0	<< 10) ),\
 SK_MA_SWEEPS	 		: 	(40 + (0	<< 10) ),\
 SK_PLATE			: 	(55 + (0	<< 10) ),\
 SK_CHAIN			: 	(45 + (0	<< 10) ),\
 SK_RIGID_LEATHER		: 	(35 + (0	<< 10) ),\
 SK_SOFT_LEATHER		: 	(30 + (0	<< 10) ),\
 SK_SUBDUING			: 	(35 + (0	<< 10) ),\
 SK_TWO_WEAPONS		        : 	(100 + (0	<< 10) ),\
 /* craft */\
 SK_LEATHER_WORK		: 	(25 + (0	<< 10) ),\
 SK_MINING			: 	(25 + (0	<< 10) ),\
 SK_SAILING			: 	(30 + (0	<< 10) ),\
 SK_WOOD_CRAFT 		        : 	(25 + (0	<< 10) ),\
 SK_METAL_CRAFT 	        : 	(30 + (0	<< 10) ),\
 SK_FLETCHING			: 	(20 + (0	<< 10) ),\
 /* general */\
 SK_APPRAISAL			: 	(20 + (0	<< 10) ),\
 SK_COOKERY			: 	(20 + (0	<< 10) ),\
 SK_EVAL_ARMOR			: 	(25 + (0	<< 10) ),\
 SK_EVAL_WEAPON		        : 	(25 + (0	<< 10) ),\
 SK_READ_TRACKS		        : 	(20 + (0	<< 10) ),\
 SK_RIDE			: 	(25 + (0	<< 10) ),\
 SK_ROPE_MASTERY		: 	(20 + (0	<< 10) ),\
 SK_TRADING			: 	(25 + (0	<< 10) ),\
 /* linguistics */\
 SK_ADUNAIC			: 	(20 + (0	<< 10) ),\
 SK_BLACK_SPEECH		: 	(20 + (0	<< 10) ),\
 SK_KHUZDUL			: 	(20 + (0	<< 10) ),\
 SK_QUENYA			: 	(20 + (0	<< 10) ),\
 SK_ROHERIC			: 	(20 + (0	<< 10) ),\
 SK_SINDARIN			: 	(20 + (0	<< 10) ),\
 SK_TALE_TELLING		: 	(20 + (0	<< 10) ),\
 SK_WESTRON			: 	(20 + (0	<< 10) ),\
 /* mental */\
 SK_CONTROL_LYC		        : 	(15 + (0	<< 10) ),\
 SK_MEDITATE_CLEANSING		: 	(15 + (0	<< 10) ),\
 SK_MEDITATE_HEALING		: 	(15 + (0	<< 10) ),\
 SK_MEDITATE_SLEEP		: 	(15 + (0	<< 10) ),\
 SK_PERCEPTION			: 	(20 + (0	<< 10) ),\
 SK_SLA		                : 	(20 + (0	<< 10) ),\
 /* medical */\
 SK_ANIMAL_HEAL		        : 	(15 + (0	<< 10) ),\
 SK_DIAGNOSTICS		        : 	(20 + (0	<< 10) ),\
 SK_FIRST_AID			: 	(15 + (0	<< 10) ),\
 SK_SECOND_AID			: 	(25 + (0	<< 10) ),\
 /* music */\
 SK_DANCE			: 	(20 + (0	<< 10) ),\
 SK_PLAY_FLUTE			: 	(20 + (0	<< 10) ),\
 SK_PLAY_TRUMPET                : 	(20 + (0	<< 10) ),\
 SK_PLAY_GUITAR                 : 	(20 + (0	<< 10) ),\
 SK_PLAY_DRUM                   : 	(20 + (0	<< 10) ),\
 SK_PLAY_VIOLIN                 : 	(20 + (0	<< 10) ),\
 SK_PLAY_HARP                   : 	(20 + (0	<< 10) ),\
 SK_PLAY_LYRE                   : 	(20 + (0	<< 10) ),\
 SK_PLAY_HORN                   : 	(20 + (0	<< 10) ),\
 SK_SINGING			: 	(20 + (0	<< 10) ),\
    ])					 
					 
#define SK_BASE_COSTS2 \
([ \
 /* magic */\
 SK_CHANNELING			: 	(60 + (0	<< 10) ),\
 SK_POWER_PROJ			: 	(35 + (0	<< 10) ),\
 SK_RUNES			: 	(35 + (0	<< 10) ),\
 SK_SENSE_MAGIC		        : 	(20 + (0	<< 10) ),\
 SK_STAVES_WANDS		: 	(35 + (0	<< 10) ),\
 SK_TRANS_ARMOR		        : 	(65 + (0	<< 10) ),\
 SK_MATTACK_COLD_BALL		: 	(0  + (0	<< 10) ),\
 SK_MATTACK_FIRE_BALL		: 	(0  + (0	<< 10) ),\
 SK_MATTACK_FIRE_BOLT		: 	(50 + (0	<< 10) ),\
 SK_MATTACK_ICE_BOLT		: 	(50 + (0	<< 10) ),\
 SK_MATTACK_LIGHT_BOLT	        : 	(50 + (0	<< 10) ),\
 SK_MATTACK_SHOCK_BOLT		: 	(50 + (0	<< 10) ),\
 SK_MATTACK_STONE_THROW	        :  	(50 + (0	<< 10) ),\
 SK_MATTACK_WATER_BOLT		: 	(50 + (0	<< 10) ),\
 SK_ML_AIONION			: 	(30 + (0	<< 10) ),\
 SK_ML_AISTHASIS		: 	(30 + (0	<< 10) ),\
 SK_ML_ANATHEMA		        : 	(30 + (0	<< 10) ),\
 SK_ML_ASEBEIA			: 	(30 + (0	<< 10) ),\
 SK_ML_ASTER			: 	(30 + (0	<< 10) ),\
 SK_ML_CHAOS			: 	(30 + (0	<< 10) ),\
 SK_ML_CONSCIOUSNESS		: 	(30 + (0	<< 10) ),\
 SK_ML_EARTHBLOOD		: 	(30 + (0	<< 10) ),\
 SK_ML_EIRENAY			: 	(30 + (0	<< 10) ),\
 SK_ML_GNOSIS			: 	(30 + (0	<< 10) ),\
 SK_ML_GRAPHAY			: 	(30 + (0	<< 10) ),\
 SK_ML_HUDATOS			: 	(30 + (0	<< 10) ),\
 SK_ML_IHYAMA			: 	(30 + (0	<< 10) ),\
 SK_ML_IRUARIC			: 	(30 + (0	<< 10) ),\
 SK_ML_KUBEIA			: 	(30 + (0	<< 10) ),\
 SK_ML_LOGOS			: 	(30 + (0	<< 10) ),\
 SK_ML_PARAPATEO		: 	(30 + (0	<< 10) ),\
 SK_ML_PETRA			: 	(30 + (0	<< 10) ),\
 SK_ML_PNEUMATIKOS		: 	(30 + (0	<< 10) ),\
 SK_ML_PORNEIA			: 	(30 + (0	<< 10) ),\
 SK_ML_PUROSIS			: 	(30 + (0	<< 10) ),\
 SK_ML_QADOSH			: 	(30 + (0	<< 10) ),\
 SK_ML_SKOTIA			: 	(30 + (0	<< 10) ),\
 SK_ML_SOMA			: 	(30 + (0	<< 10) ),\
 SK_ML_SPOUDAZO		        :  	(30 + (0	<< 10) ),\
 SK_ML_THANATOS		        : 	(30 + (0	<< 10) ),\
 SK_ML_TOTEM			: 	(30 + (0	<< 10) ),\
 SK_ML_XZULOU			: 	(30 + (0	<< 10) ),\
 /* subterfuge */\
 SK_AMBUSH			: 	(60 + (0	<< 10) ),\
 SK_DISARM_TRAP		        : 	(50 + (0	<< 10) ),\
 SK_HIDE			: 	(30 + (0	<< 10) ),\
 SK_HIDE_ITEM			: 	(25 + (0	<< 10) ),\
 SK_PICK_LOCKS			: 	(40 + (0	<< 10) ),\
 SK_PICK_POCKET		        : 	(30 + (0	<< 10) ),\
 SK_SET_TRAPS			: 	(30 + (0	<< 10) ),\
 SK_SILENT_KILL		        : 	(35 + (0	<< 10) ),\
 SK_STALK			: 	(30 + (0	<< 10) ),\
    ])
    
#define SK_BASE_COSTS (SK_BASE_COSTS1 + SK_BASE_COSTS2)
