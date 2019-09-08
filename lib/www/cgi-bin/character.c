inherit "htmlbase";

#include <config.h>

static void
create()
{
    call_out("cleanup", 10);
}

static nomask void
cleanup()
{
    destruct_object();
}

private int
checkcharacter(mapping args)
{
    string data;

    if (map_sizeof(args) < 9) /* including REQUEST_METHOD */
	return -3;
    if (file_size(PLAYER_FILE(args["NAME"]) + ".o") != -1 ||
	file_size("/players/application/" + args["NAME"] + ".o") != -1)
	return -2;
    data = "data ([8|1:\"" + args["EMAIL"] + "\",2:\"" + args["RACE"] +
	   "\",3:\"" + args["DESC"] + "\",4:\"" + args["GENDER"] +
	   "\",5:\"" + args["TALENT"] + "\",6:\"" + args["CAREER"] +
	   "\",7:\"" + args["NAME"] + "\",8:" + args["REALM"] + ",])";
    write_file("/players/application/" + args["NAME"] + ".o", data);
    return 0;
}

nomask string
start(mapping args)
{
    int    rt;
    string html;
    object *u;

    html = get_htmlhead("Character");
    
    if (args["REQUEST_METHOD"] != "post")
	html += "<p>Wrong http response.<p>\n";
    else if (rt = checkcharacter(args))
    {
	if (rt == -1)
	    html += "<p>Transfer error!<p>\n";
	else if (rt == -2)
	    html += "<p>Sorry, this name is already in use!<p>\n";
	else if (rt == -3) 
	    html += "<p>Some values were missed.<p>\n";
    }
    else
	html += "<p>Transfer successful. " +
		"You will soon receive your password.<p>\n";
    html += get_htmltrail();
    return html;
}
