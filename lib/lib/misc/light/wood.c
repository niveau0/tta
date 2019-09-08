inherit "/base/light";

#include <material.h>
#include <mudtime.h>
#include <properties.h>

static nomask void
create_light()
{
    int rnd;
    
    set_name("wood");
    set_short("wood");
    set_pshort("pieces of wood");
    set_long("Just simple wood, good to use for a fire.\n");
    rnd = random(200);
    set_lwvs(0, 400 + rnd, 100 + rnd/5, 10 + rnd/20);
    set_value(1);
    set_light_level(200);
    set_duration(3600 / TIME_SCALE);
    add_prop(OBJ_MATERIAL, M_OAK);
    add_prop(OBJ_ATTACH_SIZE, 1);
}
