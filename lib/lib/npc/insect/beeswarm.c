inherit "/base/living";

#include <living.h>
#include <slots.h>
#include <skills.h>
#include <combat.h>
#include <properties.h>

static void
create_living()
{
    add_name("swarm");
    set_race("bees");
    set_long("A swarm of honey bees.\n");
    set_gender(G_NEUTER);
    add_adj("swarm of");
    add_lwvs(0, 5, 5, 1);

    set_tmp_stat(STAT_CO,  1 + random(5));
    set_tmp_stat(STAT_AG, 60 + random(20));
    set_tmp_stat(STAT_SD,  1 + random(5));
    set_tmp_stat(STAT_ME,  1 + random(5));
    set_tmp_stat(STAT_RE,  1 + random(5));
    set_tmp_stat(STAT_ST,  1 + random(5));
    set_tmp_stat(STAT_QU, 60 + random(20));
    set_tmp_stat(STAT_PR,  1 + random(5));
    set_tmp_stat(STAT_EM,  1 + random(5));
    set_tmp_stat(STAT_IN,  1 + random(5));
    set_tmp_stat(STAT_AP, 60 + random(20));

    set_skill_rank(SK_ATTACK_STINGER, 0);
    cb_set_parry_skill(SK_ATTACK_STINGER);
    set_move_msg("buzzes");
    set_run_msg("buzzes");
    
    set_base_hp(random(50));
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 10000);
    set_fatigue(query_max_fatigue());
    
    add_tool_slot(SLT_CHEST + 1,     "some bees",  5);
    add_tool_slot(SLT_ABDOMEN + 1 ,  "stings",     0);
    
    add_prop(LIVE_NEVER_UNCONSCIOUS, 1);
    add_prop(LIVE_NEVER_BLEED, 1);
    add_prop(LIVE_SKINNED, -1);
    add_prop(LIVE_BUTCHERED, -1);
    add_prop(LIVE_NO_CRITICAL, 1);
}

static nomask int
make_corpse()
{
    remove_object();
    return 1;
}

nomask int *
query_attack_data(int slot)
{
    switch( slot ) 
    {
      case (SLT_BACK+1):
	  return ({ SK_ATTACK_STINGER, ATMOD_STINGER, OBMOD_STINGER });
    }
    return 0;
}


nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_CHEST+1):
	return ({ A_SKIN, 100 });
    }
    return 0;
}
