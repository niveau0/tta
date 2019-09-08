/* inherit this file to have a base support for the commands
 * - open
 * - close
 * - lock
 * - unlock
 * - pick
 *
 * where the lock/unlock/pick are only active if you set a key (set_key)
 * also inherit the parse() function (/system/parse.c)
 */
inherit "/system/parse";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <properties.h>
#include <skills.h>
#include <slots.h>

nomask void add_prop(mixed prop, mixed val);
nomask mixed query_prop(mixed prop);
static nomask varargs object * parse(string p, object *list,
				     object tp, int distance);

private static int    lock_difficulty;
private static string key_file;

/*
 * Function name: set_key
 * Description:   if this container can only be opened with a key this
 *                function should be called with the keyfile as argument
 * Arguments:     kf - key filename
 */
static nomask void
set_key(string kf)
{
    key_file = kf;
    if (lock_difficulty < 1)
	lock_difficulty = 50;
}

/*
 * Function name: query_key
 * Description:   returns the keyfile if this container has a lock
 * Returns:       string, key filename
 */
nomask string
query_key()
{
    return key_file;
}

/*
 * Function name: set_lock_difficulty
 * Description:   how difficult is the lock to pick
 * Arguments:     d - difficulty
 */
static nomask void
set_lock_difficulty(int d)
{
    lock_difficulty = d;
}

/*
 * Function name: query_lock_difficulty
 * Description:   how difficult is the lock to pick
 * Returns:       integer, difficulty
 */
nomask int
query_lock_difficulty()
{
    return lock_difficulty;
}

/*
 * Function name: pick_cmd
 * Description:   called if someone tries to pick the lock of this container
 * Arguments:     str - optional argument (e.g. lockpick set)
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
pick_cmd(string str, object tp)
{
    int    bonus;
    object *lockpicks;
    
    if (!key_file)
    {
	tp->catch_tell("There is no lock.\n");
	return -1;
    }
    if (!query_prop(CONT_CLOSED))
    {
	tp->catch_tell("But it's open.\n");
	return -1;
    }
    if (query_prop(CONT_LOCKED))
    {
	tp->catch_tell("It's not locked.\n");
	return -1;
    }
    if (str)
    {
	if (!sizeof(lockpicks = parse(str, tp->query_hold(SLT_ALL), 0)))
	{
	    tp->catch_tell("You don't hold such a tool.\n");
	    return -1;
	}
	if (!(bonus = lockpicks->query_skill_bonus(SK_PICK_LOCKS)))
	{
	    tp->catch_tell("That won't help you.\n");
	    return -1;
	}
    }
    if (tp->skill_check(SK_PICK_LOCKS, bonus, 0) +
	tp->skill_check(SK_LOCK_LORE, -20, 1) < lock_difficulty)
    {
	tp->catch_tell("You failed.\n");
	return -1;
    }
    add_prop(CONT_LOCKED, 0);
    tp->echo("You unlock it.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " unlocks ", QNAME(this_object()),
			".\n" }), MSG_SEE, tp);
    return 1;
}

/*
 * Function name: lock_cmd
 * Description:   called if someone tries to lock this container
 * Arguments:     str - optional argument (e.g. white key)
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
lock_cmd(string str, object tp)
{
    object *keys;
    
    if (!key_file)
    {
	tp->catch_tell("There is no lock.\n");
	return -1;
    }
    if (!query_prop(CONT_CLOSED))
    {
	tp->catch_tell("But it's open.\n");
	return -1;
    }
    if (query_prop(CONT_LOCKED))
    {
	tp->catch_tell("It's already locked.\n");
	return -1;
    }
    if (!str || !sizeof(keys = parse(str, I(tp, 4), 0)))
    {
	tp->catch_tell("You need something like a key.\n");
	return -1;
    }
    if (program_name(keys[0]) != key_file)
    {
	tp->catch_tell("The key doesn't fit.\n");
	return -1;
    }
    add_prop(CONT_LOCKED, 1);
    tp->echo("You lock it.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " locks ", QNAME(this_object()),
			".\n" }), MSG_SEE, tp);
    return 1;
}

/*
 * Function name: unlock_cmd
 * Description:   called if someone tries to unlock this container
 * Arguments:     str - optional argument (e.g. white key)
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
unlock_cmd(string str, object tp)
{
    object *keys;
    
    if (!key_file)
    {
	tp->catch_tell("There is no lock.\n");
	return -1;
    }
    if (!query_prop(CONT_LOCKED))
    {
	tp->catch_tell("It's already unlocked.\n");
	return -1;
    }
    if (!str || !sizeof(keys = parse(str, I(tp, 4), 0)))
    {
	tp->catch_tell("You need something like a key.\n");
	return -1;
    }
    if (program_name(keys[0]) != key_file)
    {
	tp->catch_tell("The key doesn't fit.\n");
	return -1;
    }
    add_prop(CONT_LOCKED, 0);
    add_prop(CONT_CLOSED, 1);  /* just to be sure */
    tp->echo("You unlock it.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " unlocks ", QNAME(this_object()),
			".\n" }), MSG_SEE, tp);
    return 1;
}

/*
 * Function name: open_cmd
 * Description:   called if someone tries to open this container
 * Arguments:     str - optional argument (e.g. with white key)
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
open_cmd(string str, object tp)
{
    if (str && unlock_cmd(str, tp) == -1)
	return -1;
    if (!query_prop(CONT_CLOSED))
    {
	tp->catch_tell("It's already open.\n");
	return -1;
    }
    if (query_prop(CONT_LOCKED))
    {
	tp->catch_tell("It's locked.\n");
	return -1;
    }
    tp->echo("You open it.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " opens ", QNAME(this_object()),
			".\n" }), MSG_SEE, tp);
    add_prop(CONT_CLOSED, 0);
    return 1;
}

/*
 * Function name: close_cmd
 * Description:   called if someone tries to close this container
 * Arguments:     str - optional argument (e.g. with white key)
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
close_cmd(string str, object tp)
{
    if (str && lock_cmd(str, tp) == -1)
	return -1;
    if (query_prop(CONT_CLOSED))
    {
	tp->catch_tell("It's already closed.\n");
	return -1;
    }
    tp->echo("You close it.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " closes ", QNAME(this_object()),
			".\n" }), MSG_SEE, tp);
    add_prop(CONT_CLOSED, 1);
    return 1;
}
