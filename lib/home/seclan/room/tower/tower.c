inherit "/base/room";

#include <event.h>

create_tower_room()
{
    object master;
    debug("seclan", "update reached room");
    
    if (master = find_player("seclan"))
    {
	add_event(E_DEATH, "death_revenge", master);
	master->catch_tell("\nI will watch your lifelight, master.\n\n");
    }
}

nomask void
test_function()
{
    debug("seclan", "in tower.c");
}

void
death_revenge(object who, object killer)
{
    if (living(killer))
	killer->catch_tell("A voice booms in your mind: Not very nice to kill"+
			   "Seclan! ... BE GONE!");
    who->resurrect();
    remove_object(killer);
}
