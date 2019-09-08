static int
check_cost(string type, string color)
{
    int     i;
    string  str;
    mapping costs, c;

    switch (type)
    {
      case "develop":
	costs = ([ ORE:1, WOOL:1, WHEAT:1 ]);
	break;
      case "village":
	costs = ([ FOREST:1, WOOL:1, STONE:1, WHEAT:1 ]);
	break;
      case "town":
	costs = ([ ORE:3, WHEAT:2 ]);
	break;
      case "road":
	costs = ([ FOREST:1, STONE:1 ]);
	break;
    }
    
    /* check if we have the required cards */
    if (!(c = cards[color]))
    {
	tell_one("You have not enough resources to build that", color);
	return 0;
    }
    for (i=FOREST; i<DESERT; i++)
    {
	if (!costs[i])
	    continue;
	if (costs[i] > c[i])
	{
	    tell_one("You have not enough resources to build that", color);
	    return 0;
	}
    }
    str = "";
    for (i=FOREST; i<DESERT; i++)
    {
	if (!costs[i])
	    continue;
	c[i] -= costs[i];
	str += "card " + c[i] + " " + resdesc[i] + "\n";
    }
    send_string(str, color);
    cards[color] = c;
    return 1;
}
