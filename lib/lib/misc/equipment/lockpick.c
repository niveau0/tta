inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <properties.h>
#include <skills.h>

static nomask void
create_object()
{
    set_name("pick");
    add_name("lockpick");
    set_pname("lock picks");
    set_short("lock pick");
    set_pshort("lock picks");
    set_long("You look on an old lockpick which was used a several times.\n");
    set_lwvs(0, 250, 750, 3);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_IRON);
}

nomask int
query_skill_bonus(int skill)
{
    if (skill != SK_PICK_LOCKS)
	return 0;
    return (random(15)+1);
}
