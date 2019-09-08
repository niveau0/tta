inherit "living";

#include <event.h>
#include <macros.h>
#include <state.h>
#include <properties.h>

private int    allowed_move;
private object rider;

/*
 * Function name: create_mount
 * Description:   redefinable constructor
 */
static void
create_mount()
{
}

/*
 * Function name: create_living
 * Description:   standard constructor
 */
static nomask void
create_living()
{
    create_mount();
}

/* description functions here to add riding person */

string
long()
{
    string desc;
    object tp;

    if (!rider || !(tp = this_player()))
	return ::long();
    if (tp == rider)
	return ::long() + "Mounted by yourself.\n"; 
    if (desc = tp->query_met(rider->query_met_name()))
	return ::long() + "Mounted by " + capitalize(desc) + ")"; 
    return ::long() + "Mounted by the " + rider->query_nonmet_name() + ")";    
}

nomask string
short()
{
    string desc;
    object tp;

    if (!rider || !(tp = this_player()))
	return ::short();
    if (tp == rider)
	return ::short() + " (mounted by yourself)"; 
    if (desc = tp->query_met(rider->query_met_name()))
	return ::short() + " (mounted by " + capitalize(desc) + ")"; 
    return ::short() + " (mounted by the " +
	rider->query_nonmet_name() + ")";
}

nomask string
plural_short()
{
    string desc;
    object tp;

    if (!rider || !(tp = this_player()))
	return ::plural_short();
    if (tp == rider)
	return ::plural_short() + " (mounted by yourself)"; 
    if (desc = tp->query_met(rider->query_met_name()))
	return ::plural_short() + " (mounted by " + capitalize(desc) + ")"; 
    return ::plural_short() + " (mounted by the " +
	rider->query_nonmet_name() + ")";    
}

/*
 * Function name: dismount_cmd
 * Description:   called from cmdsoul to dismount this horse
 * Arguments:     tp - dismounting living
 * Returns:       1 if success, -1 else
 */
nomask int
dismount_cmd(object tp)
{
    int   *co;
    mixed qtname;
    
    if (!tp)
	return -1;
    if (rider != tp)
    {
	tp->catch_tell("You did not mount this living.\n");
	return -1;
    }
    remove_event(E_MOVE, "catch_moves", this_object());
    remove_event(E_MOVE, "catch_moves", rider);
    rider = 0;
    tp->delete_state(MOUNT);
    tp->add_prop(LIVE_MOUNTED, 0);
    co = query_coordinates()[..];
    co[2] = E(this_object())->query_coordinates()[2];
    tp->set_coordinates(co);

    qtname = QTNAME(this_object());
    tp->catch_msg(({ "You dismount ", qtname, ".\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " dismounts ", qtname, ".\n" }),
		 MSG_SEE, tp);
    return 1;
}

/*
 * Function name: mount_cmd
 * Description:   called from cmdsoul to mount this horse
 * Arguments:     tp - mounting living
 * Returns:       1 if success, -1 else
 */
nomask int
mount_cmd(object tp)
{
    int   *co;
    mixed qtname;
    
    if (!tp)
	return -1;
    if (rider)
    {
	tp->catch_tell("There is already someone.\n");
	return -1;
    }
    if (tp->query_prop(LIVE_MOUNTED))
    {
	tp->catch_tell("You already mounted something.\n");
	return -1;
    }
    rider = tp;
    add_event(E_MOVE, "catch_moves", this_object());
    add_event(E_MOVE, "catch_moves", rider);
    tp->add_state(MOUNT);
    tp->add_prop(LIVE_MOUNTED, this_object());
    co = query_coordinates()[..];
    co[2] += query_size() / 3;
    tp->set_coordinates(co);

    qtname = QTNAME(this_object());
    tp->catch_msg(({ "You mount ", qtname, ".\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " mounts ", qtname, ".\n" }),
		 MSG_SEE, tp);
    return 1;
}

/*
 * Function name: query_rider()
 * Description:   the one that rides on this mount
 * Returns:       object
 */
nomask object
query_rider()
{
    return rider;
}

/*
 * Function name: catch_moves
 * Description:   event: catch moves of the mount and dismounts the rider
 *                       if too far away
 * Arguments:     ob - the moving object
 *                old - where it comes from
 *                dest - where it goes
 */
nomask void
catch_moves(object ob, object old, object dest)
{
    int *co, *obco;
    
    if (!rider || allowed_move)
	return;
    co = query_coordinates();
    obco = rider->query_coordinates();

    if (co[0] != obco[0] || co[1] != obco[1] ||
	co[2] + query_size() / 3 != obco[2])
    {
	rider->add_prop(LIVE_MOUNTED, 0);
	rider->delete_state(MOUNT);
	remove_event(E_MOVE, "catch_moves", this_object());
	remove_event(E_MOVE, "catch_moves", rider);
	rider = 0;
    }
}

/*
 * Function name: move_mount
 * Description:   called from command soul, if horse is mounted and
 *                player moves.
 * Arguments:     room - arrival room
 *                co - new coordinates
 *                dir - direction to move
 *                how - run or walk
 *                tp - riding player
 * Returns:       1 if success, -1 else
 */
nomask int
move_mount(object room, int *co, string dir, int how, object tp)
{
    if (!tp || rider != tp)
	return -1;
    allowed_move = 1;
    if (::move_living(room, co, dir, 2)) /* sneak-move to avoid messages */
    {
	allowed_move = 0;
	return -1;
    }
    co[2] += query_size() / 3;
    rider->move_living(room, co, dir, 4);
    allowed_move = 0;
    return 1;
}

/*
 * Function name: move_living
 * Description:   called from command soul, if horse walks, the rider
 *                should follow.
 * Arguments:     room - arrival room
 *                co - new coordinates
 *                dir - direction to move
 *                how - run or walk
 * Returns:       1 if success, -1 else
 */
nomask int
move_living(object room, int *co, string dir, int how)
{
    if (!rider)
	return -1;
    allowed_move = 1;
    if (::move_living(room, co, dir, how))
    {
	allowed_move = 0;
	return -1;
    }
    co = co[..];
    co[2] += query_size() / 3;
    rider->set_coordinates(co);
    rider->move(room);
    allowed_move = 0;
    return 1;
}

nomask int
do_die(object killer)
{
    if (rider)
	dismount_cmd(rider);
    return ::do_die(killer);
}

nomask varargs void
remove_object(object ob)
{
    if ((!ob || ob == this_object()) && rider)
	dismount_cmd(rider);
    ::remove_object(ob);
}
