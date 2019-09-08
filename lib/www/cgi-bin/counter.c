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
    
    s = read_file("/www/index.html");
    sscanf(s, "%sCOUNTER%s", s, e);
    return (s + counter + e);
}
