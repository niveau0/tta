#include <magic.h>

static mapping pathes;

static void
create()
{
    pathes =
	    ([
		"lawn"          : "open",

		"phen"          : "open",
		
		"unwa"          : "open",

		"wade"          : "open",
		
		"yar"		: "open",
		
		"flim"          : "closed",

		"iref"          : "base",

		"ise"           : "base",
		
		"flad"          : "base",

		"sesig"         : "base",
		]);
}

string
query_path(string list)
{
    string type;
    
    if (type = pathes[list])
	return SPELL_DIR + "/ess/" + type + "/" + list + "/";
    return 0;
}
