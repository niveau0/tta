inherit "/base/living";

#include <living.h>
#include <slots.h>
#include <skills.h>
#include <combat.h>
#include <properties.h>

static void
create_living()
{
    set_name("hornet");
    add_name("hornet");
    set_race("hornet");
    set_long("This hornet is about 1 meter tall.\n" +
	     "It seems very aggressive.\n");
    set_gender(G_NEUTER);
    add_adj("giant");
    add_lwvs(0, 2000, 50000, 40);

    set_tmp_stat(STAT_CO, 40 + random(10));
    set_tmp_stat(STAT_AG, 40 + random(20));
    set_tmp_stat(STAT_SD,  1 + random(5));
    set_tmp_stat(STAT_ME,  1 + random(5));
    set_tmp_stat(STAT_RE,  1 + random(5));
    set_tmp_stat(STAT_ST, 20 + random(20));
    set_tmp_stat(STAT_QU, 50 + random(20));
    set_tmp_stat(STAT_PR,  1 + random(5));
    set_tmp_stat(STAT_EM,  1 + random(5));
    set_tmp_stat(STAT_IN,  1 + random(5));
    set_tmp_stat(STAT_AP, 30 + random(10));

    set_pot_stat(STAT_CO, 50);
    set_pot_stat(STAT_AG, 60);
    set_pot_stat(STAT_SD,  6);
    set_pot_stat(STAT_ME,  6);
    set_pot_stat(STAT_RE,  6);
    set_pot_stat(STAT_ST, 40);
    set_pot_stat(STAT_QU, 70);
    set_pot_stat(STAT_PR,  6);
    set_pot_stat(STAT_EM,  6);
    set_pot_stat(STAT_IN,  6);
    set_pot_stat(STAT_AP, 40);

    set_skill_rank(SK_ATTACK_STINGER, 50);
    set_skill_rank(SK_ATTACK_BITE, 50);
    set_move_msg("buzzes");
    set_run_msg("buzzes");
    
    set_base_hp(50+random(20));
    set_hp(query_max_hp());
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 10000);
    set_fatigue(query_max_fatigue());

    add_tool_slot(SLT_HEAD + 1,      "head", 300);
    add_tool_slot(SLT_MOUTH + 1,     "mandibles", 20);   
    add_tool_slot(SLT_CHEST + 1,     "thorax", 620);
    add_tool_slot(SLT_EYE + 1,       "right facet-eye", 30);  
    add_tool_slot(SLT_EYE + 2,       "left facet-eye", 30);   
    add_tool_slot(SLT_ARM + 1,       "right wing", 20);        
    add_tool_slot(SLT_ARM + 2,       "left wing", 20);        
    add_tool_slot(SLT_LEG + 1,       "right foreleg", 40);
    add_tool_slot(SLT_LEG + 2,       "left foreleg", 40);
    add_tool_slot(SLT_LEG + 3,       "right middle leg", 40);
    add_tool_slot(SLT_LEG + 4,       "left middle leg", 40);
    add_tool_slot(SLT_LEG + 5,       "right hindleg", 40);
    add_tool_slot(SLT_LEG + 6,       "left hindleg", 40);
    add_tool_slot(SLT_BACK + 1,      "abdomen", 700);
    add_tool_slot(SLT_BACK + 2,      "sting", 20);
    
    add_prop(LIVE_NEVER_UNCONSCIOUS, 1);
    add_prop(LIVE_NEVER_BLEED, 1);
    add_prop(LIVE_NO_CRITICAL, 1);
    add_prop(LIVE_FIGHT_MOOD, 60);

    set_connections(([
                      SLT_HEAD+1:({ SLT_EYE+1, SLT_EYE+2, SLT_MOUTH+1 }),
                      SLT_CHEST+1:({ SLT_HEAD+1,
                                     SLT_LEG+1, SLT_LEG+2, SLT_LEG+3,
				     SLT_LEG+4, SLT_LEG+5, SLT_LEG+6, 
                                     SLT_ARM+1, SLT_ARM+2 }),
                      SLT_BACK+1:({ SLT_CHEST+1 }),
                      SLT_BACK+2:({ SLT_BACK+1 })
                      ]));

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
        case( SLT_BACK +2 ):
           return ({ SK_ATTACK_STINGER, ATMOD_STINGER,
			 OBMOD_STINGER}); 
        case( SLT_MOUTH +1 ):
	    return ({ SK_ATTACK_BITE, ATMOD_BITE,
			  OBMOD_BITE});   
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case ( SLT_HEAD + 1):
	return ({ A_R_LEATHER, 5 });
      case ( SLT_MOUTH + 1):
        return ({ A_R_LEATHER, 3 });
      case ( SLT_EYE + 1):
      case ( SLT_EYE + 2):
   	return ({ A_SKIN, 3 });
      case ( SLT_CHEST + 1 ):
	return ({ A_R_LEATHER, 19 });
      case ( SLT_ARM + 1):
      case ( SLT_ARM + 2):
       	return ({ A_SKIN, 8 });
      case ( SLT_LEG + 1):
      case ( SLT_LEG + 2):
      case ( SLT_LEG + 3):
      case ( SLT_LEG + 4):
      case ( SLT_LEG + 5):
      case ( SLT_LEG + 6):
       	return ({ A_SKIN, 5 });
      case ( SLT_BACK + 1):
       	return ({ A_R_LEATHER, 19 });
      case ( SLT_BACK + 2):
       	return ({ A_R_LEATHER, 2 });
    }
    return 0;
}
