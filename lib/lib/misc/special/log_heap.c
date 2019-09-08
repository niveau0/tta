inherit "/base/heap";

#include <macros.h>
#include <material.h>
#include <properties.h>

static void
create_heap()
{
    set_name("log");
    add_adj("wooden");
    add_adj("brown");
    set_pname("logs");
    set_short("wooden log");
    set_pshort("wooden logs");
    set_long("A log of hard brown wood.\n");
    set_plong("A cluster of wooden logs.\n");
    set_lwvs(0, 1500, 2500, 12);
    set_value(5);
    add_prop(OBJ_MATERIAL, M_OAK);
    set_heap_id(MASTER);
    set_heap_size(200);
}
