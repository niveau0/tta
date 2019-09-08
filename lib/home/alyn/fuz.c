inherit "/base/living";

#include <combat.h>
#include <living.h>
#include <macros.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>

static nomask void
create_living()
{   
    set_name("fuz");
    add_name("furball");
    set_race("furball");
    set_long_bs("This ball is roughly eight inches in diameter. "+
             "It is covered with brown fluffy fur all over, "+
             "except for the two large blue eyes. \n");
    set_gender(G_NEUTER);
    add_adj("brown");
    add_adj("fluffy");
    add_lwvs(0, 250, 4393, 8);
    add_mana(1);
    set_tmp_stat(STAT_CO, 40);
    set_tmp_stat(STAT_AG, 110);
    set_tmp_stat(STAT_SD, 70);
    set_tmp_stat(STAT_ME, 60);
    set_tmp_stat(STAT_RE, 40);
    set_tmp_stat(STAT_ST, 20);
    set_tmp_stat(STAT_QU, 110);
    set_tmp_stat(STAT_PR, 70);
    set_tmp_stat(STAT_EM, 80);
    set_tmp_stat(STAT_IN, 60);
    set_tmp_stat(STAT_AP, 100);
    

    set_pot_stat(STAT_CO, 40);
    set_pot_stat(STAT_AG, 110);
    set_pot_stat(STAT_SD, 70);
    set_pot_stat(STAT_ME, 60);
    set_pot_stat(STAT_RE, 40);
    set_pot_stat(STAT_ST, 20);
    set_pot_stat(STAT_QU, 110);
    set_pot_stat(STAT_PR, 70);
    set_pot_stat(STAT_EM, 80);
    set_pot_stat(STAT_IN, 60);
    set_pot_stat(STAT_AP, 100);

    set_skill_rank(SK_ADRENAL_DEFENSE, 100);
    set_skill_rank(SK_SINDARIN, 100);
    set_skill_rank(SK_WESTRON, 100);
    cb_set_parry_skill(SK_ADRENAL_DEFENSE);
    set_move_msg("hovers");
    set_run_msg("zaps");

    set_teleport_in("appears with a loud bang and a lot of smoke.");
    set_teleport_out("disappears with a loud bang and a lot of smoke.");
	
    set_base_hp(2);
    set_hp(query_max_hp());

    add_prop(LIVE_NO_AUTOATTACK, 1);
    add_prop(LIVE_NPC_MAX_FATIGUE, 100);
    set_fatigue(query_max_fatigue());
    
    add_tool_slot(SLT_EYE+1,     "right eye",           1);
    add_tool_slot(SLT_EYE+2,     "left eye",            1);
    add_tool_slot(SLT_CHEST+1,   "body",               20);
    set_connections(([
                      SLT_CHEST+1:({ SLT_EYE+1, SLT_EYE+1 }) ]));
    add_prop(LIVE_FIGHT_MOOD, 0);
    if (IS_CLONE)
	call_out("behaviour",0,this_object());
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_EYE + 1):
      case (SLT_EYE + 2):
	return ({ A_SKIN, 2 });
      case (SLT_CHEST + 1):
	return ({ A_S_LEATHER, 17 });
    }
    return 0;
}

static void
behaviour(object fuz)
{
    switch (random(5))
    {
      case (0):
	  command("emotesee rotates around its axis.");
	  break;
      default:
	  command("emotesee blinks with its large eyes.");
    }
	call_out("behaviour",15+random(10),this_object());
    
}

varargs void
catch_tell(string msg, int no_prompt)
{
    string message, order, which_order, fuzname, name, playername;
    object player,alyn;

    sscanf(msg, "%s %*s", name);
    if (name=="Alyn")
    {
        sscanf(msg, "%*s: %s %*s\n", fuzname);
	if (fuzname=="fuz")
	{
	    sscanf(msg, "%*s: fuz %s\n", order);
            sscanf(order, "%s %*s", which_order);
	    switch (which_order)
	    {
	      case ("annoy"):
	      {
		  sscanf(order, "annoy %s", playername);
		  player=find_player(playername);
		  move_living(E(player),
			      player->query_coordinates(),
			      "",
			      1);
		  player->catch_tell("You here a voice in your mind " +
				     "that says:\n" +
		                     "YOU ARE SO STUPID!!!\n" +
		                     "Then the voice is gone.\n", 1);    
		  alyn=find_player("alyn");
		  move_living(E(alyn),
			      alyn->query_coordinates(),
			      "",
			      1);   
		  break;
	      }
	      case ("tell"):
	      {
		  sscanf(order, "tell %s %*s", playername);
		  sscanf(order, "tell " + playername + "%s", message);
		  player=find_player(playername);
		  if (player==0)
		  {
		      alyn=find_player("alyn");
		      alyn->catch_tell("You here the voice of Fuz in your mind:\n"+
				       "Sorry, I can't find " +
				       playername +
				       " at the moment.\n",1);
		  }
		  else
		  {
		      move_living(E(player),
				  player->query_coordinates(),
				  "",
				  1);
		      player->catch_tell("You here a voice in your mind " +
					 "that says:\n" +
					 "I have this message for you:\n" +
					 message +
					 "\n", 1);
		      alyn=find_player("alyn");
		      move_living(E(alyn),
				  alyn->query_coordinates(),
				  "",
				  1);
		  }
		  break;
	      }
	      default:
	      {
	      }
	    }
	}
    }
    ::catch_tell(msg, no_prompt);
}
