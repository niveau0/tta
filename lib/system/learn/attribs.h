/*
 * skill related attributes
 */

#ifndef ATTRIBS
#define ATTRIBS

#define SKILL_ATTRIBUTES1 \
([ \
 /* academic */\
  SK_DEMON_LORE			: ({ STAT_ME, STAT_RE }),\
  SK_FAUNA_LORE			: ({ STAT_ME, STAT_RE }),\
  SK_FLORA_LORE			: ({ STAT_ME, STAT_RE }),\
  SK_HERB_LORE			: ({ STAT_ME, STAT_EM }),\
  SK_LOCK_LORE			: ({ STAT_ME, STAT_RE }),\
  SK_MAPPING			: ({ STAT_ME, STAT_RE }),\
  SK_MATHEMATICS		: ({ STAT_RE, STAT_ME }),\
  SK_NAVIGATION			: ({ STAT_RE, STAT_IN }),\
  SK_POISON_LORE		: ({ STAT_ME, STAT_RE }),\
  SK_WARDING_LORE		: ({ STAT_ME, STAT_RE }),\
  SK_WEATHER_WATCH		: ({ STAT_IN, STAT_EM }),\
  SK_STONE_LORE                 : ({ STAT_ME, STAT_IN }),\
 /* athletic */\
  SK_ACROBATICS			: ({ STAT_AG, STAT_QU }),\
  SK_BODY_DEVELOP		: ({ STAT_CO }),\
  SK_CLIMB			: ({ STAT_AG }),\
  SK_CONTORTIONS		: ({ STAT_AG, STAT_SD }),\
  SK_DISTANCE_RUN		: ({ STAT_CO }),\
  SK_JUMPING			: ({ STAT_ST, STAT_AG }),\
  SK_RIDE			: ({ STAT_AG, STAT_EM }),\
  SK_SPRINTING			: ({ STAT_QU, STAT_QU, STAT_CO }),\
  SK_SWIM			: ({ STAT_AG }),\
 /* combat */\
  SK_ADRENAL_DEFENSE		: ({ }),\
  SK_CHAIN			: ({ STAT_AG, STAT_ST }),\
  SK_DISARM_ARMED		: ({ STAT_AG }),\
  SK_DISARM_UNARMED		: ({ STAT_AG }),\
  SK_FRENZY			: ({ STAT_SD }),\
  SK_MA_STRIKE			: ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_MA_SWEEPS			: ({ STAT_ST, STAT_AG, STAT_AG }),\
  SK_PLATE			: ({ STAT_AG, STAT_ST }),\
  SK_RIGID_LEATHER		: ({ STAT_AG, STAT_ST }),\
  SK_SOFT_LEATHER		: ({ STAT_AG, STAT_ST }),\
  SK_SUBDUING			: ({ STAT_AG, STAT_QU }),\
  SK_TWO_WEAPONS		: ({ STAT_ST }),\
  SK_ATTACK_BASH                : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_BEAK                : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_BITE                : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_CLAW                : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_GRAPPLE             : ({ STAT_AG, STAT_AG, STAT_ST }),\
  SK_ATTACK_HORN                : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_STINGER             : ({ STAT_AG, STAT_AG, STAT_ST }),\
  SK_ATTACK_TINY_ANIMAL         : ({ }),\
  SK_ATTACK_TRAMPLE             : ({ STAT_ST, STAT_CO }),\
  \
  SK_ATTACK_BROADSWORD          : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_DAGGER              : ({ STAT_AG, STAT_AG, STAT_ST }),\
  SK_ATTACK_FALCHION            : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_HAND_AXE            : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_LONGSWORD           : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_MAIN_GAUCHE         : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_RAPIER              : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_SCIMITAR            : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_SHORTSWORD          : ({ STAT_ST, STAT_ST, STAT_AG }),\
  \
  SK_ATTACK_BARE_FIST           : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_CLUB                : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_MACE                : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_MORNING_STAR        : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_WAR_HAMMER          : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_WHIP                : ({ STAT_ST, STAT_AG }),\
    ])
    
#define SKILL_ATTRIBUTES2 \
([ \
  SK_ATTACK_COMPOSITEBOW        : ({ STAT_AG, STAT_AG, STAT_ST }),\
  SK_ATTACK_HEAVY_CROSSBOW      : ({ STAT_AG, STAT_AG, STAT_ST }),\
  SK_ATTACK_LIGHT_CROSSBOW      : ({ STAT_AG, STAT_AG, STAT_ST }),\
  SK_ATTACK_LONGBOW             : ({ STAT_AG, STAT_AG, STAT_ST }),\
  SK_ATTACK_SHORTBOW            : ({ STAT_AG, STAT_AG, STAT_ST }),\
  SK_ATTACK_SLING               : ({ STAT_AG, STAT_AG, STAT_ST }),\
  \
  SK_ATTACK_2_HAND_SWORD        : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_BATTLE_AXE          : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_FLAIL               : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_QUARTERSTAFF        : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_WAR_MATTOCK         : ({ STAT_ST, STAT_ST, STAT_AG }),\
  \
  SK_ATTACK_JAVELIN             : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_LANCE               : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_POLE_ARM            : ({ STAT_ST, STAT_ST, STAT_AG }),\
  SK_ATTACK_SPEAR               : ({ STAT_ST, STAT_ST, STAT_AG }),\
  \
  SK_ATTACK_BOLA                : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_DAGGER_THR          : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_HAND_AXE_THR        : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_JAVELIN_THR         : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_SHURIKEN            : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_SPEAR_THR           : ({ STAT_ST, STAT_AG }),\
  SK_ATTACK_THROW               : ({ STAT_ST, STAT_AG }),\
 /* craft */\
  SK_LEATHER_WORK		: ({ STAT_AG, STAT_RE }),\
  SK_MINING			: ({ STAT_RE, STAT_IN }),\
  SK_SAILING			: ({ STAT_AG, STAT_IN }),\
  SK_WOOD_CRAFT			: ({ STAT_AG, STAT_IN }),\
  SK_METAL_CRAFT		: ({ STAT_AG, STAT_IN, STAT_ST }),\
 /* general */\
  SK_APPRAISAL			: ({ STAT_RE, STAT_ME }),\
  SK_COOKERY			: ({ STAT_RE, STAT_AG }),\
  SK_EVAL_ARMOR			: ({ STAT_IN, STAT_RE }),\
  SK_EVAL_WEAPON		: ({ STAT_IN, STAT_RE }),\
  SK_FLETCHING			: ({ STAT_AG, STAT_SD }),\
  SK_READ_TRACKS		: ({ STAT_IN, STAT_RE }),\
  SK_ROPE_MASTERY		: ({ STAT_AG, STAT_ME }),\
  SK_TRADING			: ({ STAT_RE, STAT_EM }),\
 /* linguistics */\
  SK_ADUNAIC                    : ({ }),\
  SK_BLACK_SPEECH               : ({ }),\
  SK_KHUZDUL                    : ({ }),\
  SK_QUENYA                     : ({ }),\
  SK_ROHERIC                    : ({ }),\
  SK_SINDARIN                   : ({ }),\
  SK_TALE_TELLING		: ({ STAT_PR, STAT_ME }),\
  SK_WESTRON                    : ({ }),\
 /* magic */\
  SK_CHANNELING			: ({ STAT_IN }),\
  SK_POWER_PROJ			: "query_realm_stats",\
  SK_RUNES			: ({ STAT_IN, STAT_EM }),\
  SK_SENSE_MAGIC		: ({ STAT_IN, STAT_EM, STAT_PR }),\
  SK_STAVES_WANDS		: ({ STAT_IN, STAT_EM }),\
  SK_TRANS_ARMOR                : "query_realm_stats",\
    ])
    
#define SKILL_ATTRIBUTES3 \
([ \
  SK_ML_AIONION			: "query_realm_stats",\
  SK_ML_AISTHASIS		: "query_realm_stats",\
  SK_ML_ANATHEMA		: "query_realm_stats",\
  SK_ML_ASEBEIA			: "query_realm_stats",\
  SK_ML_ASTER			: "query_realm_stats",\
  SK_ML_CHAOS			: "query_realm_stats",\
  SK_ML_CONSCIOUSNESS		: "query_realm_stats",\
  SK_ML_EARTHBLOOD		: "query_realm_stats",\
  SK_ML_EIRENAY			: "query_realm_stats",\
  SK_ML_GNOSIS			: "query_realm_stats",\
  SK_ML_GRAPHAY			: "query_realm_stats",\
  SK_ML_HUDATOS			: "query_realm_stats",\
  SK_ML_IHYAMA			: "query_realm_stats",\
  SK_ML_IRUARIC			: "query_realm_stats",\
  SK_ML_KUBEIA			: "query_realm_stats",\
  SK_ML_LOGOS			: "query_realm_stats",\
  SK_ML_PARAPATEO		: "query_realm_stats",\
  SK_ML_PETRA			: "query_realm_stats",\
  SK_ML_PNEUMATIKOS		: "query_realm_stats",\
  SK_ML_PORNEIA			: "query_realm_stats",\
  SK_ML_PUROSIS			: "query_realm_stats",\
  SK_ML_QADOSH			: "query_realm_stats",\
  SK_ML_SKOTIA			: "query_realm_stats",\
  SK_ML_SOMA			: "query_realm_stats",\
  SK_ML_SPOUDAZO		: "query_realm_stats",\
  SK_ML_THANATOS		: "query_realm_stats",\
  SK_ML_TOTEM			: "query_realm_stats",\
  SK_ML_XZULOU			: "query_realm_stats",\
  SK_MATTACK_FIRE_BOLT		: ({ STAT_AG }),\
  SK_MATTACK_ICE_BOLT		: ({ STAT_AG }),\
  SK_MATTACK_LIGHT_BOLT	        : ({ STAT_AG }),\
  SK_MATTACK_SHOCK_BOLT		: ({ STAT_AG }),\
  SK_MATTACK_STONE_THROW	: ({ STAT_AG }),\
  SK_MATTACK_WATER_BOLT		: ({ STAT_AG }),\
  SK_MATTACK_COLD_BALL		: ({ }),\
  SK_MATTACK_FIRE_BALL		: ({ }),\
 /* medical */\
  SK_ANIMAL_HEAL		: ({ STAT_EM, STAT_RE }),\
  SK_DIAGNOSTICS		: ({ STAT_IN, STAT_RE }),\
  SK_FIRST_AID			: ({ STAT_SD, STAT_EM }),\
  SK_SECOND_AID			: ({ STAT_SD, STAT_EM, STAT_IN }),\
 /* mental */\
  SK_CONTROL_LYC		: ({ STAT_SD }),\
  SK_MEDITATE_CLEANSING		: ({ STAT_SD, STAT_EM }),\
  SK_MEDITATE_HEALING		: ({ STAT_SD, STAT_EM }),\
  SK_MEDITATE_SLEEP		: ({ STAT_SD, STAT_IN }),\
  SK_PERCEPTION			: ({ STAT_IN, STAT_IN, STAT_RE }),\
  SK_SLA			: ({ STAT_IN }),\
 /* music */\
  SK_DANCE			: ({ STAT_AG, STAT_IN }),\
  SK_PLAY_DRUM                  : ({ STAT_AG, STAT_ME }),\
  SK_PLAY_FLUTE           	: ({ STAT_AG, STAT_ME }),\
  SK_PLAY_GUITAR          	: ({ STAT_AG, STAT_ME }),\
  SK_PLAY_HARP            	: ({ STAT_AG, STAT_ME }),\
  SK_PLAY_HORN            	: ({ STAT_AG, STAT_ME }),\
  SK_PLAY_LYRE            	: ({ STAT_AG, STAT_ME }),\
  SK_PLAY_TRUMPET         	: ({ STAT_AG, STAT_ME }),\
  SK_PLAY_VIOLIN                : ({ STAT_AG, STAT_ME }),\
  SK_SINGING			: ({ STAT_PR, STAT_IN }),\
 /* subterfuge */\
  SK_AMBUSH			: ({ }),\
  SK_DISARM_TRAP		: ({ STAT_IN, STAT_AG }),\
  SK_HIDE			: ({ STAT_SD }),\
  SK_HIDE_ITEM			: ({ STAT_RE, STAT_IN }),\
  SK_PICK_LOCKS			: ({ STAT_IN, STAT_RE, STAT_AG }),\
  SK_PICK_POCKET		: ({ STAT_AG, STAT_IN }),\
  SK_SET_TRAPS			: ({ STAT_RE, STAT_AG }),\
  SK_SILENT_KILL		: ({ STAT_AG, STAT_IN }),\
  SK_STALK			: ({ STAT_AG, STAT_SD }),\
  ])

#define SKILL_ATTRIBUTES (SKILL_ATTRIBUTES1 +\
			  SKILL_ATTRIBUTES2 +\
			  SKILL_ATTRIBUTES3)

#endif
