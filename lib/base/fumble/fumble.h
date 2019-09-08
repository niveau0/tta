#include <combat.h>
#include <effects.h>
#include <event.h>
#include <macros.h>
#include <magic.h>
#include <material.h>
#include <mudtime.h>
#include <skills.h>
#include <slots.h>
#include <state.h>
#include <properties.h>

/* tell macros */
#define AOBS(m, f, t, d) target->catch_msg(({ m }), f, t, d)
#define ATBS(m, f, t, d) player->catch_msg(BS(m, 77, 0), f, t, d)
#define AO(m, f, t, d)   target->catch_msg(m, f, t, d)
#define AT(m, f, t, d)   player->catch_msg(m, f, t, d)
#define AOB(i, v)        target->cb_add_boni(i, v)
#define ATB(i, v)        player->cb_add_boni(i, v)
#define CBS(m, f, t, d)  caster->catch_msg(BS(m, 77, 0), f, t, d)
#define C(m, f, t, d)    caster->catch_msg(m, f, t, d)
#define CB(i, v)         caster->cb_add_boni(i, v)
#define TW(m, o, a)      tell_watcher(m, o, a)
#define TE(m, t, d) \
tell_objects(LISTEN(E(caster))-({caster}), m, t, caster, d)

static nomask void
tell_watcher(mixed str, object ao, object att)
{
    int    i;
    object *ob;
    
    ob = LISTEN(E(ao)) - ({ ao, att });
    for (i=sizeof(ob); i--;)
	if (ob[i]->query_prop(LIVE_SEE_FIGHT))
	    ob[i]->catch_msg(str, att, MSG_SEE);
}

static nomask int *
move_weapon(object tp, int mod)
{
    int *co, *old_co, i, j, step;
    object env;

    env=tp->environment();
    step=tp->query_step();
    old_co=tp->query_coordinates();
    for (j=0;j<2;j++)
    {
	i=step*mod;
	if (!(random(2)))
	    i=i*(-1);
	co[j]=old_co[j]+i;
    }
    if (env->query_leaving_room(co) == env)
	return co;
    return old_co;
}
