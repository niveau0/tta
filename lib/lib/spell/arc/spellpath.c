#include <magic.h>

static mapping pathes;

static void
create()
{
    pathes =
	    ([
		"a"		: "open",
		
		"b"		: "closed",
		
		"c"		: "base",
		]);
}

string
query_path(string list)
{
    string type;
    
    if (type = pathes[list])
	return SPELL_DIR + "/arc/" + type + "/" + list + "/";
    return 0;
}
