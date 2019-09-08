mapping
query_probabilities()
{
    return probability;
}

mapping
query_resources()
{
    return resources;
}

mapping
query_hex()
{
    return hexpos;
}

static void
put_harbor(int pos, int type)
{
    switch (type)
    {
      case FOREST:
	map = map[..pos] + "H!!" + map[pos+4..];
	break;
      case ORE:
	map = map[..pos] + "H^^" + map[pos+4..];
	break;
      case WOOL:
	map = map[..pos] + "Ho*" + map[pos+4..];
	break;
      case STONE:
	map = map[..pos] + "H==" + map[pos+4..];
	break;
      case WHEAT:
	map = map[..pos] + "H##" + map[pos+4..];
	break;
      default:
	map = map[..pos] + "H" + map[pos+2..];
    }
}

static void
insert_harbor()
{
    int   i, ne, sw, sz, typecount, type, pos, *ind;

    ne = sw = 0;
    typecount = random(DESERT) + 1;
    ind = map_indices(harbor);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	switch (harbor[ind[i]][1])
	{
	  case "ne":
	    if (ne % 2)
		type = DESERT;
	    else
	    {
		type = (typecount % (DESERT - 1)) + 1;
		typecount++;
	    }
	    ne++;
	    pos = ind[i] + 2;
	    break;
	  case "sw":
	    if (sw % 2)
		type = DESERT;
	    else
	    {
		type = (typecount % (DESERT - 1)) + 1;
		typecount++;
	    }
	    sw++;
	    pos = ind[i] - 4;
	    break;
	}
	harbor[ind[i]][2] = type;
	put_harbor(pos, type);
	harbdata += "harbor " + harbor[ind[i]][0] + " " +
		    harbor[ind[i]][1] + " " + resdesc[type] + "\n";
    }
}

static void
store_harbor(int hex, string dir)
{
    harbor[get_pos(hex, dir)] = ({ hex, dir, 0 });
}

static void
insert_hexcount(int num, int xpos, int ypos)
{
    int pos;

    pos = xpos + ypos * strlen(LINE);
    if (num > 9)
	map = map[..pos] + num + map[pos+3..];
    else
	map = map[..pos+1] + num + map[pos+3..];
}

static void
insert_probabilities(int count)
{
    int i, c, p, pos, *nums;

    nums = ({ 2, 6, 4, 10, 3, 8, 5, 9, 11, 6, 3, 10, 5, 12, 8, 11, 4, 9 });
    
    c = random(sizeof(nums));
    for (i=1; i<=hexnum; i++)
    {
	if (resources[i] == DESERT)
	    continue;
	p = nums[c];
	probdata += "prob " + i + " " + p + "\n";
	if (!probability[p])
	    probability[p] = ({ i });
	else
	    probability[p] += ({ i });
	
	pos = hexpos[i] + strlen(LINE) * 2 + 4;
	if (p > 9)
	    map = map[..pos-1] + p + map[pos+2..];
	else
	    map = map[..pos] + p + map[pos+2..];
	c++;
	if (c >= sizeof(nums))
	    c = 0;
    }
}

static void
insert_resources()
{
    int i, rnd, pos, *allres;

    allres = allocate(DESERT + 1);
    allres[DESERT] = hexnum / 17;
    
    rnd = hexnum / 5;
    for (i=FOREST; i<DESERT; i++)
	allres[i] = rnd;
    
    rnd = hexnum % 5 - allres[DESERT];
    if (rnd > 0)
	for (i=FOREST; i<FOREST+rnd; i++)
	    allres[i]++;
    
    for (i=1; i<=hexnum; i++)
    {
	rnd = random(DESERT) + 1;
	while (allres[rnd] < 1)
	{
	    rnd++;
	    if (rnd > DESERT)
		rnd = FOREST;
	}
	allres[rnd]--;
	resources[i] = rnd;
	pos = hexpos[i];
	pos += strlen(LINE) + 3;
	resdata += "res " + i + " ";
	switch(rnd)
	{
	  case FOREST:
	    map = map[..pos] + "!!" + map[pos+3..];
	    break;
	  case ORE:
	    map = map[..pos] + "^^" + map[pos+3..];
	    break;
	  case WOOL:
	    map = map[..pos] + "o*" + map[pos+3..];
	    break;
	  case STONE:
	    map = map[..pos] + "==" + map[pos+3..];
	    break;
	  case WHEAT:
	    map = map[..pos] + "##" + map[pos+3..];
	    break;
	  case DESERT:
	    if (!thief)	thief = i;
	    map = map[..pos] + ".." + map[pos+3..];
	    break;
	}
	resdata += resdesc[rnd] + "\n";
    }
}

static void
insert_hex(int xpos, int ypos, int hex)
{
    int    i, sz, pos, len;
    string hexdef;

    hexdef = HEX;
    len = strlen(LINE);
    pos = xpos + ypos * len;
    hexpos[hex] = pos;
    for (i=0; i<YHEX; i++)
    {
	map = map[..pos] + hexdef[(i*XHEX)..((i+1)*XHEX-1)] +
	      map[(pos+XHEX+1)..];
	pos += len;
    }
}

static void
create_map(int hex, int count)
{
    int i, xmid, xpos, ypos, xwidth, ywidth, width, xstep, ystep;
    
    xwidth = strlen(LINE);
    ywidth = (count * 2 + 1) * YHEX + 2;
    for (i=count*2, map=""; i<ywidth; i++)
	map += LINE;
    xmid = (xwidth - XHEX) / 2;
    xpos = xmid;
    ypos = (ywidth - YHEX) / 2 - count * YHEX;

    width = 0;
    xstep = XHEX * 3 / 4;
    ystep = YHEX / 2;

    i = 1;
    while (i <= hex)
    {
	insert_hex(xpos, ypos, i);
	
	if (width >= count && ((xpos == xmid - xstep * count) ||
			       (i <= hex - count && i >= hex - 2 * count)))
	    store_harbor(i, "sw");
	
	i++;
	xpos += xstep;
	if (xpos > xmid + xstep * width)
	{
	    insert_hexcount(i-1, xpos + 6, ypos);
	    store_harbor(i-1, "ne");
	    
	    if (width < count)
		width++;
	    else 
		ypos += ystep;
	    xpos = xmid - xstep * width;
	    if (i > hex)
		break;
	    insert_hexcount(i, xpos - 6, ypos + 2);
	}
	if (xpos <= xmid)
	    ypos += ystep;
	else 
	    ypos -= ystep;
    }
    insert_resources();
    insert_probabilities(count);
    insert_harbor();
}

static void
insert_dev_card(int rnd, int type)
{
    while (devcards[rnd] != KNIGHT)
    {
	if (++rnd > 99)
	    rnd = 0;
    }
    devcards[rnd] = type;
}

static void
create_dev_cards()
{
    int i;
    
    devcards = allocate(100);

    for (i=0; i<100; i++)
	devcards[i] = KNIGHT;
    for (i=5; i--;)
	insert_dev_card(random(100), MONOPOL);
    for (i=3; i--;)
	insert_dev_card(random(100), ROADBUILD);
    for (i=4; i--;)
	insert_dev_card(random(100), INVENTION);
}

/*
 * Function name: create_game
 * Description:   prepare a game
 * Arguments:     hex - how many hex
 *                num - how many players
 */
void
create_game(int hex, int num)
{
    int count, tmp;

    if (hexnum > 1)
	error("There is already a game running.");
    if (hex < 1)
	error("Bad size for hex field");
    if (num < 1 || num > sizeof(colors))
	error("Bad player number");    
    tmp = 1;
    count = 0;
    while (hex > tmp)
    {
	count++;
	tmp += 6 * count;
    }
    if (hex != tmp)
	error("Bad size for hex field");
    hexnum = hex;
    maxplayers = num;
    create_map(hex, count);
    create_dev_cards();
}
