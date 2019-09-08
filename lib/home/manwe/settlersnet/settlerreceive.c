#define DIR "/home/manwe/settlers/"

static string ipnum, login;

/*
 * NAME:	open()
 * DESCRIPTION:	start a user connection on this object
 */
static void
open()
{
    ipnum = query_ip_number();
}

/*
 * NAME:	close()
 * DESCRIPTION:	close the user connection on this object
 */
nomask void
close(int f)
{
    if (!f)
    {
	_DRIVER->message(ipnum + " disconnected.\n");
	destruct_object();
    }
}

nomask string
query_name()
{
    return login;
}

/*
 * NAME:	receive_message()
 * DESCRIPTION:	receive user input
 */
static nomask void
receive_message(string str)
{
    string pw;
    
    if (!str)
	return;
    if (sscanf(str, "login %s %s\r\n", login, pw) == 2)
    {
	if (!(DIR + "settlers")->add_client(this_object(), login, pw))
	    close(0);
	return;
    }
    if (!login)
    {
	close(0);
	return;
    }
    (DIR + "settlers")->receive(str);
}

/*
 * NAME:	send_string()
 * DESCRIPTION:	send to client
 */
nomask void
send_string(string str)
{
    if (object_name(previous_object())[..strlen(DIR)-1] == DIR)
	send_message(str);
}
