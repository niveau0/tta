/* this is the 'guest' character race file */

inherit "/base/living";

#include <combat.h>
#include <config.h>
#include <event.h>
#include <living.h>
#include <language.h>
#include <macros.h>
#include <mudtime.h>
#include <properties.h>
#include <slots.h>
#include <skills.h>

nomask void
start_player(string name) 
{
    object ob;

    set_name(name); /* name: spirit#clonenumber */
    set_living_name(name);
    init_msg_flags();
    ob = _ROOMD->load_room(DEFAULT_START);
    set_coordinates(ob->query_coordinates());
    move(ob);
    look_room(ob);
    tell_objects(LISTEN(ob) - ({ this_object() }),
		 ({ QCNAME(this_object()), " appears.\n" }),
		 MSG_SEE, this_object());
}

static nomask void
create_living()
{
    int i;
    
    add_name("spirit");
    add_adj("translucent");
    set_long("This is a translucent spirit, a visitor from another world.\n");
    set_race("spirit");
    set_gender(G_NEUTER);
    set_base_hp(7);
    set_hp(query_max_hp());
    set_move_msg("drifts");
    
    add_prop(LIVE_NO_CRITICAL, 1);
    add_prop(LIVE_NEVER_BLEED, 1);
    add_prop(LIVE_NEVER_ILL, 1);
    add_prop(LIVE_NEVER_UNCONSCIOUS, 1);
    add_prop(CODER_NO_CONSUME, 1);
    add_prop(LIVE_NPC_MAX_FATIGUE, 50);
    add_prop(LIVE_USED_LANGUAGE, SK_WESTRON);

    for (i=SK_GRP_LINGUISTICS_FIRST; i<=SK_GRP_LINGUISTICS_LAST; i++)
    {
	set_skill_rank(i, 100);
	set_theoretical_rank(i, 100);
    }
    
    add_lwvs(0, 5000, 10000, 50);

    add_tool_slot(SLT_HEAD+1,    "head",             55 *  6);
    add_tool_slot(SLT_MOUTH+1,   "shining teeth",          1);
    add_tool_slot(SLT_CHEST+1,   "translucent body", 55 * 21);
    add_tool_slot(SLT_ARM+1,     "right arm",        55 *  7);
    add_tool_slot(SLT_ARM+2,     "left arm",         55 *  7);
    add_tool_slot(SLT_HAND+1,    "right hand",       55 * 16);
    add_tool_slot(SLT_HAND+2,    "left hand",        55 * 16);
}

nomask int *
query_attack_data(int slot)
{
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ A_SKIN, 6 });
      case (SLT_CHEST + 1):
	return ({ A_SKIN, 21 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ A_SKIN, 21 });
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	return ({ A_SKIN, 16 });
    }
    return 0;
}

nomask string
short() /* never show the real name with number (guest#123) */
{
    string desc;
    
    desc = "translucent neuter spirit";
    if (query_prop(OBJ_INVIS))
	desc = "(" + desc + ")";
    return desc + (sizeof(query_extra_desc()) ?
		   implode(query_extra_desc(), "") : "");
}

nomask string
plural_short() /* never show the real name with number */
{
    string desc;
    
    desc = "translucent neuter spirits";
    if (query_prop(OBJ_INVIS))
	desc = "(" + desc + ")";
    return desc + (sizeof(query_extra_desc()) ?
		   implode(query_extra_desc(), "") : "");
}

nomask int
do_die(object killer)
{
    return 0;
}

nomask int
quit()
{
    if (::quit())
	return 1;
    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		 ({ QCTNAME(this_object()), " disappears.\n" }),
		 MSG_SEE, this_object());
    remove_object();
}

nomask void
set_linkdead(int i)
{
    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		 ({ QCTNAME(this_object()), " disappears.\n" }),
		 MSG_SEE, this_object());
    remove_object();
}
