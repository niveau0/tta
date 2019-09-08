#include <material.h>
#include <properties.h>

int
smash_cmd(object with, object tp)
{
    int type, x_bonus;
    
    if (!tp)
	return -1;

    switch (query_prop(OBJ_MATERIAL))
    {
      case M_SILK:
      case M_LINEN:
      case M_COTTON:
      case M_PAPER:
      case M_WOOL:
      case M_WAX:
	  type = 1;
	  break;
      case M_LEATHER:
      case M_FOOD:
      case M_FLESH:
	  type = 2;
	  break;
      case M_GLASS:
      case M_BONE:
      case M_ICE:
      case M_EARTH:
	  type = 3;
	  break;
      case M_WOOD_FIRST..M_WOOD_LAST:
	  type = 4;
	  break;
      default:
	  return 0;
    }
    if (!with)
    {
	bonus = tp->skill_check(SK_ATTACK_BARE_FIST, 10, 1);
	if (bonus < 0)
	    bonus = 0;
	if ((x_bonus = tp->skill_check(SK_ATTACK_STRIKE, 30, 1)) >= 0)
	    bonus += x_bonus;
	if ((x+bonus = tp->skill_check(SK_ATTACK_BASH, 30, 1)) >= 0)
	    bonus += x_bonus;
	
	return;
    }
    debug("aule",type );    
}
