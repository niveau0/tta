#ifndef MACROS_H
#define MACROS_H

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define ABS(a)   ((a) >  0  ? (a) : -(a))

#define TO()      this_object()
#define OBJ(o)    find_object(o)
#define E(o)      ((o)->environment())
#define I(o, f)   ((o)->inventory(f))
#define BS(s,b,i) break_string(s, b, i)
#define MASTER    program_name(this_object())
#define MASTER_OB(ob) program_name(ob)
#define IS_CLONE (object_name(this_object()) != program_name(this_object()))
#define OB_NUM(ob) (explode(object_name(ob),"#")[1])

#define QCTNAME(o) ("/system/composite"->object_desc(o, 1, 't'))
#define QTNAME(o) ("/system/composite"->object_desc(o, 0, 't'))
#define QCNAME(o) ("/system/composite"->object_desc(o, 1, 'a'))
#define QNAME(o) ("/system/composite"->object_desc(o, 0, 'a'))

#define IN_ARRAY(e, a) (member_array(e, a) > -1)

/* true if ob1 sees ob2 */
#define VISIBLE(ob1, ob2) \
call_other(ob1,"can_see",ob2)

/* true if ob1 hears ob2 (for given distance) */
#define CAN_HEAR(ob1, ob2, dist) \
call_other(ob1,"can_hear",ob2, dist)

#endif
