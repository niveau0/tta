inherit "/base/mount";

#include <combat.h>
#include <living.h>
#include <macros.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>

static nomask void
create_mount()
{   
    set_name("unicorn");
    add_name("cila");
    set_race("unicorn");
    set_long("A pure white female unicorn with a golden horn \n"+
             "and sad black eyes.\n"+
             "She looks peacefully in your direction.\n");
    set_gender(G_FEMALE);
    add_adj("white");
    add_lwvs(0, 300000, 330000, 85);
    
    set_tmp_stat(STAT_CO, 70 + random(30));
    set_tmp_stat(STAT_AG, 70 + random(30));
    set_tmp_stat(STAT_SD, 50 + random(20));
    set_tmp_stat(STAT_ME, 50 + random(20));
    set_tmp_stat(STAT_RE, 40 + random(10));
    set_tmp_stat(STAT_ST, 70 + random(30));
    set_tmp_stat(STAT_QU, 70 + random(30));
    set_tmp_stat(STAT_PR, 50 + random(20));
    set_tmp_stat(STAT_EM, 70 + random(30));
    set_tmp_stat(STAT_IN, 70 + random(30));
    set_tmp_stat(STAT_AP, 80 + random(20));
    

    set_pot_stat(STAT_CO, 100);
    set_pot_stat(STAT_AG, 100);
    set_pot_stat(STAT_SD, 70);
    set_pot_stat(STAT_ME, 70);
    set_pot_stat(STAT_RE, 50);
    set_pot_stat(STAT_ST, 100);
    set_pot_stat(STAT_QU, 100);
    set_pot_stat(STAT_PR, 70);
    set_pot_stat(STAT_EM, 100);
    set_pot_stat(STAT_IN, 100);
    set_pot_stat(STAT_AP, 100);

    set_skill_rank(SK_ATTACK_BASH, 40);
    set_skill_rank(SK_ATTACK_HORN, 80);
    cb_set_parry_skill(SK_ATTACK_HORN);
    set_move_msg("trots");
    set_run_msg("gallops");

    set_base_hp(60);
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 1000);
    set_fatigue(query_max_fatigue());
    
    add_tool_slot(SLT_HEAD+1,    "golden horn",         1);
    add_tool_slot(SLT_HEAD+2,    "head",          55 *  6);
    add_tool_slot(SLT_MOUTH+1,   "teeth",               1);
    add_tool_slot(SLT_EYE+1,     "right eye",           1);
    add_tool_slot(SLT_EYE+2,     "left eye",            1);
    add_tool_slot(SLT_CHEST+1,   "chest",         55 * 21);
    add_tool_slot(SLT_ARM+1,     "right foreleg", 55 *  7);
    add_tool_slot(SLT_ARM+2,     "left foreleg",  55 *  7);
    add_tool_slot(SLT_ABDOMEN+1, "abdomen",       55 * 16);
    add_tool_slot(SLT_LEG+1,     "right hindleg", 55 * 16);
    add_tool_slot(SLT_LEG+2,     "left hindleg",  55 * 16);
    add_tool_slot(SLT_BACK+1,    "tail",                1);

    set_connections(([
                      SLT_HEAD+2:({ SLT_HEAD+1, SLT_MOUTH+1,
                                    SLT_EYE+1, SLT_EYE+2 }),
                      SLT_CHEST+1:({ SLT_HEAD+2,
                                     SLT_ARM+1, SLT_ARM+2 }),
                      SLT_ABDOMEN+1:({ SLT_CHEST+1,
                                       SLT_LEG+1, SLT_LEG+2,
				       SLT_BACK+1 })
                      ]));

    add_prop(LIVE_FIGHT_MOOD, 60);

    set_skill_rank(SK_SINDARIN, 20);
    set_skill_rank(SK_QUENYA, 20);
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	  return ({ SK_ATTACK_BASH, ATMOD_BASH, OBMOD_BASH });
      case (SLT_HEAD + 1):
	  return ({ SK_ATTACK_HORN, ATMOD_HORN, OBMOD_BASH });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ A_PLATE, 4 });
      case (SLT_HEAD + 2):
	return ({ A_S_LEATHER, 6 });
      case (SLT_MOUTH + 1):
	return ({ A_SKIN, 3 });
      case (SLT_EYE + 1):
      case (SLT_EYE + 2):
	return ({ A_SKIN, 2 });
      case (SLT_CHEST + 1):
	return ({ A_S_LEATHER, 17 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ A_S_LEATHER, 13 });
      case (SLT_ABDOMEN + 1):
	return ({ A_S_LEATHER, 15 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ A_S_LEATHER, 10 });
      case (SLT_BACK + 1):
	return ({ A_SKIN, 5 });
    }
    return 0;
}

varargs void
catch_tell(string msg, int no_prompt)
{
    string komando, name;

    sscanf(msg, "%s %*s", name);
    if (name=="Alyn")
    {
	sscanf(msg, "%*s %s\n",komando);
	switch (komando)
	{
	  case ("whistles twice loudly."):
	  {
	      command("run" + " " + "alyn");
	      break;
	  }
	  default:
	  {
	  }
	}
    }
    ::catch_tell(msg, no_prompt);
}
