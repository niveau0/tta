static int barter(string str, string color);
static int build(string str, string color);
static int end_turn(string color);
static int roll(string str, string color);
static int trade(string str, string color);
    
static varargs void
send_string(string str, string color)
{
    int    i;
    string *ind;
    object c;
    
    if (color)
    {
	if (players[color] && players[color][1])
	    players[color][1]->send_string(str);
	return;
    }
    ind = map_indices(players);
    for (i=sizeof(ind); i--;)
	if (c = players[ind[i]][1])
	    c->send_string(str);
}

static void
tell_some(string str, string *ind)
{
    int    i, sz;
    object *value;
    
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	if (!(value = players[ind[i]]))
	    continue;
	if (value[0])
	    value[0]->catch_tell(str + ".\n");
	if (value[1])
	    value[1]->send_string("msg " + str + "\n"); 
    }
}

static void
tell_one(string str, string color)
{
    object *value;

    if (!(value = players[color]))
	return;
    if (value[0])
	value[0]->catch_tell(str + ".\n");
    if (value[1])
	value[1]->send_string("msg " + str + "\n");
}

void
receive(string str)
{
    int    i, sz;
    string color, *ind;
    object ob, *value;
    
    if (!str || object_name(ob = previous_object())[..10] != "/kernel/net")
	return;
    
    ind = map_indices(players);
    for (i=0, sz=sizeof(ind); i<sz; i++)
	if ((value = players[ind[i]]) && value[1] == previous_object())
	    break;
    if (i == sz)
	return;
    color = ind[i];
    if (sscanf(str, "build %s\r\n", str))
    {
	build(str, color);
	return;
    }
    if (sscanf(str, "trade %s\r\n", str))
    {
	trade(str, color);
	return;
    }
    if (sscanf(str, "barter %s\r\n", str))
    {
	barter(str, color);
	return;
    }
    if (sscanf(str, "roll %s\r\n", str))
    {
	roll(str, color);
	return;
    }
    if (sscanf(str, "end %s\r\n", str))
    {
	end_turn(color);
	return;
    }
}
