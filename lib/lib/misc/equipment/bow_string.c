inherit "/base/heap";

#include <macros.h>
#include <material.h>
#include <properties.h>

static nomask void
create_heap()
{
    set_name("bow string");
    set_pname("bow strings");
    add_name("string");
    set_short("bow string");
    set_pshort("bow strings");
    set_long("A simple bow string.\n");
    set_plong("Some simple bow strings.\n");
    set_lwvs(0, 30, 25, 1);
    set_value(160);
    set_heap_size(1);
    set_heap_id(MASTER);
    add_prop(OBJ_MATERIAL, M_BONE);
}
