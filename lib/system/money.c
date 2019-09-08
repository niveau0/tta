#include <money.h>
#include <slots.h>

/******************************************************************************
 * Function name: make_coins
 * Description:   Makes a certain number of coins of a certain type
 * Argument:      num - Number of coins
 *                type - the coin type
 *                to - where to move the money
 * Returns:       Objectpointer to the coins object or 0.
 */
object
make_coins(int num, int type, object to)
{
    object cn;

    if (!to || num < 1 || type > sizeof(COIN_VALUES))
	return 0;
  
    cn = clone_object("/base/coins");
    cn->set_heap_size(num);
    cn->set_coin_type(type);
    cn->move(to);
    
    return cn;
}

/*
 * Function name: total
 * Description:   calculates the total amount of money on a living
 *                (in copperbits)
 * Arguments:     who - calculates whose money?              
 */
int
total(object who)
{
    int    value, i;
    object *list;

    list = who->inventory(6);

    for (value=0, i=sizeof(list); i--;)
    {
	if (list[i]->query_coin_type() > 0)
	    value += list[i]->query_value();
    }
    return value;
}

/*
 * Function name: change
 * Description:   takes or gives money
 * Arguments:     n - how much to change
 *                l - on which living
 * Returns:       1 if successful
 */
int
change(int n, object l)
{
    int    i, sz, val, heap, div;
    object *list;
    
    if (!l || !n)
	return 0;

    if (n > 0)
    {
	int    hand, *hands, *values;
	object coin;
	
	values = COIN_VALUES;
	
	if (sizeof(hands = l->query_slot_type(SLT_HAND)))
	    hand = hands[0];
	
	for (i=1; i<7; i++)
	{
	    if (n >= values[i])
	    {
		coin = make_coins((heap = n / values[i]), i+1, l);
		if (hand)
		    l->add_tool(coin, hand);
		n -= heap * values[i];
		if (!n)
		    return 1;
	    }
	}
	coin = make_coins(n / values[7], COPPER_BIT, l);
	if (hand)
	    l->add_tool(coin, hand);
	return 1;
    }
    
    list = l->inventory(6);
    n = -n;
    for (i=0, sz=sizeof(list); i<sz; i++)
    {
	if (list[i]->query_coin_type() > 0)
	{
	    heap = list[i]->query_heap_size();
	    val = list[i]->query_unit_value();
	    if ((div = n / val) >= heap)
	    {
		remove_object(list[i]);
		n -= val * heap;
		if (!n) return 1;
	    }
	    else
	    {
		if (div * val == n)
		{
		    list[i]->set_heap_size(heap - div);
		    return 1;
		}
		heap -= (div + 1);
		if (!heap)
		    remove_object(list[i]);
		else
		    list[i]->set_heap_size(heap);
		change((div + 1) * val - n, l);
		return 1;
	    }
	}
    }
    return 0;
}

/*
 * Function name: money_text
 * Description:   translates amount into money description string
 * Arguments:     n - how much money 
 * Returns:       string
 */
string
money_text(int n)
{
    int    i, d, *values;
    string *str, *names;
    
    if (n < 1)
	return "nothing";

    values = COIN_VALUES;
    names  = COIN_NAMES;
    
    str = allocate(7);
    for (i=1; i<8; i++)
    {
	if (n >= values[i])
	{
	    n -= (d = n / values[i]) * values[i];
	    str[i-1] = d + " " + names[i] + (d > 1 ? "s" : "");
	    if (!n) break;
	}
    }
    str -= ({ 0 });
    if ((i = sizeof(str)) == 1)
	return str[0];
    
    return implode(str[..i-2], ", ") + " and " + str[i-1];
}

/*
 * Function name: short_desc
 * Description:   translates amount into money short description string
 * Arguments:     n - how much money 
 * Returns:       string
 */
string
short_desc(int n)
{
    int    i, d, *values;
    string *str, *names;
    
    if (n < 0)
	n = 0;

    values = COIN_VALUES;
    names  = COIN_SHORT_NAMES;
    
    str = allocate(7);
    for (i=1; i<8; i++)
    {
	if (n >= values[i])
	{
	    n -= (d = n / values[i]) * values[i];
	    str[i-1] = (d + names[i]);
	}
	else
	    str[i-1] = "";
    }
    return sprintf("%_/5s%_/5s%_/5s%_/5s%_/5s%_/5s%_/5s", str...);
}
