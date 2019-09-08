inherit "/base/container";

#include <liquid.h>
#include <material.h>
#include <properties.h>
#include <mudtime.h>

#define BASINSIZE 200000
#define FILLRATE 300 /* ml per (MUD)minute */

static int timestamp;

static void
create_container()
{
    set_name("spring");
    add_adj("clear");
    add_adj("cool");
    set_short("spring of water");
    set_pshort("springs of water");
    set_long_bs("The clear and cool water pours forth from a little hole in "+
	     "the side of a colossus of grey rock. It flows down and "+
	     "fills a big basin that has been build around the block's "+
	     "bottom.\n");
    set_max_weight(1000000 + BASINSIZE);
    set_max_volume(5000000 + BASINSIZE);
    set_lwvs(0, 1000000, 5000000, 90);
    set_liquid_type(LIQ_WATER, "clear", "water");
    set_fill(BASINSIZE);
    add_prop(OBJ_MATERIAL, M_STONE);
    add_prop(CONT_RIGID, 1);
    add_prop(OBJ_NO_GET, 1);
    set_value(2000);

    timestamp = time();
}

/* Refills the basin according to the filling rate.
 */
void
refill()
{
    int temp;
    if ( (temp=(time()-timestamp)*TIME_SCALE * FILLRATE/60) > 0)
	add_fill(temp);
    timestamp = time();
}

/*
nomask int           THIS FUNCTION IS NOMASK! :(
drink_cmd()
{
    refill();
    return ::drink_cmd();
}
*/

int
fill_cmd(object spender)
{
    refill();
    return ::fill_cmd(spender);
}

string
short()
{
    refill();
    return ::short();
}

string
long()
{
    refill();
    return ::long();
 
}
