/* get the start resopurces for the player */
static void
start_resources(int hex, string dir, string color)
{
    int i, xstep, ystep, res, *h, *v;

    h = allocate(3);
    v = map_values(hexpos);
    h[0] = hex;
    hex = hexpos[hex];
    
    xstep = XHEX * 3 / 4;
    ystep = YHEX / 2 * strlen(LINE);
    
    switch (dir)
    {
      case "e":
	h[1] = hex + xstep - ystep;
	h[2] = hex + xstep + ystep;
	break;
      case "w":
	h[1] = hex - xstep - ystep;
	h[2] = hex - xstep + ystep;
	break;
      case "nw":
	h[1] = hex - xstep - ystep;
	h[2] = hex - (YHEX - 1) * strlen(LINE);
	break;
      case "ne":
	h[1] = hex + xstep - ystep;
	h[2] = hex - (YHEX - 1) * strlen(LINE);
	break;
      case "sw":
	h[1] = hex - xstep + ystep;
	h[2] = hex + (YHEX - 1) * strlen(LINE);
	break;
      case "se":
	h[1] = hex + xstep + ystep;
	h[2] = hex + (YHEX - 1) * strlen(LINE);
	break;
    }

    h[1] = member_array(h[1], v) + 1;
    h[2] = member_array(h[2], v) + 1;
    
    for (i=0; i<3; i++)
    {
	if (!(res = resources[h[i]]) || res == DESERT)
	    continue;
	if (!cards[color])
	    cards[color] = ([ res : 1 ]);
	else
	    cards[color][res]++;
	tell_one("You got 1 " + resdesc[res], color);
	send_string("card " + cards[color][res] + " " +
		    resdesc[res] + "\n", color);
    }
}

static void
next_player(int i)
{
    int index;
    
    index = member_array(current, colors);
    if (i > 0)
    {
	index++;
	if (index >= maxplayers)
	    index = 0;
    }
    else
    {
	index--;
	if (index < 0)
	    index = maxplayers - 1;
    }
    current = colors[index];
    tell_some("It's now " + current + "'s turn", map_indices(players));
    trader = 0;
}

static int
check(string color, int ph, string phasename)
{
    if (!phase)
    {
	tell_one("The game has not been started yet", color);
	return 0;
    }
    if (current != color)
    {
	tell_one("It's " + current + "'s turn", color);
	return 0;
    }
    if (ph && phase != ph)
    {
	tell_one("It's not the " + phasename + " phase", color);
	return 0;
    }
    return 1;
}

/*****************************************************************************/
  
static int
barter(string arg, string color)
{
    int    i, sz, num, index, indn, *harb;
    string res, resnew, dest;
    
    if (!trader)
    {
	tell_one("You need to choose someone to trade with", color);
	return -1;
    }
    if (trader == "bank") /* trade with the game bank */
    {
	if (!check(color, BUILD_PHASE, "build and barter"))
	    return -1;
	if (!arg || sscanf(arg, "%s for %s", res, resnew) != 2 ||
	    (index = member_array(res, resdesc)) >= DESERT ||
	    index < FOREST ||
	    (indn = member_array(resnew, resdesc)) >= DESERT ||
	    indn < FOREST || res == resnew)
	{
	    tell_one("Use: barter <resource> for <resource>", color);
	    return -1;
	}
	num = 4;
	if (sz = sizeof(harb = harbor[color]))
	{
	    for (i=0; i<sz; i++)
		if (harbor[harb[i]][2] == index)
		{
		    num = 2;
		    break;
		}
		else if (harbor[harb[i]][2] == DESERT)
		    num = 3;
	}
	if (!cards[color] || cards[color][index] < num)
	{
	    tell_one("You don't have so many " + res + " cards", color);
	    return -1;
	}
	cards[color][index] -= num;
	send_string("card " + cards[color][index] + " " + res + "\n", color);
	cards[color][indn]++;
	send_string("card " + cards[color][indn] + " " + resnew + "\n", color);
	tell_some(capitalize(color) + " got one " + resnew + " from the bank",
		  map_indices(players));
	return 1;
    }
    /* trade with a player */
    if (color == current)
	dest = trader;
    else if (color == trader)
	dest = current;
    else
    {
	tell_one("Noone is trading with you", color);
	return -1;
    }
    if (!check(current, BUILD_PHASE, "build")) /* check only phase */
	return -1;
    if (!arg || sscanf(arg, "%d %s", num, res) != 2 ||
	(index = member_array(res, resdesc)) >= DESERT || index < FOREST)
    {
	tell_one("Use: barter <amount> <resource>", color);
	return -1;
    }
    if (!cards[color] || cards[color][index] < num)
    {
	tell_one("You don't have so many " + res + " cards", color);
	return -1;
    }
    cards[color][index] -= num;
    send_string("card " + cards[color][index] + " " + res + "\n", color);
    
    if (!cards[dest])
	cards[dest] = ([ index : num ]);
    else
	cards[dest][index] += num;
    send_string("card " + cards[dest][index] + " " + res + "\n", dest);

    tell_some(capitalize(color) + " gives " + num + " " + res + " to " +
	      dest, map_indices(players));
    return 1;
}

static int
build(string arg, string color)
{
    int    hex;
    string dir;

    if (!color)
	return 0;
    if (!arg)
    {
	if (players[color][0])
	    players[color][0]->catch_tell
		("Usage: build road|village|town <hexnum> <direction>\n" +
		 "   or  build develop\n");
	return -1;
    }
    if (phase == INIT_PHASE || phase == INIT_PHASE2)
    {
	if (!check(color, phase, "build"))
	    return -1;
    }
    else if (!check(color, BUILD_PHASE, "build"))
	return -1;

    if (arg == "develop")
    {
	if (!get_develop(color))
	{
	    tell_one("Not possible", color);
	    return -1;
	}
	tell_some(capitalize(color) + " got a development card",
		  map_indices(players));
	return 1;
    }
    
    if (sscanf(arg, "village %d %s", hex, dir) == 2)
    {
	if (phase != BUILD_PHASE)
	{
	    if (init[1])
	    {
		tell_one("You already build the village", color);
		return -1;
	    }
	    if (!put_village(hex, dir, color))
	    {
		tell_one("Not possible", color);
		return -1;
	    }
	    init[1] = 1;
	    if (phase == INIT_PHASE2)
		start_resources(hex, dir, color);
	}
	else if (!put_village(hex, dir, color))
	{
	    tell_one("Not possible", color);
	    return -1;
	}
	tell_some(capitalize(color) + " built a village at hex " + hex,
		  map_indices(players));
	return 1;
    }
    if (sscanf(arg, "town %d %s", hex, dir) == 2)
    {
	if (phase != BUILD_PHASE || !put_town(hex, dir, color))
	{
	    tell_one("Not possible", color);
	    return -1;
	}
	tell_some(capitalize(color) + " built a town at hex " + hex,
		  map_indices(players));
	return 1;
    }
    if (sscanf(arg, "road %d %s", hex, dir) == 2)
    {
	if (phase != BUILD_PHASE)
	{
	    if (init[0])
	    {
		tell_one("You already build the road", color);
		return -1;
	    }
	    if (!put_road(hex, dir, color))
	    {
		tell_one("Not possible", color);
		return -1;
	    }
	    init[0] = 1;
	}
	else if (!put_road(hex, dir, color))
	{
	    tell_one("Not possible", color);
	    return -1;
	}
	tell_some(capitalize(color) + " built a road at hex " + hex,
		  map_indices(players));
	return 1;
    }
    tell_one("Not possible", color);
    return -1;
}

static int
cards(object tp)
{
    string  color, str;
    mapping tmp;
    
    if (!(color = playercolor[tp]))
	return 0;
    if (!(tmp = cards[color]))
    {
	tp->catch_tell("You have no cards.\n");
    	return 1;
    }
    str = sprintf("Resource cards\n Forest: %3d      Ore: %3d       " +
		  "Wool: %3d      Stone: %3d  Wheat: %3d\n" +
		  "Development cards\n" +
		  " Knight: %3d  Monopol: %3d  Roadbuild: %3d  " +
		  "Invention: %3d\n", tmp[FOREST], tmp[ORE],
		  tmp[WOOL], tmp[STONE], tmp[WHEAT], tmp[KNIGHT],
		  tmp[MONOPOL], tmp[ROADBUILD], tmp[INVENTION]);
    tp->catch_tell(str);
    return 1;
}

static int
costs(object tp)
{
    tp->catch_tell("\n      Village: 1 forest, 1 stone, 1 wool, 1 wheat\n" +
		   "         Road: 1 forest, 1 stone\n" +
		   "         Town: 3 ore, 2 wheat\n" +
		   "  Development: 1 ore, 1 wool, 1 wheat\n");
    return 1;
}

static int
discard(string arg, string color)
{
    int    num, r;
    string res;

    if (!arg || sscanf(arg, "%d %s", num, res) != 2 ||
	(r = member_array(res, resdesc)) == -1)
    {
	tell_one("Usage: discard <num> <resource>", color);
	return -1;
    }
    if (!check(current, DISCARD_PHASE, "discard"))
	return -1;
    if (!cards[color] || cards[color][DISCARD] < num)
    {
	tell_one("You don't need to discard so much", color);
	return -1;
    }
    if (cards[color][r] < num)
    {
	tell_one("You don't have that much " + res, color);
	return -1;
    }
    cards[color][DISCARD] -= num;
    cards[color][r] -= num;
    discardnum -= num;
    if (discardnum < 1)
	phase = THIEF_PHASE;
    send_string("card " + cards[color][r] + " " + res + "\n", color);
    tell_some(capitalize(color) + " discards " + num + " " + res,
	      map_indices(players));
    return 1;
}

static int
end_turn(string color)
{
    int    i, sz, j, num;
    string *ind;
    
    if (!check(color, 0, ""))
	return -1;

    if (phase == DICE_PHASE || phase == THIEF_PHASE || phase == DISCARD_PHASE)
    {
	tell_one("You cannot end your turn now", color);
	return -1;
    }
    if (phase == INIT_PHASE2)
    {
	if (!init[1])
	{
	    tell_one("You need to build one more village", color);
	    return -1;
	}
	if (!init[0])
	{
	    tell_one("You need to build one more road", color);
	    return -1;
	}
	init[0] = init[1] = 0;
	if (color == colors[0])
	{
	    tell_some("The init phase is over and " + color +
		      " is first", map_indices(players));
	    phase = DICE_PHASE; /* game starts, init finished */
	    return 1;
	}
	next_player(-1);
	return 1;
    }
    if (phase == INIT_PHASE)
    {
	if (!init[1])
	{
	    tell_one("You need to build one more village", color);
	    return -1;
	}
	if (!init[0])
	{
	    tell_one("You need to build one more road", color);
	    return -1;
	}
	init[0] = init[1] = 0;
	if (color == colors[maxplayers - 1])
	{
	    tell_one("Now you can place your second village", color);
	    phase = INIT_PHASE2; /* first init round finished */
	    return 1;
	}
	next_player(1);
	return 1;
    }
    ind = map_indices(players);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	if (cards[ind[i]] && cards[ind[i]][DISCARD] > 0)
	{
	    tell_one(capitalize(ind[i]) + " still needs to discard", color);
	    return -1;
	}
    }
    
    phase = DICE_PHASE;
    develop[0] = 0;
    develop[1] = ([ ]);
    
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	num = 0;
	if (cards[ind[i]])
	{
	    for (j=FOREST; j<DESERT; j++)
		num += cards[ind[i]][j];
	}
	send_string("player " + num + " " + ind[i] + "\n");
    }
    
    next_player(1);
    return 1;
}

static int
help(object tp)
{
    tp->catch_tell("\nCommands:\n" +
		   " map             - show the current play field\n"+
		   " legend          - help about the ascii map symbols\n"+
		   " costs           - shows the building costs\n"+
		   " players         - list all players, * marks the " +
		   "current player\n" +
		   " cards           - take a quick look at your cards\n" +
		   " discard         - discard some resource cards\n" +
		   "When its your turn:\n"+
		   " roll dice       - in the dice phase you have to " +
		   "roll\n" +
		   " build           - pay resources to build "+
		   "something new\n" +
		   " play            - play a development card\n" +
		   " thief           - move the thief after playing a " +
		   "knight or '7' roll\n" +
		   " end turn        - after all your trading and " +
		   "building, please end your turn\n");
    return 1;
}

static int
legend(object tp)
{
    tp->catch_tell("\nLegend: ~~ - Water    !! - Forest     ^^ - Ore"+
		   "     .. - Desert\n"+
		   "        o* - Wool     == - Stone      ## - Wheat"+
		   "   XX - Pack of thieves\n");
    return 1;
}

static int
map(object tp)
{
    tp->catch_tell("\n\t\t\t Island of the settlers!\n" + map);
    return 1;
}

static int
play(string arg, string color)
{
    int    i, sz, count, r, r2;
    string res, res2, *ind;
    
    if (!check(color, 0, ""))
	return -1;
    
    if (!arg)
    {
	if (players[color][0])
	    players[color][0]->catch_tell("Usage: play <cardname> " +
					  "<options>\n");
	return -1;
    }
    if (develop[0])
    {
	tell_one("You may only play one development card each turn", color);
	return -1;
    }
    if (arg == "knight")
    {
	if (phase == DISCARD_PHASE || phase == THIEF_PHASE)
	{
	    tell_one("The thief has not been set", color);
	    return -1;
	}
	if (!cards[color] || cards[color][KNIGHT] < 1)
	{
	    tell_one("You have no knight card", color);
	    return -1;
	}
	if (develop[1][KNIGHT] >= cards[color][KNIGHT])
	{
	    tell_one("You must keep new cards one turn", color);
	    return -1;
	}
	develop[0] = 1;
	tell_some(capitalize(color)  + " plays a knight",
		  map_indices(players));
	cards[color][KNIGHT]--;
	send_string("card " + cards[color][KNIGHT] + " " +
		    resdesc[KNIGHT] + "\n", color);
	if (phase == DICE_PHASE)
	    phase = KNIGHT_PHASE;
	else
	    phase = THIEF_PHASE;
	return 1;
    }
    if (sscanf(arg, "monopol %s", res))
    {
	if (phase != BUILD_PHASE)
	{
	    tell_one("You cannot play monopol in this phase", color);
	    return -1;
	}
	if ((r = member_array(res, resdesc)) == -1)
	{
	    tell_one("There is no resource " + res, color);
	    return -1;
	}
	if (!cards[color] || cards[color][MONOPOL] < 1)
	{
	    tell_one("You have no monopol card", color);
	    return -1;
	}
	if (develop[1][MONOPOL] >= cards[color][MONOPOL])
	{
	    tell_one("You must keep new cards one turn", color);
	    return -1;
	}
	develop[0] = 1;
	tell_some(capitalize(color)  + " plays a monopol card for " + res,
		  (ind = map_indices(players)));
	cards[color][MONOPOL]--;
	send_string("card " + cards[color][MONOPOL] + " " +
		    resdesc[MONOPOL] + "\n", color);
	ind -= ({ color });
	for (i=0, sz=sizeof(ind); i<sz; i++)
	{
	    if (cards[ind[i]])
	    {
		cards[color][r] += cards[ind[i]][r];
		cards[ind[i]][r] = 0;
		send_string("card " + cards[color][r] + " " + res +
			    "\n", color);
		send_string("card 0 " + res + "\n", ind[i]);
	    }
	}
	return 1;
    }
    if (sscanf(arg, "invention %s %s", res, res2))
    {
	if (phase != BUILD_PHASE)
	{
	    tell_one("You cannot play invention in this phase", color);
	    return -1;
	}
	if ((r = member_array(res, resdesc)) == -1)
	{
	    tell_one("There is no resource " + res, color);
	    return -1;
	}
	if ((r2 = member_array(res2, resdesc)) == -1)
	{
	    tell_one("There is no resource " + res2, color);
	    return -1;
	}
	if (!cards[color] || cards[color][INVENTION] < 1)
	{
	    tell_one("You have no invention card", color);
	    return -1;
	}
	if (develop[1][INVENTION] >= cards[color][INVENTION])
	{
	    tell_one("You must keep new cards one turn", color);
	    return -1;
	}
	develop[0] = 1;
	tell_some(capitalize(color)  + " plays an invention card for " + res +
		  " and " + res2, map_indices(players));
	cards[color][INVENTION]--;
	send_string("card " + cards[color][INVENTION] + " " +
		    resdesc[INVENTION] + "\n", color);
	cards[color][r]++;
	cards[color][r2]++;
	send_string("card " + cards[color][r] + " " + res +
		    "\n", color);
	send_string("card " + cards[color][r2] + " " + res2 +
		    "\n", color);
	return 1;
    }
    if (arg == "roadbuild")
    {
	if (phase != BUILD_PHASE)
	{
	    tell_one("You cannot play roadbuild in this phase", color);
	    return -1;
	}
	if (!cards[color] || cards[color][ROADBUILD] < 1)
	{
	    tell_one("You have no roadbuild card", color);
	    return -1;
	}
	if (develop[1][ROADBUILD] >= cards[color][ROADBUILD])
	{
	    tell_one("You must keep new cards one turn", color);
	    return -1;
	}
	develop[0] = 1;
	tell_some(capitalize(color)  + " plays a roadbuild card",
		  map_indices(players));
	cards[color][ROADBUILD]--;
	send_string("card " + cards[color][ROADBUILD] + " " +
		    resdesc[ROADBUILD] + "\n", color);
	freeroads = 2;
	return 1;
    }
    tell_one("Not possible", color);
    return 1;
}

static int
players(object tp)
{
    int    i, j, sz, num;
    string *ind;
    
    tp->catch_tell("\nCurrently playing are:\n");
    ind = map_indices(players);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	num = 0;
	if (cards[ind[i]])
	{
	    for (j=FOREST; j<DESERT; j++)
		num += cards[ind[i]][j];
	}
	tp->catch_msg(({ sprintf("%1s %8s - %3d cards (",
				 (current == ind[i] ? "*" : " "), ind[i], num),
			     QNAME(players[ind[i]][0]), ")\n" }), 0, 0);
    }
    return 1;
}

static int
roll(string arg, string color)
{
    int     i, j, sz, res, num, roll, *hex, *pos, *tmp;
    string  col, *pls;
    mapping got;
    
    if (!color)
	return 0;
    if (arg != "dice")
    {
	if (players[color][0])
	    players[color][0]->catch_tell("Roll what?\n");
	return -1;
    }
    if (!check(color, DICE_PHASE, "dice"))
	return -1;
    
    roll = random(6) + random(6) + 2;
    pls = map_indices(players);
    tell_some(capitalize(color) + " rolls the dice... " + roll, pls);
    if (roll == 7)
    {
	for (i=discardnum=0; i<maxplayers; i++)
	{
	    for (j=FOREST, num=0; j<DESERT; j++)
		if (got = cards[colors[i]])
		    num += got[j];
	    if (num > 7)
	    {
		tell_some(capitalize(colors[i]) + " must discard " + num / 2 +
			  " cards", pls);
		cards[colors[i]][DISCARD] = num / 2;
		discardnum += num / 2;
	    }
	}
	if (discardnum > 0)
	    phase = DISCARD_PHASE;
	else
	    phase = THIEF_PHASE;
	tell_one("You have to set the 'thief' now", color);
	return 1;
    }
    
    phase = BUILD_PHASE;

    /* check all producing hex, is there any village or town? */
    if (!(sz = sizeof(hex = probability[roll])))
	return 1;
    pos = allocate(6);
    for (i=0; i<sz; i++)
    {
	if ((num = hex[i]) == thief)
	    continue;
	
	res = resources[num];
	
	/* get all corners of the current hex */
	pos[0] = get_pos(num, "nw");
	pos[1] = get_pos(num, "ne");
	pos[2] = get_pos(num, "e");
	pos[3] = get_pos(num, "se");
	pos[4] = get_pos(num, "sw");
	pos[5] = get_pos(num, "w");
	for (j=0, got=([ ]); j<6; j++)
	{
	    if (col = villages[pos[j]])
		got[col]++;
	    else if (col = towns[pos[j]])
		got[col] += 2;
	}
	for (j=0; j<maxplayers; j++)
	{
	    col = colors[j];
	    if ((num = got[col]) > 0)
	    {
		if (!cards[col])
		    cards[col] = ([ res : num ]);
		else
		    cards[col][res] += num;
		
		tell_one("You got " + num + " " + resdesc[res], col);
		send_string("card " + cards[col][res] + " " +
			    resdesc[res] + "\n", col);
	    }
	}
    }
    return 1;
}

static int
trade(string arg, string color)
{
    if (!check(color, BUILD_PHASE, "build and trade"))
	return -1;

    if (!arg || !sscanf(arg, "with %s", trader) ||
	(trader != "bank" && !players[trader]))
    {
	trader = 0;
	tell_one("Thats no player color", color);
	return -1;
    }
    tell_some(capitalize(color)  + " now trades with " +
	      trader, map_indices(players));
    return 1;
}

static int
thief(string arg, string color)
{
    int     i, sz, rnd, tpos, hex, *pos, *ind;
    string  col;
    mapping c;
    
    if (!color)
	return 0;
    if (!arg || sscanf(arg, "%d %s", hex, col) != 2 ||
	hex < 1 || hex > hexnum || col == current)
    {
	if (players[color][0])
	    players[color][0]->catch_tell("Usage: thief <hexnum> <color>\n");
	return -1;
    }
    if (!check(color, 0, ""))
	return -1;
    if (phase != THIEF_PHASE && phase != KNIGHT_PHASE)
    {
	tell_one("It's not the thief phase", color);
	return -1;
    }
    if (thief == hex)
    {
	tell_one("The thief must be placed on a different hex", color);
	return -1;
    }
    pos = allocate(6);
    pos[0] = get_pos(hex, "nw");
    pos[1] = get_pos(hex, "ne");
    pos[2] = get_pos(hex, "e");
    pos[3] = get_pos(hex, "se");
    pos[4] = get_pos(hex, "sw");
    pos[5] = get_pos(hex, "w");
    for (i=0; i<6; i++)
	if (villages[pos[i]] == col || towns[pos[i]] == col)
	    break;
    if (i != 6 && (c = cards[col]))
    {
	ind = map_indices(c) - DEVTYPES;
	if (sizeof(ind))
	{
	    rnd = ind[random(sizeof(ind))];
	    cards[col][rnd]--;
	    cards[color][rnd]++;
	    send_string("card " + cards[col][rnd] + " " + resdesc[rnd] +
			"\n", col);
	    send_string("card " + cards[color][rnd] + " " + resdesc[rnd] +
			"\n", color);
	    tell_one("You got one " + resdesc[rnd], color);
	    tell_one("You lost one " + resdesc[rnd], col);
	}
    }
    
    send_string("thief " + hex + "\n");

    tpos = hexpos[thief] + strlen(LINE) * 3 + 4;
    map = map[..tpos] + "  " + map[tpos+3..];
    thief = hex;
    tpos = hexpos[thief] + strlen(LINE) * 3 + 4;
    map = map[..tpos] + "XX" + map[tpos+3..];

    if (phase == KNIGHT_PHASE)
	phase = DICE_PHASE;
    else
	phase = BUILD_PHASE;
    tell_some("The thief appears at hex " + hex, map_indices(players));
    return 1;
}
