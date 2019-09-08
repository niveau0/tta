#define IMUD1 1
#define IMUD2 2
#define IMUD3 4

#define I1_DEMON       "/kernel/net/intermud/i1/i1d"
#define I2_DEMON       "/kernel/net/intermud/i2/i2d"
#define I3_DEMON       "/kernel/net/intermud/i3/i3d"

#define I2_SAVEFILE  "/kernel/data/INTERMUD2"
#define I3_SAVEFILE  "/kernel/data/INTERMUD3"

#define IMUD_MUDNAME       "ThirdAge"
#define MAX_UDP_PACKET_LEN 512

#define IMUD1_PORT_OBJ "/kernel/net/port/imud1"
#define IMUD2_PORT_OBJ "/kernel/net/port/imud2"

#define LOG(msg)     syslog("INTERMUD", msg);

#define RESTART_I3D \
call_other("/kernel/net/intermud/i3/restart", "restart_intermud", \
	   object_name(this_object()))

#define I2_SERVICE "/kernel/net/intermud/i2/service"
#define I2_PING    (I2_SERVICE + "/ping")
#define I2_TELL    (I2_SERVICE + "/tell")
#define I2_WHO     (I2_SERVICE + "/who")
