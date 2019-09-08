#include <skills.h>

#define FUMBLE_DIR "/base/fumble/"

/*
 * Function name: fumble
 * Description:   evaluate a fumble for a given player
 * Arguments:     skill - skill that makes the fumble
 *                weapon - weapon or slot which makes the fumble
 *                player - player who makes the fumble
 *                target - object the weapon aims at
 */
void
fumble(int skill, mixed weapon, object player, object target)
{
    int step;
    switch (random(129))
    {
      case 0..10:    step = 0;  break;
      case 11..21:   step = 1;  break;
      case 22..31:   step = 2;  break;
      case 32..41:   step = 3;  break;
      case 42..50:   step = 4;  break;
      case 51..59:   step = 5;  break;
      case 60..67:   step = 6;  break;
      case 68..75:   step = 7;  break;
      case 76..82:   step = 8;  break;
      case 83..89:   step = 9;  break;
      case 90..95:   step = 10; break;
      case 96..101:  step = 11; break;
      case 102..106: step = 12; break;
      case 107..111: step = 13; break;
      case 112..115: step = 14; break;
      case 116..119: step = 15; break;
      case 120..122: step = 16; break;
      case 123..125: step = 17; break;
      case 126..127: step = 18; break;
      case 128:      step = 19; break;
      default:       error("Wrong fumble step (fumble.c)");
    }
    
    if((skill >= SK_ATTACK_BARE_FIST && skill <= SK_ATTACK_TRAMPLE) ||
       (skill == SK_MA_STRIKE) || (skill == SK_MA_SWEEPS))
    {
	/* fumble all attacks without a weapon */
	call_other(FUMBLE_DIR+"fumble_1", "fumble",
		   skill, weapon, player, target, step);
	return;
    }
    else if ((skill >= SK_WEAPON_ATTACK_FIRST &&
	      skill <= SK_WEAPON_TYPE_1C_LAST) ||
	     (skill >= SK_WEAPON_TYPE_2H_FIRST &&
	      skill <= SK_WEAPON_TYPE_POLE_LAST))
    {
	/* fumble all melee attacks with a weapon */
	call_other(FUMBLE_DIR+"fumble_2", "fumble",
		   skill, weapon, player, target, step);
	return;
    }
    else if (skill >= SK_WEAPON_TYPE_THR_FIRST &&
	     skill <= SK_WEAPON_TYPE_THR_LAST)
    {
	/* throw fumble */
	call_other(FUMBLE_DIR+"fumble_3", "fumble",
		   skill, weapon, player, target, step);
	return;
    }
    else if (skill >= SK_WEAPON_TYPE_BOW_FIRST &&
	     skill <= SK_WEAPON_TYPE_BOW_LAST)
    {
	/* fumble all missile weapons */
	call_other(FUMBLE_DIR+"fumble_4", "fumble",
		   skill, weapon, player, target, step);
	return;
    }
    else
    {
	/* should never happen */
        error("Wrong fumble skill! (fumble.c)");
	return;
    }
}
