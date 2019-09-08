inherit "heap";

#include <properties.h>

static int mtype;

static void
create_missile()
{
    ::create_heap();
    add_reload("reload_missile", "init_missile");
}

static nomask void
create_heap()
{
    create_missile();
}

/*
 * Function name: set_missile_type
 * Description:   Set the kind of missile this is
 * Arguments:     t - missile type, see <weapon.h>
 */
void
set_missile_type(int t)
{
    mtype = t;
}

/*
 * Function name: query_missile_type
 * Description:   what kind of missile is this?
 * Returns:       integer
 */
nomask int
query_missile_type()
{
    return mtype;
}

nomask mixed
reload_missile()
{
    return query_heap_size();
}

nomask void
init_missile(mixed data)
{
    set_heap_size((int)data);
}
