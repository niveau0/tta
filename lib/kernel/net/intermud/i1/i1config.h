#include <access.h>
#include <config.h>
#include <intermud.h>
#include <net.h>
#include <status.h>

#define UDP_VERSION "1.0.1"

/* status flags */
#define UDP_UNKNOWN 0
#define UDP_TIMEOUT -1

/* IP_LIST, array indices */
#define LIST_IP     0
#define LIST_PORT   1
#define LIST_STATUS 2

#define UDP_LOCAL_IP   "134.106.121.220"

#define IPLIST \
([ \
 IMUD_MUDNAME   : ({ UDP_LOCAL_IP, PORT_IMUD1_UDP, time() }), \
 "Demonworld"   : ({ "134.106.121.220", 3005, UDP_UNKNOWN }), \
 "Xyllomer"     : ({ "131.234.38.60",   3335, UDP_UNKNOWN }), \
 "Morisot"      : ({ "130.75.24.156",   4005, UDP_UNKNOWN }), \
 "Hypermud"     : ({ "131.234.96.36",   3335, UDP_UNKNOWN }), \
 "Geas"         : ({ "194.94.249.6",    3335, UDP_UNKNOWN }), \
    ])

/* message codes */
#define RCV_CH_LIST  1 /* channel listing */
#define RCV_CH_TELL  2 /* channel tells */
#define RCV_TELL     3 /* single tell */
#define RCV_WHO      4 /* player list query */
#define RCV_INFO     5 /* connection info query */
#define RCV_MAIL     6 /* mail */

/* some lib specific definitions: */

/* how to get the object of a user */
#define USER(user) find_player(user)
/* how to tell a message to a user */
#define TELL(userobj, message) userobj->catch_tell(message)
/* how to send a message with more to a user */
#define MORE(userobj, message) smore(message, userobj)
