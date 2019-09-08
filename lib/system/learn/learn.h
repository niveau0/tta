#include "attribs.h"

#define TEACH_MIN_RANK 15

#define LEARN_GROUP_RANGE \
([ \
 SK_GRP_ALL         : ({ SK_GRP_ACADEMIC_FIRST, \
			     SK_GRP_ACADEMIC_LAST, \
			     SK_GRP_ATHLETIC_FIRST, \
			     SK_GRP_ATHLETIC_LAST, \
			     SK_GRP_COMBAT_FIRST, \
			     SK_GRP_COMBAT_LAST, \
			     SK_GRP_CRAFT_FIRST, \
			     SK_GRP_CRAFT_LAST, \
			     SK_GRP_GENERAL_FIRST, \
			     SK_GRP_GENERAL_LAST, \
			     SK_GRP_LINGUISTICS_FIRST, \
			     SK_GRP_LINGUISTICS_LAST, \
			     SK_GRP_MAGIC_FIRST, \
			     SK_GRP_MAGIC_LAST, \
			     SK_GRP_MEDICAL_FIRST, \
			     SK_GRP_MEDICAL_LAST, \
			     SK_GRP_MENTAL_FIRST, \
			     SK_GRP_MENTAL_LAST, \
			     SK_GRP_MUSIC_FIRST, \
			     SK_GRP_MUSIC_LAST, \
			     SK_GRP_SUBTERFUGE_FIRST, \
			     SK_GRP_SUBTERFUGE_LAST }), \
 SK_GRP_ACADEMIC    : ({ SK_GRP_ACADEMIC_FIRST,\
                             SK_GRP_ACADEMIC_LAST }), \
 SK_GRP_ATHLETIC    : ({ SK_GRP_ATHLETIC_FIRST,\
                             SK_GRP_ATHLETIC_LAST }), \
 SK_GRP_COMBAT      : ({ SK_GRP_COMBAT_FIRST,\
                             SK_GRP_COMBAT_LAST }), \
 SK_GRP_CRAFT       : ({ SK_GRP_CRAFT_FIRST,\
                             SK_GRP_CRAFT_LAST }), \
 SK_GRP_GENERAL     : ({ SK_GRP_GENERAL_FIRST,\
                             SK_GRP_GENERAL_LAST }), \
 SK_GRP_LINGUISTICS : ({ SK_GRP_LINGUISTICS_FIRST,\
                             SK_GRP_LINGUISTICS_LAST }), \
 SK_GRP_MAGIC       : ({ SK_GRP_MAGIC_FIRST,\
                             SK_GRP_MAGIC_LAST }), \
 SK_GRP_MEDICAL     : ({ SK_GRP_MEDICAL_FIRST,\
                             SK_GRP_MEDICAL_LAST }), \
 SK_GRP_MENTAL      : ({ SK_GRP_MENTAL_FIRST,\
                             SK_GRP_MENTAL_LAST }), \
 SK_GRP_MUSIC       : ({ SK_GRP_MUSIC_FIRST,\
                             SK_GRP_MUSIC_LAST }), \
 SK_GRP_SUBTERFUGE  : ({ SK_GRP_SUBTERFUGE_FIRST,\
                             SK_GRP_SUBTERFUGE_LAST }), \
 SK_GRP_COVERED     : ({ SK_GRP_COVERED_FIRST,\
                             SK_GRP_COVERED_LAST }), \
    ])

#define LEARN_GROUP_TO_NAMES \
([ \
 SK_GRP_ACADEMIC    : "academic", \
 SK_GRP_ATHLETIC    : "athletic", \
 SK_GRP_COMBAT      : "combat", \
 SK_GRP_CRAFT       : "craft", \
 SK_GRP_GENERAL     : "general", \
 SK_GRP_LINGUISTICS : "linguistics", \
 SK_GRP_MAGIC       : "magic", \
 SK_GRP_MEDICAL     : "medical", \
 SK_GRP_MENTAL      : "mental", \
 SK_GRP_MUSIC       : "music", \
 SK_GRP_SUBTERFUGE  : "subterfuge", \
    ])

#define LEARN_NAMES_TO_GROUP \
([ \
 "academic"    : SK_GRP_ACADEMIC, \
 "athletic"    : SK_GRP_ATHLETIC, \
 "combat"      : SK_GRP_COMBAT, \
 "craft"       : SK_GRP_CRAFT, \
 "general"     : SK_GRP_GENERAL, \
 "linguistics" : SK_GRP_LINGUISTICS, \
 "magic"       : SK_GRP_MAGIC, \
 "medical"     : SK_GRP_MEDICAL, \
 "mental"      : SK_GRP_MENTAL, \
 "music"       : SK_GRP_MUSIC, \
 "subterfuge"  : SK_GRP_SUBTERFUGE, \
 "all"         : SK_GRP_ALL, \
    ])


#define LEARN_SKILL_GROUPS (LEARN_SKILL_GROUPS1 + \
                            LEARN_SKILL_GROUPS2 + \
                            LEARN_SKILL_GROUPS3 )
#define LEARN_SKILL_GROUPS1 \
([ \
 SK_DEMON_LORE             : ({ SK_GRP_ACADEMIC, }), \
 SK_FAUNA_LORE             : ({ SK_GRP_ACADEMIC, }), \
 SK_FLORA_LORE             : ({ SK_GRP_ACADEMIC, }), \
 SK_HERB_LORE              : ({ SK_GRP_ACADEMIC, }), \
 SK_LOCK_LORE              : ({ SK_GRP_ACADEMIC, SK_GRP_SUBTERFUGE, }), \
 SK_MAPPING                : ({ SK_GRP_ACADEMIC, }), \
 SK_MATHEMATICS            : ({ SK_GRP_ACADEMIC, }), \
 SK_NAVIGATION             : ({ SK_GRP_ACADEMIC, }), \
 SK_POISON_LORE            : ({ SK_GRP_ACADEMIC, SK_GRP_SUBTERFUGE, }), \
 SK_WARDING_LORE           : ({ SK_GRP_ACADEMIC, SK_GRP_MAGIC, }), \
 SK_WEATHER_WATCH          : ({ SK_GRP_ACADEMIC, }), \
 SK_STONE_LORE             : ({ SK_GRP_ACADEMIC, }), \
 SK_ACROBATICS             : ({ SK_GRP_ATHLETIC, }), \
 SK_BODY_DEVELOP           : ({ SK_GRP_ATHLETIC, }), \
 SK_CLIMB                  : ({ SK_GRP_ATHLETIC, }), \
 SK_CONTORTIONS            : ({ SK_GRP_ATHLETIC, }), \
 SK_DISTANCE_RUN           : ({ SK_GRP_ATHLETIC, }), \
 SK_JUMPING                : ({ SK_GRP_ATHLETIC, }), \
 SK_RIDE                   : ({ SK_GRP_ATHLETIC, }), \
 SK_SPRINTING              : ({ SK_GRP_ATHLETIC, }), \
 SK_SWIM                   : ({ SK_GRP_ATHLETIC, SK_GRP_MENTAL, }), \
 \
 SK_ATTACK_BASH            : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_BEAK            : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_BITE            : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_CLAW            : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_GRAPPLE         : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_HORN            : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_STINGER         : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_TINY_ANIMAL     : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_TRAMPLE         : ({ SK_GRP_COMBAT, }), \
 \
 SK_ATTACK_2_HAND_SWORD    : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_BARE_FIST       : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_BATTLE_AXE      : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_BOLA            : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_BROADSWORD      : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_CLUB            : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_COMPOSITEBOW    : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_DAGGER          : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_DAGGER_THR      : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_FALCHION        : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_FLAIL           : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_HAND_AXE        : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_HAND_AXE_THR    : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_HEAVY_CROSSBOW  : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_JAVELIN         : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_JAVELIN_THR     : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_LANCE           : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_LIGHT_CROSSBOW  : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_LONGBOW         : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_LONGSWORD       : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_MACE            : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_MAIN_GAUCHE     : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_MORNING_STAR    : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_POLE_ARM        : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_QUARTERSTAFF    : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_RAPIER          : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_SCIMITAR        : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_SHORTBOW        : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_SHORTSWORD      : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_SHURIKEN        : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_SLING           : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_SPEAR           : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_SPEAR_THR       : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_THROW           : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_WAR_HAMMER      : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_WAR_MATTOCK     : ({ SK_GRP_COMBAT, }), \
 SK_ATTACK_WHIP            : ({ SK_GRP_COMBAT, }), \
 \
 SK_PLATE                  : ({ SK_GRP_COMBAT, }), \
 SK_RIGID_LEATHER          : ({ SK_GRP_COMBAT, }), \
 SK_SOFT_LEATHER           : ({ SK_GRP_COMBAT, }), \
 SK_CHAIN                  : ({ SK_GRP_COMBAT, }), \
 \
 SK_ADRENAL_DEFENSE        : ({ SK_GRP_COMBAT, SK_GRP_ATHLETIC, }), \
 SK_DISARM_ARMED           : ({ SK_GRP_COMBAT, }), \
 SK_DISARM_UNARMED         : ({ SK_GRP_COMBAT, SK_GRP_ATHLETIC, }), \
 SK_FRENZY                 : ({ SK_GRP_COMBAT, SK_GRP_MENTAL, }), \
 SK_MA_STRIKE              : ({ SK_GRP_COMBAT, SK_GRP_ATHLETIC, }), \
 SK_MA_SWEEPS              : ({ SK_GRP_COMBAT, SK_GRP_ATHLETIC, }), \
 SK_SUBDUING               : ({ SK_GRP_COMBAT, SK_GRP_MEDICAL, }), \
 SK_TWO_WEAPONS            : ({ SK_GRP_COMBAT, SK_GRP_MENTAL, }), \
 \
 SK_LEATHER_WORK           : ({ SK_GRP_CRAFT, }), \
 SK_MINING                 : ({ SK_GRP_CRAFT, }), \
 SK_SAILING                : ({ SK_GRP_CRAFT, }), \
 SK_WOOD_CRAFT             : ({ SK_GRP_CRAFT, }), \
 SK_METAL_CRAFT            : ({ SK_GRP_CRAFT, }), \
 SK_FLETCHING              : ({ SK_GRP_CRAFT, }), \
 \
 SK_APPRAISAL              : ({ SK_GRP_GENERAL, }), \
 SK_COOKERY                : ({ SK_GRP_GENERAL, }), \
 SK_EVAL_ARMOR             : ({ SK_GRP_GENERAL, }), \
 SK_EVAL_WEAPON            : ({ SK_GRP_GENERAL, }), \
 SK_READ_TRACKS            : ({ SK_GRP_GENERAL, }), \
 SK_ROPE_MASTERY           : ({ SK_GRP_GENERAL, }), \
 SK_TRADING                : ({ SK_GRP_GENERAL, SK_GRP_LINGUISTICS, }), \
     ])
#define LEARN_SKILL_GROUPS2 \
([ \
 SK_ADUNAIC                : ({ SK_GRP_LINGUISTICS, }), \
 SK_BLACK_SPEECH           : ({ SK_GRP_LINGUISTICS, }), \
 SK_KHUZDUL                : ({ SK_GRP_LINGUISTICS, }), \
 SK_QUENYA                 : ({ SK_GRP_LINGUISTICS, }), \
 SK_ROHERIC                : ({ SK_GRP_LINGUISTICS, }), \
 SK_SINDARIN               : ({ SK_GRP_LINGUISTICS, }), \
 SK_TALE_TELLING           : ({ SK_GRP_LINGUISTICS, }), \
 SK_WESTRON                : ({ SK_GRP_LINGUISTICS, }), \
 \
 SK_CHANNELING             : ({ SK_GRP_MAGIC, }), \
 SK_POWER_PROJ             : ({ SK_GRP_MAGIC, SK_GRP_MENTAL, }), \
 SK_RUNES                  : ({ SK_GRP_MAGIC, SK_GRP_ACADEMIC, }), \
 SK_SENSE_MAGIC            : ({ SK_GRP_MAGIC, SK_GRP_MENTAL, }), \
 SK_STAVES_WANDS           : ({ SK_GRP_MAGIC, }), \
 SK_TRANS_ARMOR            : ({ SK_GRP_MAGIC, }), \
 \
 SK_ML_AIONION             : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_AISTHASIS           : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_ANATHEMA            : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_ASEBEIA             : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_ASTER               : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_CHAOS               : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_CONSCIOUSNESS       : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_EARTHBLOOD          : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_EIRENAY             : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_GNOSIS              : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_GRAPHAY             : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_HUDATOS             : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_IHYAMA              : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_IRUARIC             : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_KUBEIA              : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_LOGOS               : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_PARAPATEO           : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_PETRA               : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_PNEUMATIKOS         : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_PORNEIA             : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_PUROSIS             : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_QADOSH              : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_SKOTIA              : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_SOMA                : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_SPOUDAZO            : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_THANATOS            : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_TOTEM               : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 SK_ML_XZULOU              : ({ SK_GRP_MAGIC, SK_GRP_LINGUISTICS, }), \
 \
 SK_MATTACK_COLD_BALL      : ({ SK_GRP_MAGIC, }), \
 SK_MATTACK_FIRE_BALL      : ({ SK_GRP_MAGIC, }), \
 SK_MATTACK_FIRE_BOLT      : ({ SK_GRP_MAGIC, }), \
 SK_MATTACK_ICE_BOLT       : ({ SK_GRP_MAGIC, }), \
 SK_MATTACK_LIGHT_BOLT     : ({ SK_GRP_MAGIC, }), \
 SK_MATTACK_SHOCK_BOLT     : ({ SK_GRP_MAGIC, }), \
 SK_MATTACK_STONE_THROW    : ({ SK_GRP_MAGIC, }), \
 SK_MATTACK_WATER_BOLT     : ({ SK_GRP_MAGIC, }), \
    ])

#define LEARN_SKILL_GROUPS3 \
([ \
 SK_ANIMAL_HEAL            : ({ SK_GRP_MEDICAL, }), \
 SK_DIAGNOSTICS            : ({ SK_GRP_MEDICAL, }), \
 SK_FIRST_AID              : ({ SK_GRP_MEDICAL, }), \
 SK_SECOND_AID             : ({ SK_GRP_MEDICAL, }), \
 \
 SK_CONTROL_LYC            : ({ SK_GRP_MENTAL, }), \
 SK_MEDITATE_CLEANSING     : ({ SK_GRP_MENTAL, }), \
 SK_MEDITATE_HEALING       : ({ SK_GRP_MENTAL, }), \
 SK_MEDITATE_SLEEP         : ({ SK_GRP_MENTAL, }), \
 SK_PERCEPTION             : ({ SK_GRP_MENTAL, }), \
 SK_SLA                    : ({ SK_GRP_MENTAL, }), \
 \
 SK_DANCE                  : ({ SK_GRP_MUSIC, SK_GRP_ATHLETIC, }), \
 SK_SINGING                : ({ SK_GRP_MUSIC, }), \
 SK_PLAY_DRUM              : ({ SK_GRP_MUSIC, }), \
 SK_PLAY_FLUTE             : ({ SK_GRP_MUSIC, }), \
 SK_PLAY_GUITAR            : ({ SK_GRP_MUSIC, }), \
 SK_PLAY_HARP              : ({ SK_GRP_MUSIC, }), \
 SK_PLAY_HORN              : ({ SK_GRP_MUSIC, }), \
 SK_PLAY_LYRE              : ({ SK_GRP_MUSIC, }), \
 SK_PLAY_TRUMPET           : ({ SK_GRP_MUSIC, }), \
 SK_PLAY_VIOLIN            : ({ SK_GRP_MUSIC, }), \
 \
 SK_AMBUSH                 : ({ SK_GRP_SUBTERFUGE, }), \
 SK_DISARM_TRAP            : ({ SK_GRP_SUBTERFUGE, }), \
 SK_HIDE                   : ({ SK_GRP_SUBTERFUGE, }), \
 SK_HIDE_ITEM              : ({ SK_GRP_SUBTERFUGE, }), \
 SK_PICK_LOCKS             : ({ SK_GRP_SUBTERFUGE, }), \
 SK_PICK_POCKET            : ({ SK_GRP_SUBTERFUGE, }), \
 SK_SET_TRAPS              : ({ SK_GRP_SUBTERFUGE, }), \
 SK_SILENT_KILL            : ({ SK_GRP_SUBTERFUGE, }), \
 SK_STALK                  : ({ SK_GRP_SUBTERFUGE, SK_GRP_ATHLETIC, }), \
    ])

/* 
 * skills that must be teached by persons (also npc)
 */

#define LEARN_NEEDS_TEACHER \
([ \
 /* academic    */ \
 SK_HERB_LORE              : 1, \
 SK_LOCK_LORE              : 1, \
 SK_MAPPING                : 1, \
 SK_NAVIGATION             : 1, \
 SK_WARDING_LORE           : 1, \
 SK_WEATHER_WATCH          : 1, \
 SK_STONE_LORE             : 1, \
 /* athletic    */ \
 /* combat      */ \
 SK_ATTACK_2_HAND_SWORD    : 1, \
 SK_ATTACK_BATTLE_AXE      : 1, \
 SK_ATTACK_BOLA            : 1, \
 SK_ATTACK_BROADSWORD      : 1, \
 SK_ATTACK_CLUB            : 1, \
 SK_ATTACK_COMPOSITEBOW    : 1, \
 SK_ATTACK_DAGGER          : 1, \
 SK_ATTACK_DAGGER_THR      : 1, \
 SK_ATTACK_FALCHION        : 1, \
 SK_ATTACK_FLAIL           : 1, \
 SK_ATTACK_HAND_AXE        : 1, \
 SK_ATTACK_HAND_AXE_THR    : 1, \
 SK_ATTACK_HEAVY_CROSSBOW  : 1, \
 SK_ATTACK_JAVELIN         : 1, \
 SK_ATTACK_JAVELIN_THR     : 1, \
 SK_ATTACK_LANCE           : 1, \
 SK_ATTACK_LIGHT_CROSSBOW  : 1, \
 SK_ATTACK_LONGBOW         : 1, \
 SK_ATTACK_LONGSWORD       : 1, \
 SK_ATTACK_MACE            : 1, \
 SK_ATTACK_MAIN_GAUCHE     : 1, \
 SK_ATTACK_MORNING_STAR    : 1, \
 SK_ATTACK_POLE_ARM        : 1, \
 SK_ATTACK_QUARTERSTAFF    : 1, \
 SK_ATTACK_RAPIER          : 1, \
 SK_ATTACK_SCIMITAR        : 1, \
 SK_ATTACK_SHORTBOW        : 1, \
 SK_ATTACK_SHORTSWORD      : 1, \
 SK_ATTACK_SHURIKEN        : 1, \
 SK_ATTACK_SLING           : 1, \
 SK_ATTACK_SPEAR           : 1, \
 SK_ATTACK_SPEAR_THR       : 1, \
 SK_ATTACK_THROW           : 1, \
 SK_ATTACK_WAR_HAMMER      : 1, \
 SK_ATTACK_WAR_MATTOCK     : 1, \
 SK_ATTACK_WHIP            : 1, \
 \
 SK_ADRENAL_DEFENSE        : 1, \
 SK_DISARM_ARMED           : 1, \
 SK_DISARM_UNARMED         : 1, \
 SK_MA_STRIKE              : 1, \
 SK_MA_SWEEPS              : 1, \
 SK_SUBDUING               : 1, \
 SK_TWO_WEAPONS            : 1, \
 /* craft       */ \
 SK_LEATHER_WORK           : 1, \
 SK_MINING                 : 1, \
 SK_SAILING                : 1, \
 SK_WOOD_CRAFT             : 1, \
 SK_METAL_CRAFT            : 1, \
 SK_FLETCHING              : 1, \
 /* general     */ \
 SK_COOKERY                : 1, \
 SK_READ_TRACKS            : 1, \
 SK_ROPE_MASTERY           : 1, \
 /* linguistics */ \
 /* magic       */ \
 SK_RUNES                  : 1, \
 SK_SENSE_MAGIC            : 1, \
 SK_STAVES_WANDS           : 1, \
 /* medical     */ \
 SK_ANIMAL_HEAL            : 1, \
 SK_DIAGNOSTICS            : 1, \
 SK_FIRST_AID              : 1, \
 SK_SECOND_AID             : 1, \
 /* mental      */ \
 /* music       */ \
 SK_DANCE                  : 1, \
 SK_PLAY_DRUM              : 1, \
 SK_PLAY_FLUTE             : 1, \
 SK_PLAY_GUITAR            : 1, \
 SK_PLAY_HARP              : 1, \
 SK_PLAY_HORN              : 1, \
 SK_PLAY_LYRE              : 1, \
 SK_PLAY_TRUMPET           : 1, \
 SK_PLAY_VIOLIN            : 1, \
 /* subterfuge  */ \
 SK_DISARM_TRAP            : 1, \
 SK_PICK_LOCKS             : 1, \
 SK_SET_TRAPS              : 1, \
 SK_SILENT_KILL            : 1, \
    ])

/* 
 * skills that can only be learned with a special object
 */
#define LEARN_NEEDS_OBJECT \
([ \
 /* academic    */ \
 SK_DEMON_LORE             : 1, \
 SK_FAUNA_LORE             : 1, \
 SK_FLORA_LORE             : 1, \
 SK_HERB_LORE              : 1, \
 SK_LOCK_LORE              : 1, \
 SK_MAPPING                : 1, \
 SK_MATHEMATICS            : 1, \
 SK_POISON_LORE            : 1, \
 SK_WARDING_LORE           : 1, \
 SK_STONE_LORE             : 1, \
 /* athletic    */ \
 SK_RIDE                   : 1, \
 /* combat      */ \
 SK_ATTACK_2_HAND_SWORD    : 1, \
 SK_ATTACK_BATTLE_AXE      : 1, \
 SK_ATTACK_BOLA            : 1, \
 SK_ATTACK_BROADSWORD      : 1, \
 SK_ATTACK_CLUB            : 1, \
 SK_ATTACK_COMPOSITEBOW    : 1, \
 SK_ATTACK_DAGGER          : 1, \
 SK_ATTACK_DAGGER_THR      : 1, \
 SK_ATTACK_FALCHION        : 1, \
 SK_ATTACK_FLAIL           : 1, \
 SK_ATTACK_HAND_AXE        : 1, \
 SK_ATTACK_HAND_AXE_THR    : 1, \
 SK_ATTACK_HEAVY_CROSSBOW  : 1, \
 SK_ATTACK_JAVELIN         : 1, \
 SK_ATTACK_JAVELIN_THR     : 1, \
 SK_ATTACK_LANCE           : 1, \
 SK_ATTACK_LIGHT_CROSSBOW  : 1, \
 SK_ATTACK_LONGBOW         : 1, \
 SK_ATTACK_LONGSWORD       : 1, \
 SK_ATTACK_MACE            : 1, \
 SK_ATTACK_MAIN_GAUCHE     : 1, \
 SK_ATTACK_MORNING_STAR    : 1, \
 SK_ATTACK_POLE_ARM        : 1, \
 SK_ATTACK_QUARTERSTAFF    : 1, \
 SK_ATTACK_RAPIER          : 1, \
 SK_ATTACK_SCIMITAR        : 1, \
 SK_ATTACK_SHORTBOW        : 1, \
 SK_ATTACK_SHORTSWORD      : 1, \
 SK_ATTACK_SHURIKEN        : 1, \
 SK_ATTACK_SLING           : 1, \
 SK_ATTACK_SPEAR           : 1, \
 SK_ATTACK_SPEAR_THR       : 1, \
 SK_ATTACK_THROW           : 1, \
 SK_ATTACK_WAR_HAMMER      : 1, \
 SK_ATTACK_WAR_MATTOCK     : 1, \
 SK_ATTACK_WHIP            : 1, \
 \
 SK_PLATE                  : 1, \
 SK_RIGID_LEATHER          : 1, \
 SK_SOFT_LEATHER           : 1, \
 SK_CHAIN                  : 1, \
 \
 SK_DISARM_ARMED           : 1, \
 SK_DISARM_UNARMED         : 1, \
 /* craft       */ \
 SK_LEATHER_WORK           : 1, \
 SK_WOOD_CRAFT             : 1, \
 SK_METAL_CRAFT            : 1, \
 /* general     */ \
 SK_ROPE_MASTERY           : 1, \
 /* linguistics */ \
 /* magic       */ \
 SK_STAVES_WANDS           : 1, \
 /* medical     */ \
 /* mental      */ \
 /* music       */ \
 SK_PLAY_DRUM              : 1, \
 SK_PLAY_FLUTE             : 1, \
 SK_PLAY_GUITAR            : 1, \
 SK_PLAY_HARP              : 1, \
 SK_PLAY_HORN              : 1, \
 SK_PLAY_LYRE              : 1, \
 SK_PLAY_TRUMPET           : 1, \
 SK_PLAY_VIOLIN            : 1, \
 /* subterfuge  */ \
 SK_DISARM_TRAP            : 1, \
 SK_SET_TRAPS              : 1, \
    ])

/* 
 * skills that can only be learned inside special rooms
 */
#define LEARN_NEEDS_LOCATION \
([ \
 /* academic    */ \
 /* athletic    */ \
 SK_CLIMB                  : 1, \
 SK_SWIM                   : 1, \
 /* combat      */ \
 /* craft       */ \
 SK_MINING                 : 1, \
 /* general     */ \
 /* linguistics */ \
 /* magic       */ \
 /* medical     */ \
 /* mental      */ \
 /* music       */ \
 /* subterfuge  */ \
    ])

/* 
 * the factors, reflecting the difficulty to raise the practical rank
 * to the theoretical rank
 */
#define LEARN_FACTORS1 \
([ \
 /* academic    */ \
 SK_DEMON_LORE             : -1, \
 SK_FAUNA_LORE             : -1, \
 SK_FLORA_LORE             : -1, \
 SK_HERB_LORE              : -1, \
 SK_LOCK_LORE              : -1, \
 SK_MAPPING                : 1, \
 SK_MATHEMATICS            : 2, \
 SK_NAVIGATION             : 1, \
 SK_POISON_LORE            : -1, \
 SK_WARDING_LORE           : -1, \
 SK_WEATHER_WATCH          : 1, \
 SK_STONE_LORE             : -1, \
 /* athletic    */ \
 SK_ACROBATICS             : 2, \
 SK_BODY_DEVELOP           : -1, \
 SK_CLIMB                  : 1, \
 SK_CONTORTIONS            : 3, \
 SK_DISTANCE_RUN           : 1, \
 SK_JUMPING                : 3, \
 SK_RIDE                   : 1, \
 SK_SPRINTING              : 2, \
 SK_SWIM                   : 2, \
 /* combat      */ \
 SK_ATTACK_BASH            : 1, \
 SK_ATTACK_BEAK            : 1, \
 SK_ATTACK_BITE            : 1, \
 SK_ATTACK_CLAW            : 1, \
 SK_ATTACK_GRAPPLE         : 1, \
 SK_ATTACK_HORN            : 1, \
 SK_ATTACK_STINGER         : 1, \
 SK_ATTACK_TINY_ANIMAL     : 1, \
 SK_ATTACK_TRAMPLE         : 1, \
 \
 SK_ATTACK_2_HAND_SWORD    : 1, \
 SK_ATTACK_BARE_FIST       : 1, \
 SK_ATTACK_BATTLE_AXE      : 1, \
 SK_ATTACK_BOLA            : 3, \
 SK_ATTACK_BROADSWORD      : 1, \
 SK_ATTACK_CLUB            : 1, \
 SK_ATTACK_COMPOSITEBOW    : 2, \
 SK_ATTACK_DAGGER          : 1, \
 SK_ATTACK_DAGGER_THR      : 2, \
 SK_ATTACK_FALCHION        : 1, \
 SK_ATTACK_FLAIL           : 1, \
 SK_ATTACK_HAND_AXE        : 1, \
 SK_ATTACK_HAND_AXE_THR    : 2, \
 SK_ATTACK_HEAVY_CROSSBOW  : 2, \
 SK_ATTACK_JAVELIN         : 1, \
 SK_ATTACK_JAVELIN_THR     : 2, \
 SK_ATTACK_LANCE           : 1, \
 SK_ATTACK_LIGHT_CROSSBOW  : 2, \
 SK_ATTACK_LONGBOW         : 2, \
 SK_ATTACK_LONGSWORD       : 1, \
 SK_ATTACK_MACE            : 1, \
 SK_ATTACK_MAIN_GAUCHE     : 1, \
 SK_ATTACK_MORNING_STAR    : 2, \
 SK_ATTACK_POLE_ARM        : 1, \
 SK_ATTACK_QUARTERSTAFF    : 1, \
 SK_ATTACK_RAPIER          : 1, \
 SK_ATTACK_SCIMITAR        : 1, \
 SK_ATTACK_SHORTBOW        : 1, \
 SK_ATTACK_SHORTSWORD      : 1, \
 SK_ATTACK_SHURIKEN        : 2, \
 SK_ATTACK_SLING           : 1, \
 SK_ATTACK_SPEAR           : 1, \
 SK_ATTACK_SPEAR_THR       : 2, \
 SK_ATTACK_THROW           : 1, \
 SK_ATTACK_WAR_HAMMER      : 1, \
 SK_ATTACK_WAR_MATTOCK     : 1, \
 SK_ATTACK_WHIP            : 2, \
 \
 SK_PLATE                  : 1, \
 SK_RIGID_LEATHER          : 1, \
 SK_SOFT_LEATHER           : 1, \
 SK_CHAIN                  : 1, \
 \
 SK_ADRENAL_DEFENSE        : 3, \
 SK_DISARM_ARMED           : 1, \
 SK_DISARM_UNARMED         : 2, \
 SK_FRENZY                 : 2, \
 SK_MA_STRIKE              : 2, \
 SK_MA_SWEEPS              : 1, \
 SK_SUBDUING               : 2, \
 SK_TWO_WEAPONS            : 3, \
 /* craft       */ \
 SK_LEATHER_WORK           : 2, \
 SK_MINING                 : 1, \
 SK_SAILING                : 1, \
 SK_WOOD_CRAFT             : 1, \
 SK_METAL_CRAFT            : 2, \
 SK_FLETCHING              : 2, \
 /* general     */ \
 SK_APPRAISAL              : -1, \
 SK_COOKERY                : 1, \
 SK_EVAL_ARMOR             : -1, \
 SK_EVAL_WEAPON            : -1, \
 SK_READ_TRACKS            : 2, \
 SK_ROPE_MASTERY           : 2, \
 SK_TRADING                : 3, \
 /* linguistics */ \
 SK_ADUNAIC                : 3, \
 SK_BLACK_SPEECH           : 3, \
 SK_KHUZDUL                : 3, \
 SK_QUENYA                 : 3, \
 SK_ROHERIC                : 3, \
 SK_SINDARIN               : 3, \
 SK_TALE_TELLING           : 2, \
 SK_WESTRON                : 3, \
    ])

#define LEARN_FACTORS2 \
([ \
 /* magic       */ \
 SK_CHANNELING             : 2, \
 SK_POWER_PROJ             : 2, \
 SK_RUNES                  : -1, \
 SK_SENSE_MAGIC            : 2, \
 SK_STAVES_WANDS           : 1, \
 SK_TRANS_ARMOR            : 4, \
 \
 SK_ML_AIONION             : 2, \
 SK_ML_AISTHASIS           : 2, \
 SK_ML_ANATHEMA            : 2, \
 SK_ML_ASEBEIA             : 2, \
 SK_ML_ASTER               : 2, \
 SK_ML_CHAOS               : 2, \
 SK_ML_CONSCIOUSNESS       : 2, \
 SK_ML_EARTHBLOOD          : 2, \
 SK_ML_EIRENAY             : 2, \
 SK_ML_GNOSIS              : 2, \
 SK_ML_GRAPHAY             : 2, \
 SK_ML_HUDATOS             : 2, \
 SK_ML_IHYAMA              : 2, \
 SK_ML_IRUARIC             : 2, \
 SK_ML_KUBEIA              : 2, \
 SK_ML_LOGOS               : 2, \
 SK_ML_PARAPATEO           : 2, \
 SK_ML_PETRA               : 2, \
 SK_ML_PNEUMATIKOS         : 2, \
 SK_ML_PORNEIA             : 2, \
 SK_ML_PUROSIS             : 2, \
 SK_ML_QADOSH              : 2, \
 SK_ML_SKOTIA              : 2, \
 SK_ML_SOMA                : 2, \
 SK_ML_SPOUDAZO            : 2, \
 SK_ML_THANATOS            : 2, \
 SK_ML_TOTEM               : 2, \
 SK_ML_XZULOU              : 2, \
 \
 SK_MATTACK_COLD_BALL      : 1, \
 SK_MATTACK_FIRE_BALL      : 1, \
 SK_MATTACK_FIRE_BOLT      : 1, \
 SK_MATTACK_ICE_BOLT       : 1, \
 SK_MATTACK_LIGHT_BOLT     : 1, \
 SK_MATTACK_SHOCK_BOLT     : 1, \
 SK_MATTACK_STONE_THROW    : 1, \
 SK_MATTACK_WATER_BOLT     : 1, \
 /* medical     */ \
 SK_ANIMAL_HEAL            : 1, \
 SK_DIAGNOSTICS            : -1, \
 SK_FIRST_AID              : 1, \
 SK_SECOND_AID             : 2, \
 /* mental      */ \
 SK_CONTROL_LYC            : 2, \
 SK_MEDITATE_CLEANSING     : 2, \
 SK_MEDITATE_HEALING       : 1, \
 SK_MEDITATE_SLEEP         : 1, \
 SK_PERCEPTION             : 2, \
 SK_SLA                    : 3, \
 /* music       */ \
 SK_DANCE                  : 1, \
 SK_SINGING                : 1, \
 SK_PLAY_DRUM              : 2, \
 SK_PLAY_FLUTE             : 2, \
 SK_PLAY_GUITAR            : 2, \
 SK_PLAY_HARP              : 2, \
 SK_PLAY_HORN              : 2, \
 SK_PLAY_LYRE              : 2, \
 SK_PLAY_TRUMPET           : 2, \
 SK_PLAY_VIOLIN            : 2, \
 /* subterfuge  */ \
 SK_AMBUSH                 : 3, \
 SK_DISARM_TRAP            : 2, \
 SK_HIDE                   : 2, \
 SK_HIDE_ITEM              : 1, \
 SK_PICK_LOCKS             : 2, \
 SK_PICK_POCKET            : 3, \
 SK_SET_TRAPS              : 1, \
 SK_SILENT_KILL            : 1, \
 SK_STALK                  : 2, \
    ])

#define LEARN_FACTORS (LEARN_FACTORS1 + \
		       LEARN_FACTORS2)
