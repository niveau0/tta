inherit "/base/object";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

private int duration, on, light_level;

/*static nomask void set_duration(int new_dur);*/
nomask void set_duration(int new_dur);

static void
create_light()
{
    set_name("light");
    set_short("light");
    set_pshort("lights");
    set_long("A simple wooden light.\n");
    set_lwvs(0, 453, 129, 12);
    set_value(1);
    set_duration(0);
    add_prop(OBJ_MATERIAL, M_OAK);
}

static nomask void
create_object()
{
    light_level = 1; /* default */
    create_light();
    on = 0;
}

/*
 * Function name: query_burning
 * Description:   is true if the light is on fire
 * Returns:       integer
 */
nomask int
query_burning()
{
    return on;
}

/*
 * Function name: set_light_level
 * Description:   set the brightness of the light device.
 *                light value equals how many inches you can see
 *                with that light.
 * Arguments:     l - light level
 */
static nomask void
set_light_level(int l)
{
    light_level = l;
}

/*
 * Function name: query_light_level
 * Description:   Estimate the brightness of this light
 * Returns:       integer - level
 */
nomask int
query_light_level()
{
    return light_level;
}

/*
 * Function name: set_duration
 * Description:   set the duration the light will burn.
 * Arguments:     dur - seconds
 */
/*static nomask void*/
nomask void
set_duration(int dur)
{
    duration = dur;
}

/*
 * Function name: query_duration
 * Description:   Estimate the duration a light will still burn.
 *                Will only work with light not currently burning.
 * Returns:       integer - seconds
 */
int
query_duration()
{
    return duration;
}

nomask string
short()
{
    if (on)
	return "lit " + ::short();
    return ::short();
}

nomask string
plural_short()
{
    if (on)
	return "lit " + ::plural_short();
    return ::plural_short();
}

/*
 * Function name: light_cmd
 * Description:   light this object. called from cmdsoul
 * Returns:       1 if success, -1 else
 */
int
light_cmd()
{
    object tp;

    tp = this_player();
    if (on)
    {
	tp->catch_tell("The " + short() + " is already lit!\n");
	return -1;
    }

    if (!duration)
    {
	tp->catch_tell("The " + short() + " is totally burned out.\n");
	return -1;
    }
    
    tp->catch_tell("You light the " + short() + ".\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " lights ", QNAME(this_object()), ".\n" }),
		 MSG_SEE);
    add_lwvs(light_level, 0, 0, 0); 
    on = time();
    call_out("fade", duration);
    return 1;
}

/*
 * Function name: extinguish
 * Description:   put the light out.
 * Arguments:     arg - ignored
 */
int
extinguish_cmd(string arg)
{
    object tp;
    
    tp = this_player();
    if (!on)
    {
	tp->catch_tell("The " + short() + " is not lit.\n");
	return -1;
    }
    duration -= (time() - on);
    on = 0;
    remove_call_out("fade");
    add_lwvs(-light_level, 0, 0, 0);
    tp->catch_tell("You extinguish the light.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " extinguishes ", QNAME(this_object()),
			".\n" }), MSG_SEE);
    if (duration < 1)
	set_value(0);
    return 1;
}

/*
 * Function name: fade
 * Description:   called when light goes out.
 */
static nomask void
fade()
{
    object env;

    if (!on)
	return;

    duration = on = 0;
    set_value(0);

    env = E(this_object());
    if (living(env))
    {
	tell_objects(  LISTEN(E(env))- ({ env }),
		     ({ QCTNAME(this_object()), " of ", QTNAME(env),
			    " fades.\n" }), MSG_SEE, 0);
	env->catch_msg("Your light goes dark.\n", 0, MSG_SEE);
    }
    else
	tell_objects(LISTEN(env),
		     ({ QCNAME(this_object()), " goes dark.\n" }),
		     MSG_SEE, 0);
    /* lower lightlevel after everything else is done, to prevent message
       'Something fades.' */
    add_lwvs(-light_level, 0, 0, 0);
}
