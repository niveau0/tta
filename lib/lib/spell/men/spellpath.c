#include <magic.h>

static mapping pathes;

static void
create()
{
    pathes =
	    ([
		"celi"		: "open",
		"cevo"		: "open",
		"delil"		: "open",
		"gilo"		: "open",
		"hafel"		: "open",
		"reda"		: "open",
		"refa"		: "open",
		"sede"		: "open",
		
		"b"		: "closed",
		
		"c"		: "base",
		]);
}

string
query_path(string list)
{
    string type;
    
    if (type = pathes[list])
	return SPELL_DIR + "/men/" + type + "/" + list + "/";
    return 0;
}
