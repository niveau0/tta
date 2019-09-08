inherit "htmlbase";

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

nomask string
start()
{
    int    i;
    string html;
    object *u;

    html = get_htmlhead("Current user list");
    u = users();
    if (sizeof(u) == 1)
	html += "<b>There is only one person in the game.</b><p>";
    else
	html += "<b>There are "+sizeof(u)+" people in the game.</b><p>";
    for (i=sizeof(u); i--;)
	html += u[i]->query_presentation(u[i]) + "<br>\n";
    html += get_htmltrail();
    
    return html;
}
