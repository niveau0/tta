/* still under development, Manwe */

inherit "/base/heap";

#include <composite.h>
#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

static int     parsedSize;
static string  parsedType, color;
static mapping cardlist;

nomask void set_cardlist(mapping cl);

static nomask void
create_heap()
{
    set_name("card");
    set_pname("cards");
    set_short("card");
    set_pshort("cards");
    set_long("");
    set_plong("");
    set_heap_id(MASTER);
    add_reload("reload_cards", "init_cards");
    add_event(E_HEAP_MERGE, "card_merge", this_object());
    add_prop(OBJ_MATERIAL, M_PAPER);
    add_lwvs(0, 30, 25, 4);
    cardlist = ([ ]);
}

nomask void
set_color(string c)
{
    remove_adj(color);
    color = c;
    add_adj(color);
    switch (color)
    {
      case "black":
	  set_cardlist(([
	      "orc" : 1,
	      "uruk" : 1,
	      "olog" : 1,
	      "witch" : 1,
	      ]));
	  break;
      case "blue":
	  set_cardlist(([
	      "peasant" : 1,
	      "knight" : 1,
	      "queen" : 1,
	      "king" : 1,
	      ]));
	  break;
      case "green":
	  set_cardlist(([
	      "acolyth" : 1,
	      "adept" : 1,
	      "priest" : 1,
	      "wizard" : 1,
	      ]));
	  break;
      case "red":
	  set_cardlist(([
	      "bird" : 1,
	      "hawk" : 1,
	      "eagle" : 1,
	      "phoenix" : 1,
	      ]));
	  break;
    }
}

nomask string
query_color()
{
    return color;
}
    
nomask void
set_cardlist(mapping cl)
{
    int i, c, *val;
    
    if (!cl)
	return;
    val = map_values(cl);
    for (c=0, i=sizeof(val); i--;)
	c += val[i];
    cardlist = cl;
    set_heap_size(c);
    set_lwvs(query_light(), c * 5, c * 3, 1);
    debug("manwe", cardlist);
}

nomask void
add_cardlist(mapping cl)
{
    int    i;
    string *ind;
    
    debug("manwe", cl);
    if (!cl)
	return;
    ind = map_indices(cl);
    for (i=sizeof(ind); i--;)
	cardlist[ind[i]] += cl[ind[i]];
    set_cardlist(cardlist);
}

nomask mapping
remove_split(int num)
{
    int     i;
    string  *ind;
    mapping splitlist;

    splitlist = ([ ]);
    ind = map_indices(cardlist);
    if (!parsedType)
    {
	for (i=sizeof(ind); i--;)
	{
	    if (cardlist[ind[i]] >= num)
	    {
		splitlist[ind[i]] += num;
		cardlist[ind[i]] -= num;
		return splitlist;
	    }
	    splitlist[ind[i]] += cardlist[ind[i]];
	    num -= cardlist[ind[i]];
	    cardlist[i] = 0;
	}
    }
    else
    {
	if (cardlist[parsedType] < num)
	{
	    splitlist[parsedType] += cardlist[parsedType];
	    cardlist[parsedType] = 0;
	}
	else
	{
	    splitlist[parsedType] += num;
	    cardlist[parsedType] -= num;
	}
    }
    return splitlist;
}

nomask void
card_merge(object orig, int grow)
{
    if (!orig)  /* whole heap moved */
    {
	if (!environment()) /* newly cloned */
	{
	    set_color("black");
	    set_heap_id(MASTER + "black");
	}
    }
    else
    {
	debug("manwe", grow);
	color = orig->query_color();
	set_heap_id(MASTER + color);
	add_cardlist(orig->remove_split(grow));
    }
}

int
query_heap_size()
{
    debug("manwe", parsedSize);
    if (compare_thread() && parsedSize > 0)
	return parsedSize;
    return ::query_heap_size();
}

nomask string
long()
{
    int    i;
    string out, *ind;

    if (!sizeof(ind = map_indices(cardlist)))
	return "An empty card deck.\n";
    if (query_heap_size() < 2)
	return capitalize(LANG_ADDART(ind[i])) + " game card.\n";
    out = "";
    for (i=sizeof(ind); i--;)
	out += cardlist[ind[i]] + " " + ind[i] + "\n";
    return capitalize(color) + " cards:\n" + sprintf("% - *78s", out);
}

nomask mapping
query_cardlist()
{
    return cardlist;
}

varargs int
parse_object(string *cmd, int type, int *co, int d)
{
    int    sz;
    string name;
    
    parsedType = 0;
    parsedSize = 0;
    if (!(sz = sizeof(cmd)))
	return 0;
    name = cmd[sz-1];
    if ((parsedSize = cardlist[name]) < 1)
	return ::parse_object(cmd, type, co, d); /* not a special card name */
    parsedType = name;
    cmd[sz-1] = "card";
    return ::parse_object(cmd, 1, co, d); /* distance checks */
}

nomask mixed
reload_cards()
{
    return cardlist;
}

nomask void
init_cards(mixed data)
{
    set_cardlist(data);
}
