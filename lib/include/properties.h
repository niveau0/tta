#ifndef PROPERTIES_H
#define PROPERTIES_H
 
#define CONT_TRANSP               1000 /* true if transparent */
#define CONT_CLOSED               1001 /* true if closed */
#define CONT_LOCKED		  1002 /* true if locked */
#define CONT_RIGID	          1003 /* true if always at max volume */
#define CONT_LESS_WEIGHT          1004 /* how much % reduced weight */
					    
#define CODER_MSG_ENTER           2000 /* true shows entering players */
#define CODER_REPLY               2001 /* the last coder that send a tell */
#define CODER_PATH                2002 /* predefined paths to for searching */
#define CODER_CURRENT_CONTROL     2003 /* current npc to control with ';' */
#define CODER_NO_CONSUME          2004 /* true if no consume wanted */
#define CODER_HOME                2005 /* path of workroom for 'home' cmd */
					    
#define STATUE_LOCATION           3000 /* last location before linkdeath */
#define STATUE_TIME               3001 /* linkdeath time */
#define SKILL_SUPPORT             3002 /* object skill support */
#define SPELLIST_SUPPORT          3003 /* ([ "realm list" : level ]) */
#define SPELL_CODE                3004 /* spell id, internal usage */
#define CURRENT_EDITOR            3005 /* editor object, if in edit mode */
#define LEARN_TEACH_DATA          3006 /* temporary data for teach cmd */
#define LEARN_RESEARCH_LIST       3007 /* current spellist player learns */
#define LEARN_RESEARCH_SPELL      3008 /* current spell player learns */
#define LEARN_COST_MODIFIER       3009 /* skillcost modifier */
#define RESTING_START             3010 /* resting timestamp */ 
#define SLEEP_START               3011 /* sleep timestamp */
#define UPDOWN_OBJ                3012 /* last object used by up/down cmd */
					    
#define WORKCMD_DATA              3500 /* stored cmd data to stop working */
#define WALKCMD_DATA              3501 /* stored cmd data to stop walking */
#define CONCENTRATE_DATA          3502 /* stored concentration data ... */
#define FOLLOWCMD_DATA            3503 /* stored cmd data for follow */

#define LIVE_SEE_INVIS            4000 /* invis level that can be seen */
#define LIVE_SEE_NO_WALKING       4001 /* if true, show no walking messages */
#define LIVE_HASTED	          4002 /* used for haste spells/items */
#define LIVE_SEE_DARK		  4003 /* infravision/blindness */
#define LIVE_NEVER_BLEED          4004 /* if true, can never bleed */
#define LIVE_NO_CRITICAL          4005 /* if true, gets no critically hits */
#define LIVE_NEVER_ILL            4006 /* if true, is never ill */
#define LIVE_NEVER_UNCONSCIOUS    4007 /* if true, is never unconscious */
#define LIVE_UNDEAD               4008 /* living is undead */
#define LIVE_WALKMODE             4009 /* running, walking, sneaking, etc. */
#define LIVE_STATE                4010 /* state of living (rest, fight...) */
#define LIVE_CONTROLLED_BY        4011 /* if living is controlled */
#define LIVE_BREATH_WATER	  4012 /* if true, can breath under water */
#define LIVE_MANA_MULTIPLY        4013 /* multiply max powerpoints if > 1 */
#define LIVE_ANIMAL               4014 /* living is an animal*/
#define LIVE_SEE_FIGHT            4015 /* see others fighting */
#define LIVE_USING_SLA            4016 /* spatial location awareness */
#define LIVE_LAST_RR_RESULT       4017 /* stores the last resist roll */
#define LIVE_NPC_MAX_FATIGUE      4018 /* max. fatigue of npcs */
#define LIVE_LAST_QUESTION  	  4019 /* used by 'answer' command */
#define LIVE_NO_CONSUME           4020 /* never saved, used for spells */
#define LIVE_MOVE_SPEED   	  4021 /* precalculated step component */
#define LIVE_BLOODFLOW_STOP  	  4022 /* used by flowstop spells */
#define LIVE_SEE_SHORT_DESC       4023 /* if true, look shows short desc */
#define LIVE_ATTACHED_QUIVER      4024 /* the currently used quiver */
#define LIVE_TMP_SPELLKNOWLEDGE   4025 /* used for channeled spells */
#define LIVE_LOST_ROUNDS          4026 /* how many combat rounds to lose */
#define LIVE_POSSESSED            4027 /* possessed object before linkdeath */
#define LIVE_NOISE_REDUCTION	  4028 /* how much noise is swallowed */
#define LIVE_TMP_SKILLS           4029 /* temporal skill boni/mali */
#define LIVE_DEAFNESS             4030 /* how deaf is this living */
#define LIVE_MOUNTED              4031 /* object mounted (e.g. horse)*/
#define LIVE_BUTCHERED            4032 /* living is butchered (-1 disables),
					  (-2 corpse is already butchered) */
#define LIVE_SKINNED              4033 /* living is skinned (-1 disables) */
#define LIVE_AUTO_DRINK           4034 /* player drinks when thirsty */
#define LIVE_AUTO_EAT             4035 /* player eats when hungry */

/* saved living properties */
#define LIVE_KNOWN_SPELLS         4500 /* mapping of known spells */
#define LIVE_FIGHT_MOOD           4501 /* attack mood */
#define LIVE_TRADING              4502 /* if true, living bargains */
#define LIVE_USED_LANGUAGE        4503 /* currently used language */
#define LIVE_GET_ECHO             4504 /* if true, echo messages */
#define LIVE_NO_AUTOATTACK        4505 /* if true, no auto attacking */
#define LIVE_LONG_INVENTORY       4506 /* if true, more inventory info */
#define LIVE_PREPARE_RNDS         4507 /* spell preparation time */
#define LIVE_KNOWN_RECIPES        4508 /* mapping of known recipes */

#define OBJ_NO_GET                6001 /* object cannot be taken */
#define OBJ_INVIS		  6002 /* invisible level */
#define OBJ_HIDE                  6003 /* hide level */
#define OBJ_HIDE_PLACES           6004 /* where is object hidden */
#define OBJ_MAGIC                 6005 /* magic level */
#define OBJ_NEVER_BREAK           6006 /* true cannot break */
#define OBJ_NO_SHOW               6007 /* true will not be shown */
#define OBJ_MATERIAL              6009 /* material */
#define OBJ_THIN_MATERIAL         6010 /* allows more things to wear */
#define OBJ_OFFERED_ITEMS         6011 /* currently offered items data */
#define OBJ_ACCEPT_STATUS         6012 /* if true, object accepts all */
#define OBJ_ATTACH_SIZE           6013 /* allowed minimum attachslot size */
#define OBJ_BUYER                 6014 /* the living which bought this item */
#define OBJ_DAMAGED               6015 /* how damaged an object is (in %)
					  (-1 is not repairable) */
#define OBJ_LYING_ON              6016 /* the object this one lies on */


#define IS_ROOM		          7000 /* object counts as room */
#define IS_CONT		          7001 /* object counts as container */
#define IS_HEAP		          7002 /* object counts as herb */
#define IS_HERB		          7003 /* object counts as herb */
#define IS_PLANT		  7004 /* object counts as plant */
#define IS_ARMOR		  7005 /* object counts as armor */
#define IS_WEAPON		  7006 /* object counts as weapon */
#define IS_MISSILE_WEAPON	  7007 /* object counts as missile weapon */
#define IS_SHIELD		  7008 /* object counts as a shield */
#define IS_WIELDABLE		  7009 /* object can be wielded */
#define IS_WEARABLE		  7010 /* object can be worn */
#define IS_POISON                 7011 /* object is a poison */
#define IS_KEY                    7012 /* object useable as key somewhere */
#define IS_AMMO                   7013 /* object counts as ammo */

#define PLAYER_MORE_LEN		  8000 /* page length for 'more' */
#define PLAYER_NEWS               8001 /* login news timestamp */

#define TYPE_INSIDE		(2<<0)  
#define TYPE_UNDER_GROUND	(2<<1)  
#define TYPE_UNDER_WATER	(2<<2)  
#define TYPE_OCEAN		(2<<3)  
#define TYPE_COAST		(2<<4)  
#define TYPE_RIVER		(2<<5)  
#define TYPE_LAKE		(2<<6)  
#define TYPE_HILLS		(2<<7) 
#define TYPE_MOUNTAINS		(2<<8) 
#define TYPE_SWAMP		(2<<9) 
#define TYPE_WASTE		(2<<10) 
#define TYPE_SHORT_GRASS	(2<<11) 
#define TYPE_LONG_GRASS		(2<<12) 
#define TYPE_LIGHT_FOREST	(2<<13) 
#define TYPE_DENSE_FOREST	(2<<14) 
#define TYPE_JUNGLE		(2<<15) 
#define TYPE_VILLAGE		(2<<16) 
#define TYPE_TOWN		(2<<17) 
#define TYPE_SHOP		(2<<18) 

#define SAVE_PROPS \
({ \
       CODER_PATH, \
       CODER_MSG_ENTER, \
       CODER_NO_CONSUME, \
       CODER_HOME, \
       \
       LEARN_RESEARCH_LIST,\
       \
       LIVE_KNOWN_SPELLS, \
       LIVE_FIGHT_MOOD, \
       LIVE_TRADING, \
       LIVE_USED_LANGUAGE, \
       LIVE_GET_ECHO, \
       LIVE_NO_AUTOATTACK, \
       LIVE_LONG_INVENTORY, \
       LIVE_PREPARE_RNDS, \
       LIVE_KNOWN_RECIPES, \
       \
       PLAYER_MORE_LEN, \
       PLAYER_NEWS, \
       })
 
#endif
