inherit "/base/container";

#include <event.h>
#include <language.h>
#include <liquid.h>
#include <macros.h>
#include <material.h>
#include <mudtime.h>
#include <properties.h>

#define LIGHT_LEVEL 255

/* 1.00 inch wick lasts 4 x 24hrs (mudtime),
 * (4 * 24 * 3600 / 1)
 */
#define WICK_MOD    345600

/* 0.15 ltrs oil  last  1 x 12hrs (mudtime),
 * (1 * 12 * 3600 / 150)
 */
#define OIL_MOD     288

private int on, max_wick, cur_wick, wick_time, oil_time;

static nomask void set_max_wick(int max);
static nomask void set_wick(int new_len);

static nomask void
create_container()
{
    set_name("lamp");
    add_name("lantern");
    add_adj("oil");
    set_short("oil lamp");
    set_pshort("oil lamps");
    set_long("A device that needs oil, wick and a spark to spend light.\n");
    set_lwvs(0, 1000, 900, 12);
    set_max_volume(900 + 150);  
    set_max_weight(1000 + 150);  
    set_value(140);
    set_fill(0);		
    set_max_wick(12);		/* 12 inches fit into the lamp */
    set_wick(6);		/* 6 inch supplied on clone */
    add_prop(OBJ_MATERIAL, M_IRON);
    add_prop(CONT_RIGID, 1);
}

static nomask void
update_lamp()
{
    int wick_done, oil_done;
    
    if (on)
    {
	oil_done = (time() - oil_time) * TIME_SCALE / OIL_MOD;
	wick_done = (time() - wick_time) * TIME_SCALE / WICK_MOD;
	::add_fill(-oil_done);
	set_wick(cur_wick - wick_done);
	oil_time += oil_done * OIL_MOD / TIME_SCALE;
	wick_time += wick_done * WICK_MOD / TIME_SCALE;
    }
}

/*
 * Function name: query_burning
 * Description:   is true if object can be used to light something,
 * Returns:       integer
 */
nomask int
query_burning()
{
    return on;
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

string
long()
{
    update_lamp();

    if (!on && cur_wick < 1)
	return ::long() + "There's not much wick left.\n";

    return ::long();
}

static nomask void
set_max_wick(int max)
{
    max_wick = max;
}

static nomask void
set_wick(int new_len)
{
    cur_wick = new_len;
}

int
add_fill(int f)
{
    int oil_dur, wick_dur;
    
    if (on)
    {
	remove_call_out("fade");

	oil_dur  = query_internal_liquid() * OIL_MOD / TIME_SCALE; 
	wick_dur = cur_wick * WICK_MOD / TIME_SCALE; 
	if (oil_dur > wick_dur)
	    call_out("fade", wick_dur);
	else
	    call_out("fade", oil_dur);
    }
    return ::add_fill(f);
}

/*
 * Function name: fill_cmd
 * Description:   fill the lamp
 * Arguments:     ob - where does the liquid come from
 * Returns:       integer amount
 */
int
fill_cmd(object ob)
{
    int    len;
    object tp;
	
    if (ob->query_wick())
    {
	tp = this_player();

	if (on)
	{
	    tp->catch_tell("But it is burning.\n");
	    return -1;
	}
	len = ob->query_size();
	if (len > max_wick)
	{
	    tp->catch_tell("The wick is too long.\n");
	    return -1;
	}
	ob->add_lwvs(0, 0, 0, cur_wick - len);
	cur_wick = len - 1;

	tp->catch_tell("You get an old wick from the lamp "+
		       "and insert a new one.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " puts a new wick into ",
			QNAME(this_object()), ".\n" }), MSG_SEE, tp);
	return 1;
    }
    return ::fill_cmd(ob);
}

/*
 * Function name: light_cmd
 * Description:   light the lamp. called from cmdsoul
 * Returns:       1 if success, -1 else
 */
int
light_cmd()
{
    int    oil, oil_dur, wick_dur;
    object tp;

    tp = this_player();
    if (on)
    {
	tp->catch_tell("But it is lit!\n");
	return -1;
    }
    if (cur_wick < 2)
    {
	tp->catch_tell("Not enough wick to light the lamp.\n");
	return -1;
    }
    if (query_liquid_int() != LIQ_OIL ||
	(oil = query_internal_liquid()) < 2)
    {
	tp->catch_tell("Not enough oil to light the lamp.\n");
	return -1;
    }

    oil_time = wick_time = time();
    oil_dur  = oil * OIL_MOD / TIME_SCALE; 
    wick_dur = cur_wick * WICK_MOD / TIME_SCALE;
    if (oil_dur > wick_dur)
	call_out("fade", wick_dur);
    else
	call_out("fade", oil_dur);
    
    tp->catch_tell("You light the lamp.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " lights ", QNAME(this_object()),
			".\n" }), MSG_SEE, tp);
    on = 1;
    add_lwvs(LIGHT_LEVEL, 0, 0, 0); 
    return 1;
}

/*
 * Function name: extinguish_cmd
 * Description:   put the lamp out.
 * Arguments:     arg - ignored
 */
int
extinguish_cmd(string arg)
{
    object tp;

    tp = this_player();
    
    if (!on)
    {
	tp->catch_tell("The lamp is not lit.\n");
	return -1;
    }
    remove_call_out("fade");
    update_lamp();
    on = 0;
    add_lwvs(-LIGHT_LEVEL, 0, 0, 0); 
    tp->echo("You extinguish lamp.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " extinguishes ", QNAME(this_object()),
			".\n" }), MSG_SEE, 0);
    return 1;
}

/*
 * Function name: fade
 * Description:   called out when lamp goes out.
 */
static void
fade()
{
    object env;
    
    if (!on)
	return;
    update_lamp();
    on = 0;
    add_lwvs(-LIGHT_LEVEL, 0, 0, 0);
    
    env = E(this_object());
    if (living(env))
    {
	tell_objects(LISTEN(E(env)),
		     ({ QCTNAME(this_object()), " of ", QTNAME(env),
			    " fades.\n" }), MSG_SEE, 0);
	env->catch_tell("Your lamp fades.\n");
    }
    else
    {
	tell_objects(LISTEN(env),
		     ({ QCNAME(this_object()), " fades.\n" }),
		     MSG_SEE, 0);
    }
}
