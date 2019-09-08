# define _AUTO       "/kernel/lib/auto"    /* auto inherited object */
# define _ACCESSD    "/kernel/sys/accessd"
# define _DRIVER     "/kernel/sys/driver"  
# define _GLOBAL     "/kernel/sys/global"
# define _LOGIN      "/kernel/sys/login"
# define _SYSTEMD    "/kernel/sys/systemd"
# define _VBFC       "/kernel/lib/vbfc"    /* obj to call for vbfc */
# define _IMUDD      "/kernel/net/intermud/imudd"

# define _ROOMD      "/system/roomd"

/* message types */
# define MSG_HEAR  1
# define MSG_SEE   2
# define MSG_WALK  4 /* for 'walk' messages, so livings may ignore them */

#define _ROOT               "-2"   /* root uid */
#define _SYSTEM             "-1"   /* system uid */

/* coder levels */
#define _BUILDER            1
#define _CREATOR            2
#define _ADMIN              3

/* global debug macro */
# define debug(name, var)\
if (_GLOBAL->query_find_player(name))\
_GLOBAL->query_find_player(name)->\
catch_tell("DEBUG (in " + object_name(this_object()) + "):\n" +\
	   type_to_string(var, 0) + "\n");

# define set_this_player(p) (_GLOBAL->global_this_player(p))
# define this_player()      (_GLOBAL->query_this_player())
# define set_living_name(n) (_GLOBAL->global_living_name(n))
# define find_player(n)     (_GLOBAL->query_find_player(n))
# define find_living(n)     (_GLOBAL->query_find_living(n))
# define samethread()       (_GLOBAL->compare_thread())

# include "type.h"
# define intp(v)     (typeof(v) == T_INT)
# define floatp(v)   (typeof(v) == T_FLOAT)
# define stringp(v)  (typeof(v) == T_STRING)
# define pointerp(v) (typeof(v) == T_ARRAY)
# define objectp(v)  (typeof(v) == T_OBJECT)
# define mappingp(v) (typeof(v) == T_MAPPING)
# define living(o)   ((o)->_Q_alive())
