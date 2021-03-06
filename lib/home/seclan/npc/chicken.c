inherit "/base/living";

#include <combat.h>
#include <language.h>
#include <living.h>
#include <macros.h>
#include <mudtime.h>
#include <skills.h>
#include <slots.h>
#include <stdproperties.h>

/* NOTICE: About every MUDday the chicken will drop an egg upon
 * the usage of E_move_inv().
 */

#define EGG_PATH "/home/seclan/food/chicken_egg"

static int time_stamp;

static void
create_living()
{
    int i;
    
    set_name("chicken");
    set_race("chicken");
    set_long("A dumb chicken.\n");
    set_gender(G_NEUTER);
    add_adj("dumb");
    add_lwvs(0, 2000, 2000, 15);

    for (i=STAT_FIRST; i<STAT_LAST; i++)
    {
	set_tmp_stat(i, 10 + random(10));
	set_pot_stat(i, 10 + random(10));
    }
    set_tmp_stat(STAT_AP, 20 + random(30));

    set_skill_rank(SK_ATTACK_TINY_ANIMAL, 5);
    cb_set_unarmed_skill(SK_ATTACK_TINY_ANIMAL);
    set_move_msg("walks");
    set_run_msg("flutters");
    
    set_base_hp(3+random(4));
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 50);
    set_fatigue(query_max_fatigue());
    
    add_tool_slot(SLT_HEAD+1,    "head",          12);
    add_tool_slot(SLT_MOUTH+1,   "bill",           2);
    add_tool_slot(SLT_CHEST+1,   "chest",         22);
    add_tool_slot(SLT_ARM+1,     "right wing",    10);
    add_tool_slot(SLT_ARM+2,     "left wing",     10);
    add_tool_slot(SLT_ABDOMEN+1, "abdomen",       14);
    add_tool_slot(SLT_LEG+1,     "right leg",     15);
    add_tool_slot(SLT_LEG+2,     "left leg",      15);

    time_stamp = time();
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_MOUTH + 1):
	return ({ SK_ATTACK_TINY_ANIMAL, 114, 159, 250, 290, 315,
		  ({ PUNCTURE }) });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ ATT_SKIN, 0, 60, 12});
      case (SLT_CHEST + 1):
	return ({ ATT_SKIN, 0, 60, 22 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ ATT_SKIN, 0, 60, 10 });
      case (SLT_ABDOMEN + 1):
	return ({ ATT_SKIN, 0, 60, 14 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ ATT_SKIN, 0, 60, 15 });
    }
    return 0;
}

void
E_move_inv(object ob, object from, object to)
{
    ::E_move_inv(ob, from, to);

    if(!living(this_object()))
        return;

    /* every day one egg */
    if ((time() - time_stamp) > 24 * 3600 / TIME_SCALE)
    {
	/* Faulty, if E(chicken) is not a room! */
	object egg;

	time_stamp = time();
	egg = E(this_object())->clone_here(EGG_PATH);
	egg->set_coordinates(query_coordinates());
	tell_objects(E(this_object())->query_listen() - ({ this_object() }),
		     ({ QCTNAME(this_object()), " cackles and drops ",
			    QNAME(egg), ".\n" }), MSG_SEE, this_object());
    }
}
