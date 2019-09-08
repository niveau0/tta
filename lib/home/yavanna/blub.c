inherit "/base/living";
 
#include <combat.h>
#include <living.h>
#include <race.h>
#include <skills.h>
#include <slots.h>
#include <stdproperties.h>

void
create_living()
{
    int vol;

    set_name("hobbit");
    set_race("hobbit");
    set_long("He seems to be an experienced hobbit fighter.\n");
    set_gender(G_MALE);
    add_lwvs(0, 23000, 25000, HOBBIT_MIN + random(HOBBIT_MAX-HOBBIT_MIN));

    set_tmp_stat(STAT_CO, 50 + random(30));
    set_tmp_stat(STAT_AG, 50 + random(30));
    set_tmp_stat(STAT_SD, 30 + random(20));
    set_tmp_stat(STAT_ME, 40 + random(20));
    set_tmp_stat(STAT_RE, 40 + random(20));
    set_tmp_stat(STAT_ST, 40 + random(30));
    set_tmp_stat(STAT_QU, 50 + random(20));
    set_tmp_stat(STAT_PR, 25 + random(25));
    set_tmp_stat(STAT_EM, 35 + random(20));
    set_tmp_stat(STAT_IN, 40 + random(20));
    set_tmp_stat(STAT_AP, 40 + random(20));

    set_skill_rank(SK_ATTACK_DAGGER, 2 + random(5));
    set_skill_rank(SK_ATTACK_SHORT_SWORD, 2 + random(5));
    set_skill_rank(SK_PLATE, 2 + random(15));
    set_skill_rank(SK_CHAIN, 2 + random(10));
    set_skill_rank(SK_RIGID_LEATHER, 2 + random(5));
    set_skill_rank(SK_TRADING, random(25));
    set_skill_rank(SK_WESTRON, 20);
    set_skill_rank(SK_SINDARIN, random(15));
    set_skill_rank(SK_ATTACK_BARE_FIST, 5);
    cb_set_unarmed_skill(SK_ATTACK_BARE_FIST);
    
    set_base_hp(9000 + random(10) * 8);
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 500);
    set_fatigue(query_max_fatigue());
    
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

    add_prop(LIVE_USED_LANGUAGE, SK_WESTRON);
    add_prop(LIVE_FIGHT_MOOD, 75);
/*		    
    wear_tool(add_equipment("/lib/armor/s_leather/hobbit_bracer"));
    wear_tool(add_equipment("/lib/armor/r_leather/hobbit_bracer"));
    */
    wear_tool(add_equipment("/lib/armor/plate/hobbit_plate"));
    wear_tool(add_equipment("/lib/armor/s_leather/small_helmet"));
    if (random(2))
	wield_tool(add_equipment("/lib/weapon/one_h_s/dagger"));
    else
	wield_tool(add_equipment("/lib/weapon/one_h_s/shortsword"));
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	  return ({ SK_ATTACK_BARE_FIST, 114, 159, 250, 290, 315,
		    ({ CRUSH }) });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ ATT_SKIN, 0, 60, 6 });
      case (SLT_CHEST + 1):
	return ({ ATT_SKIN, 0, 60, 21 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ ATT_SKIN, 0, 60, 7 });
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	return ({ ATT_SKIN, 0, 60, 3 });
      case (SLT_ABDOMEN + 1):
	return ({ ATT_SKIN, 0, 60, 16 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ ATT_SKIN, 0, 60, 16 });
    }
    return 0;
}

varargs void
catch_tell(string msg, int no_prompt)
{
    find_player("yavanna")->catch_tell("##" + msg, no_promt);
}


