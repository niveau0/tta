#include <intermud.h>
#include <net.h>

/*
 * Function name: create
 * Description:   constructor of the udp port object
 */
static nomask void
create()
{
    /*    if (object_name(previous_object(2)) != _DRIVER)
	return;*/
    open_port("udp", PORT_IMUD2_UDP);
}

static nomask void
message(string msg)
{
    _DRIVER->message(msg);
}

/*
 * Function name: reopen
 * Description:   Tries to reopen the udp port if the port somehow accidently
 *                closed.
 */
static nomask void
reopen()
{
    _DRIVER->message("Trying to reopen port " + PORT_IMUD2_UDP +
			" (intermud2).\n");
    open_port("udp", PORT_IMUD2_UDP);
}

/*
 * Function name: open
 * Description:   Called if the udp port has been opened successfully.
 *                Also default talk channels will be opened
 * Arguments:     p - the port that has been opened for udp-protocol
 */
static nomask void
open(int p)
{
    call_out("message", 0, "Port " + PORT_IMUD2_UDP +
	     " (intermud2) successfully opened.\n");
}

/*
 * Function name: close
 * Description:   Called if the udp port gets closed. If this happens reopen()
 *                is called once.
 */
static nomask void
close()
{
    call_out("message", 0, "Port " + PORT_IMUD2_UDP +
	     " (intermud2) closed.\n");
    call_out("reopen", 0);
}

/*
 * Function name: send_udp
 * Description:   send a packet
 * Arguments:     data - data to send
 *                ipnum - receiver host
 *                port - receiver udp port
 */
nomask int
send_udp(string data, string ipnum, int port)
{
    if (object_name(previous_object()) != I2_DEMON)
	error("Illegal call to send_udp");
    /*debug("manwe", data +":"+ipnum+":"+port);*/
    send_datagram(data, ipnum, port);
    return 1;
}

/*
 * Function name: receive_datagram
 * Description:   Called if some data has arrived at the udp port.
 * Arguments:     data - the arriving data
 *                ipnum - the ipnum where data is coming from
 *                p - the port where data is coming from
 * Returns:       0 if finished, 1 if process_datagram should be called
 *                as soon as possible
 */
static nomask int
receive_datagram(string data, string ipnum, int port)
{
    if (!data)
	return 0;
    /*debug("manwe", "receive: " + data +":"+ipnum+":"+port);*/
    set_this_player(0);
    I2_DEMON->receive_udp(ipnum, data);
    return 0;
}
