#include <net.h>

/*
 * NAME:        edit()
 * DESCRIPTION: handle an editor command
 */
nomask varargs void
edit(string cmd)
{
    object po;
    
    po = previous_object();
    if (!query_ip_number(po))
	error("Illegal call to edit");
    if (cmd = editor(cmd))
	po->catch_tell(cmd, 1);
}

/*
 * NAME:        rescue_file()
 * DESCRIPTION: attempt to rescue the currently edited file
 */
nomask void
rescue_file()
{
    string file, *path;
    object po, player;
    
    po = previous_object();
    if (program_name(po) != _USER)
	error("Illegal call to rescue_file");
    if (query_editor(this_object()) == "insert") 
	editor(".");
    if ((player = po->query_player()) &&
	sscanf(editor("f"), "\"%s\" [Modified] %*s", file) == 2)
    {
        if (sizeof(path = explode(file, "/")))
	{
	    editor("w! /home/" + po->query_player()->query_name() +
		   "/.ed_" + path[sizeof(path) - 1]);
	}
    }
}
