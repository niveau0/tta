/*
 * skill related attributes
 */

#ifndef SIMILAR
#define SIMILAR

#define SIMILAR_SKILLS_1 \
([ \
 /* academic */\
 SK_DEMON_LORE                 : ([ ]),\
 SK_FAUNA_LORE                 : ([ ]),\
 SK_FLORA_LORE                 : ([ SK_HERB_LAW       : 4 ]),\
 SK_HERB_LORE                  : ([ SK_FLORA_LAW      : 2   ,\
                                    SK_POISON_LORE    : 8 ]),\
 SK_LOCK_LORE                  : ([ SK_PICK_LOCKS     : 4 ]),\
 SK_MAPPING                    : ([ SK_NAVIGATION     : 4 ]),\
 SK_MATHEMATICS                : ([ ]),\
 SK_NAVIGATION                 : ([ SK_MAPPING        : 4 ]),\
 SK_POISON_LORE                : ([ SK_HERB_LORE      : 8 ]),\
 SK_WARDING_LORE               : ([ ]),\
 SK_WEATHER_WATCH              : ([ ]),\
 /* athletic */\
 SK_ACROBATICS                 : ([ SK_BODY_DEVELOP   : 8 ]),\
 SK_BODY_DEVELOP               : ([ SK_ACROBATICS     : 4   ,\
				    SK_CLIMB          : 8 ]),\
 SK_CLIMB                      : ([ SK_BODY_DEVELOP   : 8   ,\
				    SK_ACROBATICS     : 8 ]),\
 SK_CONTORTIONS                : ([ ]),\
 SK_DISTANCE_RUN               : ([ SK_SPRINTING      : 4 ]),\
 SK_JUMPING                    : ([ SK_SPRINTING      : 8 ]),\
 SK_RIDE                       : ([ ]),\
 SK_SPRINTING		       : ([ SK_DISTANCE_RUN   : 4 ]),\
 SK_SWIM                       : ([ SK_BODY_DEVELOP   : 8 ]),\
 /* combat */\
 SK_ADRENAL_DEFENSE            : ([ ]),\
 SK_DISARM_ARMED               : ([ SK_DISARM_UNARMED : 1 ]),\
 SK_DISARM_UNARMED             : ([ SK_DISARM_ARMED   : 1 ]),\
 SK_FRENZY                     : ([ ]),\
 SK_MA_STRIKE                  : ([ SK_DISARM_UNARMED : 1 ]),\
 SK_MA_SWEEPS                  : ([ SK_DISARM_UNARMED : 1 ]),\
 SK_PLATE                      : ([ SK_RIGID_LEATHER  : 1 ]),\
 SK_CHAIN                      : ([ SK_SOFT_LEATHER   : 1 ]),\
 SK_RIGID_LEATHER              : ([ SK_PLATE          : 1 ]),\
 SK_SOFT_LEATHER               : ([ SK_CHAIN          : 1 ]),\
 SK_SUBDUING                   : ([ SK_FIRST_AID      : 1 ]),\
 SK_TWO_WEAPONS                : ([ ]),\
 SK_ATTACK_2_HAND_SWORD        : ([ ]),\
 SK_ATTACK_BARE_FIST           : ([ ]),\
 SK_ATTACK_BASH                : ([ ]),\
 SK_ATTACK_BATTLE_AXE          : ([ ]),\
 SK_ATTACK_BEAK                : ([ ]),\
 SK_ATTACK_BITE                : ([ ]),\
 SK_ATTACK_BOLA                : ([ ]),\
 SK_ATTACK_BROADSWORD          : ([ ]),\
 SK_ATTACK_CLAW                : ([ ]),\
 SK_ATTACK_CLUB                : ([ ]),\
 SK_ATTACK_COMPOSITEBOW        : ([ ]),\
 SK_ATTACK_DAGGER              : ([ ]),\
 SK_ATTACK_DAGGER_THR          : ([ ]),\
 SK_ATTACK_FALCHION            : ([ ]),\
 SK_ATTACK_FLAIL               : ([ ]),\
 SK_ATTACK_GRAPPLE             : ([ ]),\
 SK_ATTACK_HAND_AXE            : ([ ]),\
 SK_ATTACK_HAND_AXE_THR        : ([ ]),\
 SK_ATTACK_HEAVY_CROSSBOW      : ([ ]),\
 SK_ATTACK_HORN                : ([ ]),\
 SK_ATTACK_JAVELIN             : ([ ]),\
 SK_ATTACK_JAVELIN_THR         : ([ ]),\
 SK_ATTACK_LANCE               : ([ ]),\
 SK_ATTACK_LIGHT_CROSSBOW      : ([ ]),\
 SK_ATTACK_LONGBOW             : ([ ]),\
 SK_ATTACK_LONGSWORD           : ([ ]),\
 SK_ATTACK_MACE                : ([ ]),\
 SK_ATTACK_MAIN_GAUCHE         : ([ ]),\
 SK_ATTACK_MORNING_STAR        : ([ ]),\
 SK_ATTACK_POLE_ARM            : ([ ]),\
 SK_ATTACK_QUARTERSTAFF        : ([ ]),\
 SK_ATTACK_RAPIER              : ([ ]),\
 SK_ATTACK_SCIMITAR            : ([ ]),\
 SK_ATTACK_SHORTBOW            : ([ ]),\
 SK_ATTACK_SHORTSWORD          : ([ ]),\
 SK_ATTACK_SHURIKEN            : ([ ]),\
 SK_ATTACK_SLING               : ([ ]),\
 SK_ATTACK_SPEAR               : ([ ]),\
 SK_ATTACK_SPEAR_THR           : ([ ]),\
 SK_ATTACK_STINGER             : ([ ]),\
 SK_ATTACK_THROW               : ([ ]),\
 SK_ATTACK_TINY_ANIMAL         : ([ ]),\
 SK_ATTACK_TRAMPLE             : ([ ]),\
 SK_ATTACK_WAR_HAMMER          : ([ ]),\
 SK_ATTACK_WAR_MATTOCK         : ([ ]),\
 SK_ATTACK_WHIP                : ([ ]),\
    ])

#define SIMILAR_SKILLS_2 \
([ \
 /* craft */\
 SK_LEATHER_WORK         : ([ ]),\
 SK_MINING               : ([ ]),\
 SK_SAILING              : ([ SK_NAVIGATION         : 1   ,\
			      SK_MAPPING            : 1 ]),\
 SK_WOOD_CRAFT           : ([ ]),\
 /* general */\
 SK_APPRAISAL            : ([ SK_EVAL_ARMOR         : 1   ,\
			      SK_EVAL_WEAPON        : 1 ]),\
 SK_COOKERY              : ([ SK_HERB_LORE          : 1   ,\
			      SK_POISON_LORE        : 1 ]),\
 SK_EVAL_ARMOR           : ([ SK_EVAL_WEAPON        : 1   ,\
			      SK_APPRAISAL          : 1 ]),\
 SK_EVAL_WEAPON          : ([ SK_EVAL_ARMOR         : 1   ,\
			      SK_APPRAISAL          : 1 ]),\
 SK_FLETCHING            : ([ SK_WOOD_CRAFT         : 1 ]),\
 SK_READ_TRACKS          : ([ SK_HIDE               : 1   ,\
			      SK_STALK              : 1 ]),\
 SK_ROPE_MASTERY         : ([ ]),\
 SK_TRADING              : ([ SK_APPRAISAL          : 1 ]),\
 /* linguistics */\
 SK_ADUNAIC              : ([ SK_ROHERIC            : 1   ,\
			      SK_WESTRON            : 1 ]),\
 SK_BLACK_SPEECH         : ([ ]),\
 SK_KHUZDUL              : ([ ]),\
 SK_QUENYA               : ([ SK_SINDARIN           : 1 ]),\
 SK_ROHERIC              : ([ SK_ADUNAIC            : 1   ,\
			      SK_WESTRON            : 1 ]),\
 SK_SINDARIN             : ([ SK_QUENYA             : 1 ]),\
 SK_TALE_TELLING         : ([ ]),\
 SK_WESTRON              : ([ SK_ROHERIC            : 1   ,\
			      SK_ADUNAIC            : 1 ]),\
 /* magic */\
 SK_CHANNELING           : ([ SK_POWER_PROJ         : 1   ,\
			      SK_TRANS_ARMOR        : 1 ]),\
 SK_POWER_PROJ           : ([ SK_CHANNELING         : 1 ]),\
 SK_RUNES                : ([ ]),\
 SK_SENSE_MAGIC          : ([ ]),\
 SK_STAVES_WANDS         : ([ ]),\
 SK_TRANS_ARMOR          : ([ SK_CHANNELING         : 1 ]),\
 SK_ML_AIONION           : ([ ]),\
 SK_ML_AISTHASIS	 : ([ ]),\
 SK_ML_ANATHEMA	         : ([ ]),\
 SK_ML_ASEBEIA		 : ([ ]),\
 SK_ML_ASTER		 : ([ ]),\
 SK_ML_CHAOS		 : ([ ]),\
 SK_ML_CONSCIOUSNESS	 : ([ ]),\
 SK_ML_EARTHBLOOD	 : ([ ]),\
 SK_ML_EIRENAY		 : ([ ]),\
 SK_ML_GNOSIS		 : ([ ]),\
 SK_ML_GRAPHAY		 : ([ ]),\
 SK_ML_HUDATOS		 : ([ ]),\
 SK_ML_IHYAMA		 : ([ ]),\
 SK_ML_IRUARIC		 : ([ ]),\
 SK_ML_KUBEIA		 : ([ ]),\
 SK_ML_LOGOS		 : ([ ]),\
 SK_ML_PARAPATEO	 : ([ ]),\
 SK_ML_PETRA		 : ([ ]),\
 SK_ML_PNEUMATIKOS	 : ([ ]),\
 SK_ML_PORNEIA		 : ([ ]),\
 SK_ML_PUROSIS		 : ([ ]),\
 SK_ML_QADOSH		 : ([ ]),\
 SK_ML_SKOTIA		 : ([ ]),\
 SK_ML_SOMA		 : ([ ]),\
 SK_ML_SPOUDAZO	         : ([ ]),\
 SK_ML_THANATOS	         : ([ ]),\
 SK_ML_TOTEM		 : ([ ]),\
 SK_ML_XZULOU		 : ([ ]),\
 SK_MATTACK_FIRE_BOLT    : ([ SK_MATTACK_ICE_BOLT   : 1   ,\
			      SK_MATTACK_LIGHT_BOLT : 1   ,\
			      SK_MATTACK_SHOCK_BOLT : 1   ,\
			      SK_MATTACK_WATER_BOLT : 1 ]),\
 SK_MATTACK_ICE_BOLT     : ([ SK_MATTACK_LIGHT_BOLT : 1   ,\
			      SK_MATTACK_WATER_BOLT : 1   ,\
			      SK_MATTACK_SHOCK_BOLT : 1   ,\
			      SK_MATTACK_FIRE_BOLT  : 1 ]),\
 SK_MATTACK_LIGHT_BOLT   : ([ SK_MATTACK_ICE_BOLT   : 1   ,\
			      SK_MATTACK_WATER_BOLT : 1   ,\
			      SK_MATTACK_SHOCK_BOLT : 1   ,\
			      SK_MATTACK_FIRE_BOLT  : 1 ]),\
 SK_MATTACK_SHOCK_BOLT   : ([ SK_MATTACK_ICE_BOLT   : 1   ,\
			      SK_MATTACK_LIGHT_BOLT : 1   ,\
			      SK_MATTACK_WATER_BOLT : 1   ,\
			      SK_MATTACK_FIRE_BOLT  : 1 ]),\
 SK_MATTACK_WATER_BOLT   : ([ SK_MATTACK_ICE_BOLT   : 1   ,\
			      SK_MATTACK_LIGHT_BOLT : 1   ,\
			      SK_MATTACK_SHOCK_BOLT : 1   ,\
			      SK_MATTACK_FIRE_BOLT  : 1 ]),\
 SK_MATTACK_STONE_THROW  : ([ ]),\
 SK_MATTACK_COLD_BALL    : ([ SK_MATTACK_FIRE_BALL  : 1 ]),\
 SK_MATTACK_FIRE_BALL    : ([ SK_MATTACK_COLD_BALL  : 1 ]),\
 /* mental */\
 SK_CONTROL_LYC          : ([ ]),\
 SK_MEDITATE_CLEANSING   : ([ SK_MEDITATE_HEALING   : 1   ,\
			      SK_MEDITATE_SLEEP     : 1 ]),\
 SK_MEDITATE_HEALING     : ([ SK_MEDITATE_CLEANSING : 1   ,\
			      SK_MEDITATE_SLEEP     : 1 ]),\
 SK_MEDITATE_SLEEP       : ([ SK_MEDITATE_CLEANSING : 1   ,\
			      SK_MEDITATE_HEALING   : 1 ]),\
 SK_PERCEPTION           : ([ SK_SLA                : 1 ]),\
 SK_SLA                  : ([ SK_PERCEPTION         : 1 ]),\
 /* music */\
 SK_DANCE                : ([ ]),\
 SK_PLAY_DRUM            : ([ ]),\
 SK_PLAY_FLUTE           : ([ SK_PLAY_HORN          : 1   ,\
			      SK_PLAY_TRUMPET       : 1 ]),\
 SK_PLAY_GUITAR          : ([ SK_PLAY_LYRE          : 1   ,\
			      SK_PLAY_HARP          : 1 ]),\
 SK_PLAY_HARP            : ([ SK_PLAY_LYRE          : 1   ,\
			      SK_PLAY_GUITAR        : 1 ]),\
 SK_PLAY_HORN            : ([ SK_PLAY_TRUMPET       : 1   ,\
			      SK_PLAY_FLUTE         : 1 ]),\
 SK_PLAY_LYRE            : ([ SK_PLAY_HARP          : 1   ,\
			      SK_PLAY_GUITAR        : 1 ]),\
 SK_PLAY_TRUMPET         : ([ SK_PLAY_HORN          : 1   ,\
			      SK_PLAY_FLUTE         : 1 ]),\
 SK_PLAY_VIOLIN          : ([ ]),\
 SK_SINGING              : ([ ]),\
 /* medical */\
 SK_ANIMAL_HEAL          : ([ SK_FIRST_AID          : 1   ,\
			      SK_RIDE               : 1 ]),\
 SK_DIAGNOSTICS          : ([ SK_FIRST_AID          : 1 ]),\
 SK_FIRST_AID            : ([ SK_SECOND_AID         : 1   ,\
			      SK_ANIMAL_HEAL        : 1   ,\
			      SK_SUBDUING           : 1   ,\
			      SK_DIAGNOSTICS        : 1 ]),\
 SK_SECOND_AID           : ([ SK_FIRST_AID          : 1   ,\
			      SK_ANIMAL_HEAL        : 1 ]),\
 /* subterfuge */\
 SK_AMBUSH               : ([ SK_SILENT_KILL        : 1   ,\
			      SK_STALK              : 1   ,\
			      SK_HIDE               : 1 ]),\
 SK_DISARM_TRAP          : ([ SK_PICK_LOCKS         : 1   ,\
			      SK_SET_TRAPS          : 1 ]),\
 SK_HIDE                 : ([ SK_STALK              : 1 ]),\
 SK_HIDE_ITEM            : ([ ]),\
 SK_PICK_LOCKS           : ([ SK_LOCK_LORE          : 1 ]),\
 SK_PICK_POCKET          : ([ ]),\
 SK_SET_TRAPS            : ([ SK_DISARM_TRAP        : 1 ]),\
 SK_SILENT_KILL          : ([ SK_AMBUSH             : 1   ,\
			      SK_STALK              : 1 ]),\
 SK_STALK                : ([ SK_HIDE               : 1 ]),\
    ])

#define SIMILAR_SKILLS  (SIMILAR_SKILLS_1 + SIMILAR_SKILLS_2)
#endif
