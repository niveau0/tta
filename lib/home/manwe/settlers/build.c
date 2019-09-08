static string build_history;

static int
get_develop(string color)
{
    int res;
    
    if (!check_cost("develop", color))
	return 0;
    if (!sizeof(devcards))
    {
	tell_one("There are no cards left", color);
	return 0;
    }
    res = devcards[0];
    devcards = devcards[1..];
    cards[color][res]++;
    tell_one("You got 1 " + resdesc[res], color);
    send_string("card " + cards[color][res] + " " +
		resdesc[res] + "\n", color);
    develop[1][res]++;
    return 1;
}

static int
get_road_pos(int hex, string dir)
{
    int pos;
    
    if (hex < 0 || hex > map_sizeof(hexpos))
	return 0;
    pos = hexpos[hex];
    switch (dir)
    {
      case "nw":
	pos = pos + 1 + strlen(LINE);
	break;
      case "ne":
	pos = pos + XHEX - XHEX / 4 + strlen(LINE);
	break;
      case "sw":
	pos = pos + 1 + strlen(LINE) * (YHEX - 2);
	break;
      case "se":
	pos = pos + XHEX - XHEX / 4 + strlen(LINE) * (YHEX - 2);;
	break;
      case "n":
	pos = pos + XHEX / 2;
	break;
      case "s":
	pos = pos + XHEX / 2 + strlen(LINE) * (YHEX - 1);
	break;
      default:
	return 0;
    }
    return pos;
}

static int
get_pos(int hex, string dir)
{
    int pos;
    
    if (hex < 0 || hex > map_sizeof(hexpos))
	return 0;
    pos = hexpos[hex];
    switch (dir)
    {
      case "nw":
	pos = pos + XHEX / 4;
	break;
      case "ne":
	pos = pos + XHEX - XHEX / 4 - 1;
	break;
      case "sw":
	pos = pos + strlen(LINE) * (YHEX - 1) + XHEX / 4;
	break;
      case "se":
	pos = pos + strlen(LINE) * (YHEX - 1) + XHEX - XHEX / 4 - 1;
	break;
      case "w":
	pos = pos + strlen(LINE) * (YHEX / 2);
	break;
      case "e":
	pos = pos + strlen(LINE) * (YHEX / 2) + XHEX - 1;
	break;
      default:
	return 0;
    }
    return pos;
}

/* check if it is allowed to build road */
static int
road_allowed(int pos, string dir, string color)
{
    int *p;

    /* check adjacent villages and towns */
    p = allocate(2);
    switch (dir)
    {
      case "n":
      case "s":
	p[0] = pos - XHEX / 4;
	p[1] = pos + XHEX / 4;
	break;
      case "nw":
      case "se":
	p[0] = pos - 1 + strlen(LINE);
	p[1] = pos + 1 - strlen(LINE);
	break;
      case "ne":
      case "sw":
	p[0] = pos - 1 - strlen(LINE);
	p[1] = pos + 1 + strlen(LINE);
	break;
    }
    if (villages[p[0]] == color || villages[p[1]] == color ||
	towns[p[0]] == color || towns[p[1]] == color)
	return 1;
    
    /* check adjacent roads */
    p = allocate(4);
    switch (dir)
    {
      case "n":
      case "s":
	p[0] = pos - 3 - strlen(LINE);
	p[1] = pos - 3 + strlen(LINE);
	p[2] = pos + 3 - strlen(LINE);
	p[3] = pos + 3 + strlen(LINE);
	break;
      case "nw":
      case "se":
	p[0] = pos - 3 + strlen(LINE);
	p[1] = pos + 3 - strlen(LINE);
	p[2] = pos + strlen(LINE) * 2;
	p[3] = pos - strlen(LINE) * 2;
	break;
      case "ne":
      case "sw":
	p[0] = pos + 3 + strlen(LINE);
	p[1] = pos - 3 - strlen(LINE);
	p[2] = pos + strlen(LINE) * 2;
	p[3] = pos - strlen(LINE) * 2;
	break;
    }
    if (roads[p[0]] == color || roads[p[1]] == color ||
	roads[p[2]] == color || roads[p[3]] == color)
	return 1;
    return 0;
}

/* check if it is allowed to build village */
static int
village_allowed(int pos, string dir, string color)
{
    int *p;

    /* check adjacent villages and towns */
    p = allocate(3);
    switch (dir)
    {
      case "w":
      case "ne":
      case "se":
	p[0] = pos - 4;
	p[1] = pos + 2 - strlen(LINE) * 2;
	p[2] = pos + 2 + strlen(LINE) * 2;
	break;
      case "e":
      case "nw":
      case "sw":
	p[0] = pos + 4;
	p[1] = pos - 2 - strlen(LINE) * 2;
	p[2] = pos - 2 + strlen(LINE) * 2;
	break;
    }
    if (villages[p[0]] || villages[p[1]] || villages[p[2]] ||
	towns[p[0]] || towns[p[1]] || towns[p[2]])
	return 0;

    /* during init no adjacent roads required */
    if (phase == INIT_PHASE || phase == INIT_PHASE2) 
	return 1;
    
    /* check adjacent roads */
    p = allocate(3);
    switch (dir)
    {
      case "w":
      case "ne":
      case "se":
	p[0] = pos - 2;
	p[1] = pos + 1 - strlen(LINE);
	p[2] = pos + 1 + strlen(LINE);
	break;
      case "e":
      case "nw":
      case "sw":
	p[0] = pos + 2;
	p[1] = pos - 1 - strlen(LINE);
	p[2] = pos - 1 + strlen(LINE);
	break;
    }
    if (roads[p[0]] == color || roads[p[1]] == color || roads[p[2]] == color)
	return 1;
    return 0;
}

static int
put_road(int hex, string dir, string color)
{
    int    pos;
    string str;
    
    if (!(pos = get_road_pos(hex, dir)))
	return 0;
    if (roads[pos] || !road_allowed(pos, dir, color))
	return 0;
    if (phase == BUILD_PHASE)
    {
	if (freeroads > 0)
	    freeroads--;
	else if (!check_cost("road", color))
	    return 0;
    }
    roads[pos] = color;
    map = map[.. pos] + color[0..0] + map[pos+2 ..];
    str = "road " + hex + " " + COL_RGB[color] + " " + dir + "\n";
    build_history += str;
    send_string(str);
    return 1;
}

static int
put_village(int hex, string dir, string color)
{
    int    pos;
    string str;
    
    if (!(pos = get_pos(hex, dir)))
	return 0;
    if (villages[pos] || !village_allowed(pos, dir, color))
	return 0;
    if (villages[color] >= 5)
    {
	tell_one("You already own 5 villages", color);
	return 0;
    }
    if (phase == BUILD_PHASE && !check_cost("village", color))
	return 0;
    villages[pos] = color;
    villages[color]++;
    if (harbor[pos])
    {
	if (!harbor[color])
	    harbor[color] = ({ pos });
	else
	    harbor[color] += ({ pos });
    }
    map = map[.. pos-1] + "(" + color[0..0] + ")" + map[pos+3 ..];
    str = "vill " + hex + " " + COL_RGB[color] + " " + dir + "\n";
    build_history += str;
    send_string(str);
    return pos; /* return position here, needed to get start resources */
}

static int
put_town(int hex, string dir, string color)
{
    int    pos;
    string str;
    
    if (!(pos = get_pos(hex, dir)) || villages[pos] != color)
	return 0;
    if (towns[pos])
	return 0;
    if (towns[color] >= 4)
    {
	tell_one("You already own 4 towns", color);
	return 0;
    }
    if (phase == BUILD_PHASE && !check_cost("town", color))
	return 0;
    villages[pos] = 0;
    villages[color]--;
    towns[pos] = color;
    towns[color]++;
    map = map[.. pos-1] + "(" + capitalize(color[0..0]) + ")" + map[pos+3 ..];
    str = "town " + hex + " " + COL_RGB[color] + " " + dir + "\n";
    build_history += str;
    send_string(str);
    return 1;
}
