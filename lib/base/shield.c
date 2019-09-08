inherit "weapon";

#include <properties.h>

static void
create_shield()
{
    ::create_weapon();
}

static nomask void
create_weapon()
{
    add_prop(IS_SHIELD, 1);
    create_shield();
}

/*
 * Function name: set_wielder
 * Description:   Set the wielder of this object
 * Arguments:     The wielder object
 */
void
set_wielder(object w)
{
    if (w && !wielder)
	w->cb_add_shield(this_object());
    else if (!w && wielder)
	wielder->cb_remove_shield(this_object());
    ::set_wielder(w);
}
