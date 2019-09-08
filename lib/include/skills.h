#ifndef SKILLS_H
#define SKILLS_H

#define SK_FIRST                         1
				    
/* academic */			    
#define SK_GRP_ACADEMIC_FIRST            1
#define SK_DEMON_LORE                    2
#define SK_FAUNA_LORE                    3
#define SK_FLORA_LORE                    4
#define SK_HERB_LORE                     5
#define SK_LOCK_LORE                     6
#define SK_MAPPING                       7
#define SK_MATHEMATICS                   8
#define SK_NAVIGATION                    9
#define SK_POISON_LORE                  10
#define SK_WARDING_LORE                 11
#define SK_WEATHER_WATCH                12
#define SK_STONE_LORE                   13
#define SK_GRP_ACADEMIC_LAST            13
/* athletic */			    
#define SK_GRP_ATHLETIC_FIRST          100
#define SK_ACROBATICS                  100
#define SK_CLIMB                       101
#define SK_CONTORTIONS                 102
#define SK_DISTANCE_RUN                103
#define SK_JUMPING                     104
#define SK_RIDE                        105
#define SK_SPRINTING		       106
#define SK_SWIM                        107
#define SK_GRP_ATHLETIC_LAST           107
/* combat */			    
#define SK_GRP_COMBAT_FIRST            200
#define SK_ADRENAL_DEFENSE             200
#define SK_CHAIN                       201
#define SK_DISARM_ARMED                202
#define SK_DISARM_UNARMED              203
#define SK_FRENZY                      204
#define SK_MA_STRIKE                   205
#define SK_MA_SWEEPS                   206
#define SK_PLATE                       207
#define SK_RIGID_LEATHER               208
#define SK_SOFT_LEATHER                209
#define SK_SUBDUING                    210
#define SK_TWO_WEAPONS                 211

#define SK_ATTACK_BARE_FIST            220
#define SK_ATTACK_BASH                 221 /* natural unarmed attacks */
#define SK_ATTACK_BEAK                 222
#define SK_ATTACK_BITE                 223
#define SK_ATTACK_CLAW                 224
#define SK_ATTACK_GRAPPLE              225
#define SK_ATTACK_HORN                 226
#define SK_ATTACK_STINGER              227
#define SK_ATTACK_TINY_ANIMAL          228
#define SK_ATTACK_TRAMPLE              229

#define SK_WEAPON_ATTACK_FIRST         250
#define SK_WEAPON_TYPE_1S_FIRST        250 /* one handed slashing weapons */   
#define SK_ATTACK_BROADSWORD           250
#define SK_ATTACK_DAGGER               251
#define SK_ATTACK_FALCHION             252
#define SK_ATTACK_HAND_AXE             253
#define SK_ATTACK_LONGSWORD            254
#define SK_ATTACK_MAIN_GAUCHE          255
#define SK_ATTACK_RAPIER               256
#define SK_ATTACK_SCIMITAR             257
#define SK_ATTACK_SHORTSWORD           258
#define SK_WEAPON_TYPE_1S_LAST         258

#define SK_WEAPON_TYPE_1C_FIRST        270 /* one handed crushing weapons */   
#define SK_ATTACK_CLUB                 270
#define SK_ATTACK_MACE                 271
#define SK_ATTACK_MORNING_STAR         272
#define SK_ATTACK_WAR_HAMMER           273
#define SK_ATTACK_WHIP                 274
#define SK_WEAPON_TYPE_1C_LAST         274

#define SK_WEAPON_TYPE_BOW_FIRST       290 /* bows and projectile weapons */   
#define SK_ATTACK_COMPOSITEBOW         290
#define SK_ATTACK_HEAVY_CROSSBOW       291
#define SK_ATTACK_LIGHT_CROSSBOW       292
#define SK_ATTACK_LONGBOW              293
#define SK_ATTACK_SHORTBOW             294
#define SK_ATTACK_SLING                295
#define SK_WEAPON_TYPE_BOW_LAST        295

#define SK_WEAPON_TYPE_2H_FIRST        310 /* two handed weapons */	    
#define SK_ATTACK_2_HAND_SWORD         310
#define SK_ATTACK_BATTLE_AXE           311
#define SK_ATTACK_FLAIL                312
#define SK_ATTACK_QUARTERSTAFF         313
#define SK_ATTACK_WAR_MATTOCK          314
#define SK_WEAPON_TYPE_2H_LAST         314

#define SK_WEAPON_TYPE_POLE_FIRST      330 /* pole arms */
#define SK_ATTACK_JAVELIN              330
#define SK_ATTACK_LANCE                331
#define SK_ATTACK_POLE_ARM             332
#define SK_ATTACK_SPEAR                333
#define SK_WEAPON_TYPE_POLE_LAST       333

#define SK_WEAPON_TYPE_THR_FIRST       350 /* thrown weapons */
#define SK_ATTACK_BOLA                 350
#define SK_ATTACK_DAGGER_THR           351
#define SK_ATTACK_HAND_AXE_THR         352
#define SK_ATTACK_JAVELIN_THR          353
#define SK_ATTACK_SHURIKEN             354
#define SK_ATTACK_SPEAR_THR            355
#define SK_ATTACK_THROW                356
#define SK_WEAPON_TYPE_THR_LAST        356
#define SK_WEAPON_ATTACK_LAST          356
#define SK_GRP_COMBAT_LAST             356
/* craft */			    
#define SK_GRP_CRAFT_FIRST             400
#define SK_LEATHER_WORK                400
#define SK_MINING                      401
#define SK_SAILING                     402
#define SK_WOOD_CRAFT                  403
#define SK_METAL_CRAFT                 404
#define SK_FLETCHING                   405
#define SK_GRP_CRAFT_LAST              405
/* general */			    
#define SK_GRP_GENERAL_FIRST           500
#define SK_APPRAISAL                   500
#define SK_COOKERY                     501  
#define SK_EVAL_ARMOR                  502
#define SK_EVAL_WEAPON                 503
#define SK_READ_TRACKS                 504
#define SK_ROPE_MASTERY                505
#define SK_TRADING                     506
#define SK_GRP_GENERAL_LAST            506
/* linguistics */		    
#define SK_GRP_LINGUISTICS_FIRST       600
#define SK_ADUNAIC                     600
#define SK_BLACK_SPEECH                601
#define SK_KHUZDUL                     602
#define SK_QUENYA                      603
#define SK_ROHERIC                     604
#define SK_SINDARIN                    605
#define SK_TALE_TELLING                606
#define SK_WESTRON                     607
#define SK_GRP_LINGUISTICS_LAST        607
/* magic */			    
#define SK_GRP_MAGIC_FIRST             700
#define SK_CHANNELING                  700
#define SK_POWER_PROJ                  701
#define SK_RUNES                       702
#define SK_SENSE_MAGIC                 703 
#define SK_STAVES_WANDS                704
#define SK_TRANS_ARMOR                 705
#define SK_ML_FIRST                    720
#define SK_ML_AIONION                  720
#define SK_ML_AISTHASIS		       721
#define SK_ML_ANATHEMA		       722
#define SK_ML_ASEBEIA		       723
#define SK_ML_ASTER		       724
#define SK_ML_CHAOS		       725
#define SK_ML_CONSCIOUSNESS	       726
#define SK_ML_EARTHBLOOD	       727
#define SK_ML_EIRENAY		       728
#define SK_ML_GNOSIS		       729
#define SK_ML_GRAPHAY		       730
#define SK_ML_HUDATOS		       731
#define SK_ML_IHYAMA		       732
#define SK_ML_IRUARIC		       733
#define SK_ML_KUBEIA		       734
#define SK_ML_LOGOS		       735
#define SK_ML_PARAPATEO		       736
#define SK_ML_PETRA		       737
#define SK_ML_PNEUMATIKOS	       738
#define SK_ML_PORNEIA		       739
#define SK_ML_PUROSIS		       740
#define SK_ML_QADOSH		       741
#define SK_ML_SKOTIA		       742
#define SK_ML_SOMA		       743
#define SK_ML_SPOUDAZO		       744
#define SK_ML_THANATOS		       745
#define SK_ML_TOTEM		       746
#define SK_ML_XZULOU		       747
#define SK_ML_LAST                     747
#define SK_MATTACK_COLD_BALL           760
#define SK_MATTACK_FIRE_BALL           761
#define SK_MATTACK_FIRE_BOLT           762
#define SK_MATTACK_ICE_BOLT            763
#define SK_MATTACK_LIGHT_BOLT          764
#define SK_MATTACK_SHOCK_BOLT          765
#define SK_MATTACK_STONE_THROW         766
#define SK_MATTACK_WATER_BOLT          767
#define SK_GRP_MAGIC_LAST              767
/* medical */			    
#define SK_GRP_MEDICAL_FIRST           800
#define SK_ANIMAL_HEAL                 800
#define SK_DIAGNOSTICS                 801
#define SK_FIRST_AID                   802
#define SK_SECOND_AID                  803
#define SK_GRP_MEDICAL_LAST            803
/* mental */			    
#define SK_GRP_MENTAL_FIRST            900
#define SK_CONTROL_LYC                 900
#define SK_MEDITATE_CLEANSING          901
#define SK_MEDITATE_HEALING            902
#define SK_MEDITATE_SLEEP              903
#define SK_PERCEPTION                  904
#define SK_SLA                         905
#define SK_GRP_MENTAL_LAST             905
/* music */			    
#define SK_GRP_MUSIC_FIRST            1000
#define SK_DANCE                      1000
#define SK_PLAY_DRUM                  1001
#define SK_PLAY_FLUTE                 1002
#define SK_PLAY_GUITAR                1003
#define SK_PLAY_HARP                  1004
#define SK_PLAY_HORN                  1005
#define SK_PLAY_LYRE                  1006
#define SK_PLAY_TRUMPET               1007
#define SK_PLAY_VIOLIN                1008
#define SK_SINGING                    1009
#define SK_GRP_MUSIC_LAST             1009
/* subterfuge */		    
#define SK_GRP_SUBTERFUGE_FIRST       1100
#define SK_AMBUSH                     1100
#define SK_DISARM_TRAP                1101
#define SK_HIDE                       1102
#define SK_HIDE_ITEM                  1103
#define SK_PICK_LOCKS                 1104
#define SK_PICK_POCKET                1105
#define SK_SET_TRAPS                  1106
#define SK_SILENT_KILL                1107
#define SK_STALK                      1108
#define SK_GRP_SUBTERFUGE_LAST        1108
/* covered
 * covered skills have a special purpose and should never
 * appear in SK_DESC
 */
#define SK_GRP_COVERED_FIRST          1200
#define SK_RESIST_FIRST               1200
#define SK_RESIST_MEN                 1200
#define SK_RESIST_ESS		      1201
#define SK_RESIST_CHA		      1202
#define SK_RESIST_POISON	      1203
#define SK_RESIST_DISEASE	      1204
#define SK_RESIST_HEAT		      1205
#define SK_RESIST_COLD		      1206
#define SK_RESIST_IMPACT	      1207
#define SK_RESIST_ELECTRICITY         1208
#define SK_RESIST_LAST                1208
#define SK_BODY_DEVELOP               1209
#define SK_MANA_DEVELOP               1210
#define SK_GRP_COVERED_LAST           1210

#define SK_LAST                       1210

/* this special skill is only used to count the research tries 
 * while learning a spellist
 */
#define SK_SPELLIST_PROGRESS          3000 

#define SK_DESC1 \
([ \
 /* academic */\
 SK_DEMON_LORE                 : "demon lore",\
 SK_FAUNA_LORE                 : "fauna lore",\
 SK_FLORA_LORE                 : "flora lore",\
 SK_HERB_LORE                  : "herb lore",\
 SK_LOCK_LORE                  : "lock lore",\
 SK_MAPPING                    : "mapping",\
 SK_MATHEMATICS                : "mathematics",\
 SK_NAVIGATION                 : "navigation",\
 SK_POISON_LORE                : "poison lore",\
 SK_WARDING_LORE               : "warding lore",\
 SK_WEATHER_WATCH              : "weather watching",\
 SK_STONE_LORE                 : "gem lore",\
 /* athletic */\
 SK_ACROBATICS                 : "acrobatics",\
 SK_BODY_DEVELOP               : "body development",\
 SK_CLIMB                      : "climbing",\
 SK_CONTORTIONS                : "contortions",\
 SK_DISTANCE_RUN               : "distance run",\
 SK_JUMPING                    : "jumping",\
 SK_RIDE                       : "riding",\
 SK_SPRINTING		       : "sprinting",\
 SK_SWIM                       : "swimming",\
 /* combat */\
 SK_ADRENAL_DEFENSE            : "adrenal defense",\
 SK_DISARM_ARMED               : "disarm armed",\
 SK_DISARM_UNARMED             : "disarm unarmed",\
 SK_FRENZY                     : "frenzy",\
 SK_MA_STRIKE                  : "MA striking",\
 SK_MA_SWEEPS                  : "MA sweeping",\
 SK_PLATE                      : "MM plate",\
 SK_CHAIN                      : "MM chain",\
 SK_RIGID_LEATHER              : "MM rigid leather",\
 SK_SOFT_LEATHER               : "MM soft leather",\
 SK_SUBDUING                   : "subduing",\
 SK_TWO_WEAPONS                : "two weapons",\
 SK_ATTACK_2_HAND_SWORD        : "two hand sword",\
 SK_ATTACK_BARE_FIST           : "bare fist",\
 SK_ATTACK_BASH                : "bash",\
 SK_ATTACK_BATTLE_AXE          : "battle axe",\
 SK_ATTACK_BEAK                : "beak",\
 SK_ATTACK_BITE                : "bite",\
 SK_ATTACK_BOLA                : "bola",\
 SK_ATTACK_BROADSWORD          : "broadsword",\
 SK_ATTACK_CLAW                : "claw",\
 SK_ATTACK_CLUB                : "club",\
 SK_ATTACK_COMPOSITEBOW        : "composite bow",\
 SK_ATTACK_DAGGER              : "dagger",\
 SK_ATTACK_DAGGER_THR          : "throw dagger",\
 SK_ATTACK_FALCHION            : "falchion",\
 SK_ATTACK_FLAIL               : "flail",\
 SK_ATTACK_GRAPPLE             : "grapple",\
 SK_ATTACK_HAND_AXE            : "hand axe",\
 SK_ATTACK_HAND_AXE_THR        : "throw hand axe",\
 SK_ATTACK_HEAVY_CROSSBOW      : "heavy crossbow",\
 SK_ATTACK_HORN                : "horn",\
 SK_ATTACK_JAVELIN             : "javelin",\
 SK_ATTACK_JAVELIN_THR         : "throw javelin",\
 SK_ATTACK_LANCE               : "lance",\
 SK_ATTACK_LIGHT_CROSSBOW      : "light crossbow",\
 SK_ATTACK_LONGBOW             : "longbow",\
 SK_ATTACK_LONGSWORD           : "long sword",\
 SK_ATTACK_MACE                : "mace",\
 SK_ATTACK_MAIN_GAUCHE         : "main gauche",\
 SK_ATTACK_MORNING_STAR        : "morning star",\
 SK_ATTACK_POLE_ARM            : "pole arm",\
 SK_ATTACK_QUARTERSTAFF        : "quarterstaff",\
 SK_ATTACK_RAPIER              : "rapier",\
 SK_ATTACK_SCIMITAR            : "scimitar",\
 SK_ATTACK_SHORTBOW            : "shortbow",\
 SK_ATTACK_SHORTSWORD          : "short sword",\
 SK_ATTACK_SHURIKEN            : "throw shuriken",\
 SK_ATTACK_SLING               : "sling",\
 SK_ATTACK_SPEAR               : "spear",\
 SK_ATTACK_SPEAR_THR           : "throw spear",\
 SK_ATTACK_STINGER             : "stinger",\
 SK_ATTACK_THROW               : "throwing",\
 SK_ATTACK_TINY_ANIMAL         : "tiny animal",\
 SK_ATTACK_TRAMPLE             : "trample",\
 SK_ATTACK_WAR_HAMMER          : "war hammer",\
 SK_ATTACK_WAR_MATTOCK         : "war mattock",\
 SK_ATTACK_WHIP                : "whip",\
    ])
    
#define SK_DESC2 \
([ \
 /* craft */\
 SK_LEATHER_WORK         : "leather works",\
 SK_MINING               : "mining",\
 SK_SAILING              : "sailing",\
 SK_WOOD_CRAFT           : "wood craft",\
 SK_METAL_CRAFT          : "metal craft",\
 /* general */\
 SK_APPRAISAL            : "appraisal",\
 SK_COOKERY              : "cookery",\
 SK_EVAL_ARMOR           : "eval armor",\
 SK_EVAL_WEAPON          : "eval weapon",\
 SK_FLETCHING            : "fletching",\
 SK_READ_TRACKS          : "read tracks",\
 SK_ROPE_MASTERY         : "rope mastery",\
 SK_TRADING              : "trading",\
 /* linguistics */\
 SK_ADUNAIC              : "speak adunaic",\
 SK_BLACK_SPEECH         : "speak black speech",\
 SK_KHUZDUL              : "speak khuzdul",\
 SK_QUENYA               : "speak quenya",\
 SK_ROHERIC              : "speak roheric",\
 SK_SINDARIN             : "speak sindarin",\
 SK_TALE_TELLING         : "tale telling",\
 SK_WESTRON              : "speak westron",\
 /* magic */\
 SK_CHANNELING           : "channeling",\
 SK_POWER_PROJ           : "power projection",\
 SK_RUNES                : "reading runes",\
 SK_SENSE_MAGIC          : "sense magic",\
 SK_STAVES_WANDS         : "using staves",\
 SK_TRANS_ARMOR          : "transcend armor",\
 SK_ML_AIONION           : "aionion",\
 SK_ML_AISTHASIS	 : "aisthasis",\
 SK_ML_ANATHEMA	         : "anathema",\
 SK_ML_ASEBEIA		 : "asebeia",\
 SK_ML_ASTER		 : "aster",\
 SK_ML_CHAOS		 : "chaos",\
 SK_ML_CONSCIOUSNESS	 : "consciousness",\
 SK_ML_EARTHBLOOD	 : "earthblood",\
 SK_ML_EIRENAY		 : "eirenay",\
 SK_ML_GNOSIS		 : "gnosis",\
 SK_ML_GRAPHAY		 : "graphay",\
 SK_ML_HUDATOS		 : "hudatos",\
 SK_ML_IHYAMA		 : "ihyama",\
 SK_ML_IRUARIC		 : "iruaric",\
 SK_ML_KUBEIA		 : "kubeia",\
 SK_ML_LOGOS		 : "logos",\
 SK_ML_PARAPATEO	 : "parapateo",\
 SK_ML_PETRA		 : "petra",\
 SK_ML_PNEUMATIKOS	 : "pneumatikos",\
 SK_ML_PORNEIA		 : "porneia",\
 SK_ML_PUROSIS		 : "purosis",\
 SK_ML_QADOSH		 : "qadosh",\
 SK_ML_SKOTIA		 : "skotia",\
 SK_ML_SOMA		 : "soma",\
 SK_ML_SPOUDAZO	         : "spoudazo",\
 SK_ML_THANATOS	         : "thanatos",\
 SK_ML_TOTEM		 : "totem",\
 SK_ML_XZULOU		 : "xzulou",\
 SK_MATTACK_FIRE_BOLT    : "fire bolt",\
 SK_MATTACK_ICE_BOLT     : "ice bolt",\
 SK_MATTACK_LIGHT_BOLT   : "lightning bolt",\
 SK_MATTACK_SHOCK_BOLT   : "shock bolt",\
 SK_MATTACK_STONE_THROW  : "stone throw",\
 SK_MATTACK_WATER_BOLT   : "water bolt",\
 SK_MATTACK_COLD_BALL    : "cold ball",\
 SK_MATTACK_FIRE_BALL    : "fire ball",\
 /* mental */\
 SK_CONTROL_LYC          : "ctrl lycantrophy",\
 SK_MEDITATE_CLEANSING   : "meditation clean",\
 SK_MEDITATE_HEALING     : "meditation heal",\
 SK_MEDITATE_SLEEP       : "meditation sleep",\
 SK_PERCEPTION           : "perception",\
 SK_SLA                  : "spatial loc. aware",\
 /* music */\
 SK_DANCE                : "dance",\
 SK_PLAY_DRUM            : "play drum",\
 SK_PLAY_FLUTE           : "play flute",\
 SK_PLAY_GUITAR          : "play guitar",\
 SK_PLAY_HARP            : "play harp",\
 SK_PLAY_HORN            : "play horn",\
 SK_PLAY_LYRE            : "play lyre",\
 SK_PLAY_TRUMPET         : "play trumpet",\
 SK_PLAY_VIOLIN          : "play violin",\
 SK_SINGING              : "singing",\
 /* medical */\
 SK_ANIMAL_HEAL          : "animal healing",\
 SK_DIAGNOSTICS          : "diagnostics",\
 SK_FIRST_AID            : "first aid",\
 SK_SECOND_AID           : "second aid",\
 /* subterfuge */\
 SK_AMBUSH               : "ambush",\
 SK_DISARM_TRAP          : "disarm traps",\
 SK_HIDE                 : "hide",\
 SK_HIDE_ITEM            : "hide items",\
 SK_PICK_LOCKS           : "pick locks",\
 SK_PICK_POCKET          : "pick pockets",\
 SK_SET_TRAPS            : "set traps",\
 SK_SILENT_KILL          : "silent kill",\
 SK_STALK                : "stalk",\
    ])
    
#define SK_DESC (SK_DESC1 + SK_DESC2)

#define SK_GRP_FIRST         1
#define SK_GRP_ACADEMIC      1
#define SK_GRP_ATHLETIC      2
#define SK_GRP_COMBAT        3
#define SK_GRP_CRAFT         4
#define SK_GRP_GENERAL       5
#define SK_GRP_LINGUISTICS   6
#define SK_GRP_MAGIC         7
#define SK_GRP_MEDICAL       8
#define SK_GRP_MENTAL        9
#define SK_GRP_MUSIC        10
#define SK_GRP_SUBTERFUGE   11
#define SK_GRP_COVERED      12
#define SK_GRP_LAST         12

#define SK_GRP_ALL          13

#endif
