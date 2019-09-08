inherit "/base/light";

#include <material.h>
#include <mudtime.h>
#include <properties.h>

static nomask void
create_light()
{
    set_name("candle");
    add_adj("white");
    set_short("candle");
    set_pshort("candles");
    set_long("A white candle.\n");
    set_duration(18000 / TIME_SCALE); /* 5 hours */
    set_lwvs(0, 150, 100, 5);
    set_light_level(150);
    set_value(1);
    add_prop(OBJ_MATERIAL, M_WAX);
}
