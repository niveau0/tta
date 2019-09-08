inherit "/base/living";

#include <living.h>
#include <slots.h>
#include <skills.h>
#include <combat.h>
#include <properties.h>

static void
create_living()
{
    int i;
    
    set_name("wolf");
    set_race("wolf");
    set_long("A grey wolf.\n");
    set_gender(G_MALE);
    add_adj("grey");
    add_lwvs(0, 6000, 5500, 20);
    
    for (i=STAT_FIRST; i<STAT_LAST; i++)
    {
	set_tmp_stat(i, 10 + random(10));
	set_pot_stat(i, 10 + random(10));
    }
    set_tmp_stat(STAT_AP, 10);
    set_tmp_stat(STAT_QU, 30 + random(30));

    set_skill_rank(SK_ATTACK_BITE, 5);
    set_skill_rank(SK_ADRENAL_DEFENSE, 5);
    set_skill_rank(SK_DISTANCE_RUN, 5);
    //cb_set_unarmed_skill(SK_ATTACK_BITE);
    set_move_msg("walks");
    set_run_msg("chases");
    
    set_base_hp(20);
    set_hp(query_max_hp());

    add_prop(LIVE_NPC_MAX_FATIGUE, 150);
    set_fatigue(query_max_fatigue());

    add_tool_slot(SLT_HEAD+1,    "head",         55 *  6);
    add_tool_slot(SLT_MOUTH+1,   "fangs",        55 *  1);
    add_tool_slot(SLT_CHEST+1,   "body",         55 * 21);
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_MOUTH + 1):
	return ({ SK_ATTACK_TINY_ANIMAL, 114, 159, 250, 290, 315,
		  ({ CRUSH, PUNCTURE }) });
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
    }
    return 0;
}
