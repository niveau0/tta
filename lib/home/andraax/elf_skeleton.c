inherit "/lib/npc/undead/skeleton";
 
#include <living.h>
#include <race.h>
#include <properties.h>
#include <skills.h>

static void
create_living()
{
    add_adj("elven");
    set_long("A tall and slender skeleton stands here.\n");

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
    
    set_skill_rank(SK_ATTACK_DAGGER, 2 + random(5));
    set_skill_rank(SK_ATTACK_SHORTSWORD, 4 + random(5));
    set_skill_rank(SK_PLATE, 2 + random(10));
    set_skill_rank(SK_CHAIN, 8 + random(20));
    set_skill_rank(SK_RIGID_LEATHER, 4 + random(5));
    set_skill_rank(SK_SOFT_LEATHER, 4 + random(5));
    set_skill_rank(SK_TRADING, random(25));
    set_skill_rank(SK_WESTRON, 20);
    set_skill_rank(SK_SINDARIN, 15 + random(5));
    set_skill_rank(SK_ATTACK_BARE_FIST, 15);

    add_lwvs(0, 23000, 25000, ELF_MIN + random(ELF_MAX-ELF_MIN));
    
    init_skeleton(); /* call standard skeleton definition */
    
    wear_tool(add_equipment("/home/andraax/medium_plate_helmet"));
    wear_tool(add_equipment("/lib/armor/s_leather/elven_leg_greaves"));
    wear_tool(add_equipment("/lib/armor/chain/elven_chain_shirt"));
    if (random(2))
	wield_tool(add_equipment("/lib/weapon/one_h_s/dagger"));
    else
	wield_tool(add_equipment("/lib/weapon/one_h_s/shortsword"));
}
