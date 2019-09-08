inherit	"heap";

#include <event.h>
#include <language.h>
#include <money.h>
#include <macros.h>
#include <properties.h>

private int coin_type; 

static void
create_coins()
{
    set_long("It is " + LANG_ADDART(short()) + ", it looks like money.\n");
    set_plong("Some " + plural_short() + ", they look like money.\n");
}

static nomask void
create_heap()
{
    set_name("coin");
    add_name("money");
    set_pname("money");
    set_short("coin");
    set_pshort("coins");
    set_heap_id(MASTER);
    set_heap_size(1);
    create_coins();
    add_reload("reload_coins", "init_coins");
    add_event(E_HEAP_MERGE, "coin_merge", this_object());
}

/*
 * Function name: set_coin_type
 * Description:	  sets the type to 't'
 * Arguments:     t - The coin type, define in money.h
 */
nomask void
set_coin_type(int t)
{
    int tmp;
    string *coin_names, *coin_s_names;

    if (t > sizeof(COIN_VALUES))
	return;

    coin_names = COIN_NAMES;
    coin_s_names = COIN_SHORT_NAMES;
    if (coin_type)
    {
	tmp = coin_type - 1;
	remove_name(coin_names[tmp]);
	remove_name(coin_s_names[tmp]);
	remove_pname(coin_names[tmp] + "s");
	remove_pname(coin_s_names[tmp] + "s");
    }
        
    coin_type = t;
    tmp = coin_type - 1;
    set_lwvs(0, COIN_WEIGHT[tmp], COIN_VOLUME[tmp], COIN_SIZE[tmp]);
    set_value(COIN_VALUES[tmp]);
    set_heap_id("_coin_" + tmp);
    set_name(coin_s_names[tmp]);
    set_name(coin_names[tmp]);
    set_short(coin_names[tmp]);
    set_pshort(coin_names[tmp] + "s");
}

/*
 * Function name: query_coin_type
 * Description:	  find out what type of coins this are
 * Returns:  	  the type
 */
nomask int
query_coin_type() 
{
    return coin_type; 
}

nomask void
coin_merge(object orig, int num)
{
    if (orig)
	set_coin_type(orig->query_coin_type());
}

nomask mixed *
reload_coins()
{
    return ({ query_heap_size(), coin_type });
}

nomask void
init_coins(mixed *data)
{
    set_heap_size(data[0]);
    set_coin_type(data[1]);
}
