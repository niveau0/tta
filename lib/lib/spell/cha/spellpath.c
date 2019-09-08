#include <magic.h>

static mapping pathes;

static void
create()
{
    pathes =
	    ([
		"anar"		: "open",
		"brel"		: "open",
		"cor"	        : "open",
		"dec"           : "open",
		"dri"         	: "open",
		"nat"		: "open",
		"pure"	        : "open",
		"sou"		: "open",
		"weth"		: "open",

		"clec"		: "closed",
		"drip"		: "closed",
		"lota"		: "closed",
		"nera"		: "closed",
		"org"		: "closed",
		"orl"		: "closed",
		"syc" 		: "closed",
		"ulcum"		: "closed",
		"wek"		: "closed",
		
		"comu"		: "base",
		"licra"		: "base",
		"nhar"	        : "base",
		"piro"	        : "base",
		"pro"		: "base",
		"sumo"		: "base",
		]);
}

string
query_path(string list)
{
    string type;
    
    if (type = pathes[list])
	return SPELL_DIR + "/cha/" + type + "/" + list + "/";
    return 0;
}
