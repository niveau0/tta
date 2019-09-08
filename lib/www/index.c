int counter;

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
start(mapping args)
{
    string s, e;

    restore_object("/syslog/wwwcounter");

    if (!args["file"])
	s = read_file("/www/index.ger");
    else
	s = read_file("/www/index.eng");
    if (!s)
	return 0;
    sscanf(s, "%sCOUNTER%s", s, e);
    counter++;
    save_object("/syslog/wwwcounter");
    return (s + counter  + e);
}
