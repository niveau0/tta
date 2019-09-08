nomask void
restart_intermud(string path)
{
    call_out("restart", 1, path);
}

static nomask void
restart(string path)
{
    call_other(path, "???"); /* reload the intermud3 demon */
}
