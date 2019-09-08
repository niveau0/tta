/*
 * auto inherited object
 */

#include <access.h>
#include <config.h>
#include <event.h>
#include <kernel.h>
#include <net.h>
#include <status.h>
#include <trace.h>
#include <type.h>
#include "auto.h"
 
#include "creator.c"
#include "call_out.c"
#include "comm.c"
#include "file.c"
#include "event.c"
#include "living.c"
#include "net.c"
#include "simfun.c"
#include "tool.c"

private object prev, next;

/* 
 * Function name: _F_create
 * Description:   base initialization of object, called by DGD
 */
nomask void
_F_create()
{
  if (getuid())
    return;
    
  init_uid();
    
  rlimits(50; 500000)
    {
      catch(this_object()->create());
    }
}

int
_Q_destruct()
{
  if (::object_name(this_object()) == _AUTO)
    return _ADMIN;
  return 0;
}

/*
 * Function name: _Q_prev
 * Description:   give previous object in object chain
 * Returns:       object
 */
nomask object
_Q_prev()
{
  return prev;
}

/*
 * Function name: _Q_next
 * Description:   give following object in object chain
 * Returns:       object
 */
nomask object
_Q_next()
{
  return next;
}

/*
 * Function name: _F_set_prev
 * Description:   set previous object in object chain
 *                only allowed for _DRIVER
 * Arguments:     p - previous object
 */
nomask void
_F_set_prev(object p)
{
  if (previous_program() == _DRIVER)
    prev = p;
}

/*
 * Function name: _F_set_next
 * Description:   set following object in object chain
 *                only allowed for _DRIVER
 * Arguments:     n - following object
 */
nomask void
_F_set_next(object n)
{
  if (previous_program() == _DRIVER)
    next = n;
}
