static nomask void
do_item(string str)
{
    int id;
    mapping tmp;
    
    if (!str || (!sscanf(str, "%d", id) && str != "new"))
    {
	if(str != "new" && id < 1)
	{
	tp->catch_tell("Wrong value, cancelled.\n");
	return;
	}
    }
    if (str == "new")
    {
	tmp = query_item(-1);
	id = sizeof(map_indices(tmp))+1;
	while (tmp[id])
	    id++;
	tp->catch_tell("Creating new item #"+id+"\n",1);
    }
    
    tp->catch_tell("Item names [string,string,...] or " +
		   "'remove': ", 1);
    input_to("item_names", 0, id);
}

static nomask void
item_names(string str, int id)
{
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    if (str == "remove")
    {
	remove_item(id);
	tp->catch_tell("Tried to remove item " + id + ".\n");
	return;
    }
    tp->catch_tell("Item plural names [string,string,...]: ", 1);
    input_to("item_pnames", 0, id, explode(str, ","));
}

static nomask void
item_pnames(string str, int id, string *n)
{
    int i;
    
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    /* removing leading spaces from names */
    for ( i = sizeof(n)-1; i--;)
	while (n[i][0] == ' ')
	    n[i] = n[i][1..strlen(n[i])-1];
    
    tp->catch_tell("Item description [string]: ", 1);
    input_to("item_desc", 0, id, n, explode(str, ","));
}

static nomask void
item_desc(string str, int id, string *n, string *pn)
{
    int i;
    
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    /* removing leading spaces from plural names */
    for (i = sizeof(pn)-1; i--;)
	while (pn[i][0] == ' ')
	    pn[i] = pn[i][1..strlen(pn[i])-1];

    tp->catch_tell("Item coordinates or no entry [int,int,int]: ", 1);
    input_to("item_coord", 0, id, n, pn, str);
}

static nomask void
item_coord(string str, int id, string *n, string *pn, string desc)
{
    int x, y, z;
    
    if (!str || !strlen(str) || sscanf(str, "%d,%d,%d", x, y, z) != 3)
    {
	tp->catch_tell("Ok, assuming no coordinates.\n");
	add_item(n, pn, (desc[0] == '@' ? desc : BS(desc, 77, 0)), id);
    }
    else
	add_item(n, pn, (desc[0] == '@' ? str : BS(desc, 77, 0)), id,
		 ({ x, y, z }));
    tp->catch_tell("Item values set.\n");
}
