#ifndef I2CONFIG_H
#define I2CONFIG_H

#include <access.h>
#include <config.h>
#include <intermud.h>
#include <net.h>

private int
to_int(string x)
{
    int res;
    sscanf(x, "%d", res);
    return res;
}

private mixed
filter_array(mixed arr, string func, object o, string extra)
{
    int i;
    mixed res;
    for (i=sizeof(arr), res=allocate(i); i--;)
	if (call_other(o, func, arr[i], extra))
	    res[i] = arr[i];
    return res - ({ 0 });
}

private mixed
map_array(mixed arr, string func, object o)
{
    int i;
    mapping res;
    res = ([ ]);
    for (i=sizeof(arr); i--;)
	res[arr[i]] = call_other(o, func, arr[i]);
    return res;
}

#define to_string(x)		 ("" + (x))
#define copy_mapping(x)		 (x + ([ ]))

#define REPLY_TIME_OUT	10
#define RETRIES		1

/* Buffered packet fragment timeout */
#define INCOMING_TIME_OUT	60
/* Timout for received packet id's */
#define RECEIVED_TIME_OUT	60

#define DIAGNOSTICS

/* --- Standard header macros. --- */

#define RECIPIENT	"RCPNT"
#define REQUEST		"REQ"
#define SENDER		"SND"
/* The DATA field should be used to store the main body of any packet. */
#define DATA		"DATA"

/* These headers are reserved for system use only. */
#define HOST		"HST"
#define ID		"ID"
#define NAME		"NAME"
#define PACKET		"PKT"
#define UDP_PORT	"UDP"
#define SYSTEM		"SYS"

/* Reserved headers for diagnostics. */
#define PACKET_LOSS	"PKT_LOSS"
#define RESPONSE_TIME	"TIME"

/* --- Standard REQUEST macros. --- */

#define PING		"ping"
#define QUERY		"query"
#define REPLY		"reply"

/* --- Standard SYSTEM macros. --- */

#define FAILED		"F"
#define REPEAT		"R"
#define TIME_OUT	"TO"
#define UNAVAILABLE	"U"

/* --- Index macros for host arrays. --- */

#define HOST_NAME	0
#define HOST_IP		1
#define HOST_UDP_PORT	2
#define LOCAL_COMMANDS	3
#define HOST_COMMANDS	4
#define HOST_STATUS	5

#define PARSE_MUD_NAME(x)	implode(explode(x, " "), "_")
/* basic commands */
#define COMMANDS ({ "ping", "channel", "tell", "who" })

#define USE_OLD_DELIMITER
#define DELIMITER_COMPAT
#define USE_OLD_DATA_FORMAT

#endif
