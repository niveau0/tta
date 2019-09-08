inherit "/base/light";

#include <material.h>
#include <mudtime.h>
#include <properties.h>

static nomask void
create_light()
{
    set_name("torch");
    set_pname("torches");
    set_short("torch");
    set_pshort("torches");
    set_long("A simple wooden torch.\n");
    set_lwvs(0, 453, 129, 12);
    set_value(1);
    set_light_level(315);
    set_duration(7200 / TIME_SCALE);
    add_prop(OBJ_MATERIAL, M_OAK);
}
