/* the basic definition of a skeleton */

inherit "/base/living";
inherit "/base/npc/humanoid";
inherit "/base/npc/undead";

#include <combat.h>
#include <living.h>
#include <race.h>
#include <properties.h>

static nomask void init_skeleton();

static void
create_living()
{
    set_tmp_stat(STAT_CO, 50 + random(40));
    set_tmp_stat(STAT_AG, 40 + random(30));
    set_tmp_stat(STAT_SD, 10 + random(20));
    set_tmp_stat(STAT_ME, 15 + random(30));
    set_tmp_stat(STAT_RE, 15 + random(30));
    set_tmp_stat(STAT_ST, 60 + random(30));
    set_tmp_stat(STAT_QU, 55 + random(20));
    set_tmp_stat(STAT_PR, 45 + random(25));
    set_tmp_stat(STAT_EM, 35 + random(20));
    set_tmp_stat(STAT_IN, 10 + random(20));
    set_tmp_stat(STAT_AP,  1 + random(20));

    add_lwvs(0, 23000, 25000, HUMAN_MIN + random(HUMAN_MAX-HUMAN_MIN));
    set_base_hp(60 + random(15) * 6);
    set_hp(query_max_hp());

    add_prop(LIVE_NPC_MAX_FATIGUE, 500);
    add_prop(LIVE_SKINNED, -1);
    add_prop(LIVE_BUTCHERED, -1);
    set_fatigue(query_max_fatigue());

    add_prop(LIVE_FIGHT_MOOD, 75);
    
    init_skeleton();
}

static nomask void
init_skeleton()
{
    set_name("skeleton");
    set_race("skeleton");
    
    init_undead();
    init_humanoid();
}
