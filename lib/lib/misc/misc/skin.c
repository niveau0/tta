inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <properties.h>

private string race;

static nomask void
create_object()
{
    set_name("skin");
    set_pname("skins");
    set_short("skin");
    set_pshort("skins");
    set_long("A somehow fresh and bloody skin.\n");
    set_lwvs(0, 4500, 4500, 20);
    set_value(100);
    add_prop(OBJ_MATERIAL, M_FLESH);
    add_reload("reload_skin", "init_skin");
}

void
init_skin(mixed *data)
{
    add_lwvs(0, data[0] - query_weight(),
	     data[1] - query_volume(),
	     data[2] - query_size());
    race = data[3];
    set_short(short() + " of " + race);
    set_pshort(plural_short() + " of " + race);
    set_long(long() + "It is made from " + race + ".\n");
}

mixed *
reload_skin()
{
    return ({ query_weight(), query_volume(), query_size(), race });
}

void
set_race_string(string s)
{
    race = s;
}
