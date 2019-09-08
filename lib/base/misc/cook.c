#include <event.h>
#include <analyse.h>
#include <language.h>
#include <liquid.h>
#include <macros.h>
#include <mudtime.h>
#include <properties.h>
#include <recipe.h>

private int modifier;

nomask int
cook_cmd(string str, mixed recipe, object *wood, object tp)
{
    int    i, j, k, sz, min, liquid, *amount;
    object to, *list, *obj;
    mixed  ingredients;

    to = this_object();
    list = tp->visible(I(tp, 1));
    sz = sizeof(ingredients = recipe[RECIPE_INGREDIENTS]);
    amount = ({ });
    obj = ({ });
    
    for (j=0; j<sz; j=j+2)
    {
	min = ingredients[j+1];
	
	for(i=sizeof(list); i--;)
	{
	    if (!list[i]->query_ingredient(ingredients[j]) ||
		/* not analysed herbs and seeds cannot be used */
		(list[i]->query_prop(IS_HERB) &&       
		 (list[i]->query_grow_phase() <= 1 || 
		  list[i]->query_analysed(tp) != ITEM_ANALYSED)))
		continue;

	    if (!(k = list[i]->query_heap_size()))
		k = 1;
	    obj += ({ list[i] });
	    if (k >= min)
	    {
		amount += ({ min });
		min = 0;
		break; /* found enough of current ingredient */
	    }
	    else if (k < min)
	    {
		amount += ({ k });
		min -= k;
	    }
	}
	if (min > 0)
	{
	    tp->catch_tell("You are missing " + min + " " +
			   (min > 1
			    ? LANG_PSENT(ingredients[j])
			    : ingredients[j]) + ".\n");
	    return -1;
	}
    }

    for (k=0, i=sizeof(wood); i--;)
    {
	if (!wood[i]->light_cmd())
	    tp->catch_msg(({ QCTNAME(wood[i]), " cannot be lit.\n" }), 0, 0);
	else
	    k++;
    }
    if (k == 0)
    {
	tp->catch_msg(({ "You weren't able to start a fire below ", 
			     QTNAME(to), ".\n" }), 0, 0);
	return -1;
    }

    min = recipe[RECIPE_WATER];
    liquid = to->query_internal_liquid();
    
    if (to->query_liquid_type() == LIQ_WATER)
    {
	if (liquid < min)
	    min -= liquid;
	else if (liquid > min)
	{
	    tp->catch_msg(({ "You pour some water out of ",
				 QTNAME(to), ".\n" }), 0, 0);
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " pours some water out of ",
				QNAME(to), ".\n" }), MSG_SEE, tp);
	    to->add_fill(-(liquid - min));
	    liquid = min = 0;
	}
    }
    else if (liquid > 0)
    {
	tp->catch_msg(({ "There is already some different liquid within ",
			     QTNAME(to), ".\n" }), 0, 0);
	return -1;
    }
    
    if (liquid < min) /* search for some water in livings inventory */
    {
	for (i=sizeof(list); i--;)
	{
	    if (list[i]->query_prop(IS_CONT) &&
		list[i]->query_liquid_type() == LIQ_WATER)
	    {
		liquid = list[i]->query_internal_liquid();
		if (liquid >= min)
		{
		    list[i]->add_fill(-min);
		    to->add_fill(min);
		    min = 0;
		    break;
		}
		list[i]->add_fill(-liquid);
		to->add_fill(liquid);
		min -= liquid;
	    }
	}
    }
    if (min > 0)
    {
	tp->catch_tell("You don't have enough water to cook.\n");
	return -1;
    }
    to->set_liquid_type(LIQ_WATER, "clear", "water");
    
    for (j=sizeof(obj); j--;)
    {
	if (obj[j]->query_prop(IS_HEAP))
	    obj[j]->add_heap_size(-amount[j]);
	else
	    remove_object(obj[j]);
    }

    tp->catch_msg(({ "You start a nice fire below ", QTNAME(to),
			 " and put the ingredients into it.\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " starts a nice fire below ", QNAME(to),
			" and puts the ingredients into it.\n" }),
		 MSG_SEE, tp);
    
    to->set_liquid_type(LIQ_BREWING, "bubbling", "bubbling brewing");
    add_event(E_MOVE, "E_cook_move", tp);
    add_event(E_MOVE_INV, "E_inv_changed", to);
    
    call_out("resolve_cook", recipe[RECIPE_TIME] / TIME_SCALE,
	     str, recipe, tp);
    return 1;
}

static nomask void
resolve_cook(string str, mixed recipe, object tp)
{
    remove_event(E_MOVE, "E_cook_move", tp);
    remove_event(E_MOVE_INV, "E_inv_changed", this_object());
    
    this_object()->set_liquid_type(recipe[RECIPE_LIQ_EFFECT],
				   recipe[RECIPE_LOOK], str);
    tp->catch_tell("You think, your " + str + " is ready now.\n");
}

nomask void
E_inv_changed(object ob, object from, object to)
{
    if (!from || !ob || !living(from) || to != this_object())
	return;
    from->catch_tell("Uh, might ruin that brewing, won't you?\n");
    modifier += random(3) + 1;
}

nomask void
E_cook_move(object ob, object from, object to)
{
    if (!ob || !living(ob))
	return;
    if (!distance(this_object()->query_coordinates(), ob->query_coordinates(),
		  ob->query_step()))
	ob->catch_tell("Go back to your brewing to watch your cooking!\n");

    modifier += random(3);
}
