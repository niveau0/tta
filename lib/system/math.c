/*
 * Function name: find_exp
 * Description:   Find the solution to 2^x = sum.
 * Arguments:	  sum - The sum to find the solution to.
 * Returns:       x
 */
int
find_exp(int sum)
{
    int x, i;
    
    x = 1;
    while (sum > x)
    {
	i++;
	x += x; /* x = 2 * x; */
    }
    
    return i;
}

/*
 * Function name: inches_to_string
 * Description:   Convert and int describing inches to a string describing
 *                leagues, miles, furlongs, ...
 * Arguments:     int inches
 * Returns:       string
 */
string *
inches_to_strings(int inches)
{
    string *erg;
    erg = ({ });

    if (inches > 380160)
    {
	erg += ({ (inches / 190080) + " leagues" });
	inches %= 190080;
    }
    else if (inches > 190079)
    {
	erg += ({ "1 league" });
	inches %= 190080;
    }
    if (inches > 126719)
    {
	erg += ({ (inches / 63360) + " miles" });
	inches %= 63360;
    }
    else if (inches > 63359)
    {
	erg += ({ "1 mile" });
	inches %= 63360;
    }
    if (inches > 15839)
    {
	erg += ({ (inches / 7920) + " furlongs" });
	inches %= 7920;
    }
    else if (inches > 7919)
    {
	erg += ({ "1 furlong" });
	inches %= 7920;
    }
    if (inches > 1583)
    {
	erg += ({ (inches / 792) + " chains" });
	inches %= 792;
    }
    else if (inches > 791)
    {
	erg += ({ "1 chain" });
	inches %= 792;
    }
    if (inches > 395)
    {
	erg += ({ (inches / 198) + " rods" });
	inches %= 198;
    }
    else if (inches > 197)
    {
	erg += ({ "1 rod" });
	inches %= 198;
    }
    if (inches > 71)
    {
	erg += ({ "" + (inches / 36) + " yards" });
	inches %= 36;
    }
    else if (inches > 35)
    {
	erg += ({ "1 yard" });
	inches %= 36;
    }
    if (inches > 23)
    {
	erg += ({ (inches / 12) + " feet" });
	inches %= 12;
    }
    else if (inches > 11)
    {
	erg += ({ "1 foot" });
	inches %= 12;
    }

    if (inches > 1)
	erg += ({ inches + " inches" });
    else if (inches)
	erg += ({ "1 inch" });

    return erg;
}

/*
 * Function name: string_to_inches
 * Description:   Parse a string of miles, feet etc. to get an int of inches
 * Arguments:     array consisting of strings that are pairs of a number
 *                and one of the following words: league, leagues,
 *                mile, miles, furlong, furlongs, chain, chains, rod,
 *                rods, yard, yards, foot, feet, inch or inches.
 * Returns:       int
 */
int
strings_to_inches(string *inp)
{
    int n, i, inches;
    string s;

    for (i = 0; i < sizeof(inp); i++)
    {
	if (sscanf(inp[i],"%d %s",n,s) == 2)
	{
	    switch(s)
	    {
	      case "league":
	      case "leagues":
		inches += n * 190080;
		break;
	      case "mile":
	      case "miles":
		inches += n * 63360;
		break;
	      case "furlong":
	      case "furlongs":
		inches += n * 7920;
		break;
	      case "chain":
	      case "chains":
		inches += n * 792;
		break;
	      case "rod":
	      case "rods":
		inches += n * 198;
		break;
	      case "yard":
	      case "yards":
		inches += n * 36;
		break;
	      case "foot":
	      case "feet":
		inches += n * 12;
		break;
	      case "inch":
	      case "inches":
		inches += n;
	    }
	}
    }
    return inches;
}

float
sqrt(mixed x)
{
    float r, store;
    
    x = (float)x;
    r = 1.0;
    
    while (store != r)
    {
	store = r;
	r = 0.5 * (r + (x / r));
    }
    return r;
}
