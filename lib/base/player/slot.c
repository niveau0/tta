#include <combat.h>
#include <effects.h>
#include <slots.h>
#include <skills.h>
#include <properties.h>
#include "/base/living/prototypes.h"

/*
 * Function name: slot_player_reset
 * Description:   set the tool slots for the player
 *                must be called after player is loaded
 */
static nomask void
slot_player_reset()
{
    int vol;

    vol = query_volume() / 100;

    add_tool_slot(SLT_HEAD+1,    "head",          vol * 6);
    add_tool_slot(SLT_EAR+1,     "right ear",     5);
    add_tool_slot(SLT_EAR+2,     "left ear",      5);
    add_tool_slot(SLT_EYE+1,     "right eye",     1);
    add_tool_slot(SLT_EYE+2,     "left eye",      1);
    add_tool_slot(SLT_NOSE+1,    "nose",          1);
    add_tool_slot(SLT_TONGUE+1,  "tongue",        1);
    add_tool_slot(SLT_NECK+1,    "neck",          5);
    add_tool_slot(SLT_CHEST+1,   "chest",         vol * 21);
    add_tool_slot(SLT_BACK+1,    "back",          vol * 21);
    add_tool_slot(SLT_ARM+1,     "right arm",     vol * 7);
    add_tool_slot(SLT_ARM+2,     "left arm",      vol * 7);
    add_tool_slot(SLT_HAND+1,    "right hand",    vol * 3);
    add_tool_slot(SLT_HAND+2,    "left hand",     vol * 3);
    add_tool_slot(SLT_ABDOMEN+1, "abdomen",       vol * 16);
    add_tool_slot(SLT_LEG+1,     "right leg",     vol * 16);
    add_tool_slot(SLT_LEG+2,     "left leg",      vol * 16);
    add_tool_slot(SLT_FOOT+1,    "right foot",    vol * 2);
    add_tool_slot(SLT_FOOT+2,    "left foot",     vol * 2);

    set_connections(([
		      SLT_HEAD+1:({ SLT_EYE+1, SLT_EYE+2 }),
		      SLT_LEG+1:({ SLT_FOOT+1 }),
		      SLT_LEG+2:({ SLT_FOOT+2 }),
		      SLT_ARM+1:({ SLT_HAND+1 }),
		      SLT_ARM+2:({ SLT_HAND+2 }),
		      ]));

    cb_set_parry_skill(SK_ATTACK_BARE_FIST);
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	  return ({ SK_ATTACK_BARE_FIST, ATMOD_BARE_FIST, OBMOD_BARE_FIST });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ A_SKIN, 6 });
      case (SLT_CHEST + 1):
	return ({ A_SKIN, 21 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ A_SKIN, 7 });
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	return ({ A_SKIN, 3 });
      case (SLT_ABDOMEN + 1):
	return ({ A_SKIN, 16 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ A_SKIN, 16 });
      case (SLT_FOOT + 1):
      case (SLT_FOOT + 2):
	return ({ A_SKIN, 2 });
    }
    return 0;
}
