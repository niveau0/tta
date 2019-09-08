#ifdef NET
/*
 * Function name: connect
 * Description:   open a connection (default protocol 'tcp')  
 * Arguments:     ipnumber - ipnumber as string
 *                port - port number
 *                protocol - internet protocol type
 */
static nomask varargs void
connect(string ipnumber, int port)
{
    if (object_name(this_object())[..11] != "/kernel/net/")
	error("Illegal call to connect");
    ::connect(ipnumber, port);
}

/*
 * Function name: open_port
 * Description:   open a port
 * Arguments:     protocol - internet protocol type
 *                port - port number
 */
static nomask void
open_port(string protocol, int port)
{
    if (object_name(this_object())[..11] != "/kernel/net/")
	error("Illegal call to open_port");
    ::open_port(protocol, port);
}
#endif
