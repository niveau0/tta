/* dead player race-file */

inherit "/base/player";

#include <macros.h>
#include <properties.h>
#include <state.h>
#include <config.h>

static void
create_living()
{
    ::create_living(); /* init variables */
    add_prop(OBJ_INVIS, 1);
    add_prop(LIVE_NO_CRITICAL, 1);
    add_prop(LIVE_NEVER_BLEED, 1);
    add_prop(LIVE_NEVER_ILL, 1);
    add_prop(LIVE_NEVER_UNCONSCIOUS, 1);
    add_prop(CODER_NO_CONSUME, 1);
    add_state(GHOST);
    init_msg_flags();
}

nomask void
config_soul(object who)
{
    string n;
    
    if (previous_program() != _SYSTEMD || !(n = who->query_name()))
	error("Illegal call to config_soul");

    restore_object(PLAYER_FILE(n));
    set_race("ghost");
    
    set_living_name(n + ".dead");
    add_adj("pale");
    set_lwvs(1, 1, 1, query_size());
    set_coordinates(who->query_coordinates());
    move(E(who));

    init_saved_props();
}

int
do_die(object killer)
{
    return 0;
}
