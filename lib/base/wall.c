inherit "object";
inherit "/system/parse";

#include <event.h>
#include <living.h>
#include <macros.h>
#include <properties.h>

static int    hp, *range;

/*
 * Function name: create_wall
 * Description:   redefinable constructor
 */
static void
create_wall()
{
    set_name("wall");
    set_short("wall");
    set_pshort("walls");
    add_reload("reload_wall", "init_wall");
}

/*
 * Function name: create_object
 * Description:   standard constructor
 */
static nomask void
create_object()
{
    range = ({ 0, 0, 0 });
    create_wall();
    add_prop(OBJ_NO_GET, 1);
    add_event(E_MOVE, "E_config_block", this_object());
}

string
long()
{
    switch ((hp & 0xffff) * 100 / (hp >> 16))
    {
      case 0..20:
	  return "It seems to break every second if damaged.\n";
      case 21..40:
	  return "There are many deep fissures, it looks unstably.\n";
      case 41..60:
	  return "It is damaged, you see some deep rifts.\n";
      case 61..80:
	  return "It look somewhat damaged.\n";
      default:
	  return "It has only a few scratches.\n";
    }
}

/*
 * Function name: set_hp
 * Description:   set the max and current hitpoints of the wall
 * Arguments:     max - maximum hits
 *                h - current hits
 */
nomask void
set_hp(int max, int h)
{
    hp = h + (max << 16);
}

nomask void
add_hp(int h)
{
    int max, cur;

    max = hp >> 16;        /* get maximum hp */
    cur = (hp & 0xffff) + h; /* get current hp and add amount */
    if (cur < 1)
    {
	remove_object();
	return;
    }
    if (cur > max) 
	hp = max + (max << 16); /* set current hp to max */
    else
	hp = cur + (max << 16);
}

nomask int
query_hp()
{
    return hp & 0xffff;
}

nomask void
set_range(int *r)
{
    int cubes;
	
    range = r;
    cubes = r[0] * r[1] * r[2];
    set_lwvs(query_light(), cubes * 20, cubes * 22, r[2]);
}

nomask int *
query_range()
{
    return range;
}

/*
 * Function name: E_config_block
 * Description:   event, here: configure the block events for this wall
 * Arguments:     ob - which object moved
 *                from - where the object comes from
 *                to - where it goes
 */
nomask void
E_config_block(object ob, object from, object to)
{
    int    *co;
    object other;
    
    if (ob != this_object())
	return;
    if (from)
	error("Moving of walls is not allowed");
    add_event(E_BLOCK, "E_block", to);
    co = query_coordinates()[..];
    co[0] += range[0];
    co[1] += range[1];
    if ((other = to->query_leaving_room(co)) && other != to)
	add_event(E_BLOCK, "E_block", other);
}

int
smash_cmd(object with, object tp)
{
    int    damage;
    object *obj;
    
    if (with)
	damage = obj[0]->query_weight() / 1000;
    tp->catch_tell("You batter against the wall.\n");
    tell_objects((obj = LISTEN(E(this_object()))) - ({ tp }),
		 ({ QCTNAME(tp), " batters against ", QTNAME(this_object()),
			".\n" }), MSG_SEE, tp);
    damage += tp->query_stat_bonus(STAT_ST);
    if (!with)
    {
	tp->catch_tell("Ouch! The wall is hard.\n");
	tp->check_death(damage / 5 + 1, this_object(), 0); 
    }
    if (query_hp() - damage < 1)
    {
	tell_objects(obj, ({ QCTNAME(this_object()), " crumbles to small " +
				 "pieces.\n" }), MSG_SEE, this_object());
    }
    add_hp(-damage);
    return 1;
}

nomask mixed *
reload_wall()
{
    return ({ range[0], range[1], range[2] });
}

nomask void
init_wall(mixed *data)
{
    range = data;
}
