inherit "/base/food";

#include <event.h>
#include <material.h>
#include <macros.h>
#include <properties.h>

static int apple_color;

static nomask void
set_apple_color(int color)
{
    apple_color = color;
    if (color) 
    {
        add_adj("red");
        remove_adj("green");
        set_short("red apple");
        set_pshort("red apples");
    }  
    else 
    {
        add_adj("green");
        remove_adj("red");
        set_short("green apple");
        set_pshort("green apples");
    }
}

static void
create_food()
{
    set_name("apple");
    set_apple_color(random(2));
    set_long("This seems to be a delicious apple.\n");
    set_plong("These are delicious looking apples.\n");
    set_heap_id(MASTER);
    set_amount(75);  /* food value */
    add_lwvs(0, 100, 300, 4);
    set_value(5);
    add_prop(OBJ_MATERIAL, M_FOOD);
    add_event(E_HEAP_MERGE, "apple_merge", this_object());
}

string
query_heap_id()
{
     return "apple" + apple_color + ::query_heap_id();
}

nomask int
query_apple_color()
{
    return apple_color;
}

nomask void
apple_merge(object orig, int num)
{
    if (orig)
	set_apple_color(orig->query_apple_color());
}
