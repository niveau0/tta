/******************************************************************************
 *
 * A common orc, about the size of a hobbit, should only be dangerous in large
 * numbers.
 *
 * Last change: 10.05.97 By: Tulkas
 */


inherit "/base/npc/humanoid";
 
#include <combat.h>
#include <living.h>
#include <race.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>


static nomask void equip();

static void
create_living()
{    
    set_race("orc");
    set_name("orc");
    add_adj("common");
    set_long("Just an ordinary orc.\n");
    set_gender(G_MALE);

    add_lwvs(0, 23000, 25000, HOBBIT_MIN + random(HOBBIT_MAX - HOBBIT_MIN));

    set_tmp_stat(STAT_CO, 40 + random(30));
    set_tmp_stat(STAT_AG, 30 + random(30));
    set_tmp_stat(STAT_SD, 30 + random(20));
    set_tmp_stat(STAT_ME, 40 + random(20));
    set_tmp_stat(STAT_RE, 40 + random(20));
    set_tmp_stat(STAT_ST, 40 + random(30));
    set_tmp_stat(STAT_QU, 40 + random(20));
    set_tmp_stat(STAT_PR, 25 + random(25));
    set_tmp_stat(STAT_EM, 35 + random(20));
    set_tmp_stat(STAT_IN, 30 + random(20));
    set_tmp_stat(STAT_AP, 30 + random(20));
    
    set_base_hp(40 + random(10) * 8);
    set_hp(query_max_hp());

    init_humanoid();
    
    set_skill_rank(SK_PLATE, 2 + random(15));
    set_skill_rank(SK_CHAIN, 2 + random(10));
    set_skill_rank(SK_RIGID_LEATHER, 2 + random(5));
    set_skill_rank(SK_BLACK_SPEECH, 20);
    set_skill_rank(SK_ATTACK_BARE_FIST, 5);

    add_prop(LIVE_NPC_MAX_FATIGUE, 500);

    set_fatigue(query_max_fatigue());

    add_prop(LIVE_USED_LANGUAGE, SK_BLACK_SPEECH);
    add_prop(LIVE_FIGHT_MOOD, 75);

    wear_tool(add_equipment("/lib/armor/r_leather/hobbit_bracer"));
    wear_tool(add_equipment("/lib/armor/plate/hobbit_plate"));
    wear_tool(add_equipment("/lib/armor/s_leather/small_helmet"));

    equip();
}


static nomask void
equip()
{
    int i;
    object to;

    i  = random(2);
    to = this_object();
    
    if (i = 0)
    {        
        set_skill_rank(SK_ATTACK_HAND_AXE, 2 + random(5));

        wield_tool(add_equipment("/lib/weapon/one_h_s/handaxe"));
    }   

    if (i = 1)
    {
        set_skill_rank(SK_ATTACK_SHORTSWORD, 2 + random(5));

        wield_tool(add_equipment("/lib/weapon/one_h_s/shortsword"));
    }
}
