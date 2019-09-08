inherit "/base/living";

#include <macros.h>
#include <living.h>
#include <state.h>
#include <slots.h>
#include <skills.h>
#include <combat.h>
#include <stdproperties.h>

#include <postscript.h>
#define PSOUT "/open/scout.ps"


int *dx, *dy;
int ***map, **togo, togostep, togosz, *x, *y;
string *ds;
int psout;
int next_act;
object approach_obj;


/*
 * Function name: set_psout
 * Description:   set whether postscript-output be created on path finding.
 * Arguments:     new - zero for don't create any output,
 *                	anything else for yes, do.
 * Returns:       old status of psout.
 */
int
set_psout(int neu)
{
    int alt;
    alt=psout;
    psout=neu;
    return alt;
}

void
create_living()
{
    int vol;

    psout = 0;
    dx = ({  0,  1,  1,  1,  0, -1, -1, -1 });
    dy = ({ -1, -1,  0,  1,  1,  1,  0, -1 });
    ds = ({ "s", "se", "e", "ne", "n", "nw", "w", "sw"});
    approach_obj=this_object();
    
    set_race_name("braintester");
    set_name("scout");
    set_long("A cute sim, consuming lots of time finding ways.\n");
    set_gender(random(3));

    set_level(1 + random(30));
    set_base_hp(80 + random(query_level())*8);
    set_hp(query_max_hp());
    set_move_msg("rolls");
    
    add_prop(LIVE_NPC_MAX_FATIGUE, 500);

    add_lwvs(0, 60000, 55000, 200);

    vol = query_volume() / 100;

    add_tool_slot(SLT_HEAD+1,    "head",          vol * 6);
    add_tool_slot(SLT_EAR+1,     "right ear",     5);
    add_tool_slot(SLT_EAR+2,     "left ear",      5);
    add_tool_slot(SLT_EYE+1,     "right eye",     1);
    add_tool_slot(SLT_EYE+2,     "left eye",      1);
    add_tool_slot(SLT_NOSE+1,    "nose",          1);
    add_tool_slot(SLT_TONGUE+1,  "tongue",        1);
    add_tool_slot(SLT_NECK+1,    "neck",          5);
    add_tool_slot(SLT_CHEST+1,   "chest",         vol * 21);
    add_tool_slot(SLT_BACK+1,    "back",          vol * 21);
    add_tool_slot(SLT_ARM+1,     "right arm",     vol * 7);
    add_tool_slot(SLT_ARM+2,     "left arm",      vol * 7);
    add_tool_slot(SLT_HAND+1,    "right hand",    vol * 3);
    add_tool_slot(SLT_HAND+2,    "left hand",     vol * 3);
    add_tool_slot(SLT_ABDOMEN+1, "abdomen",       vol * 16);
    add_tool_slot(SLT_LEG+1,     "right leg",     vol * 16);
    add_tool_slot(SLT_LEG+2,     "left leg",      vol * 16);
    add_tool_slot(SLT_FOOT+1,    "right foot",    vol * 2);
    add_tool_slot(SLT_FOOT+2,    "left foot",     vol * 2);

    set_connections(([
		      SLT_HEAD+1:({ SLT_EYE+1, SLT_EYE+2 }),
		      SLT_LEG+1:({ SLT_FOOT+1 }),
		      SLT_LEG+2:({ SLT_FOOT+2 }),
		      SLT_ARM+1:({ SLT_HAND+1 }),
		      SLT_ARM+2:({ SLT_HAND+2 }),
		      ]));
		    
    set_skill_rank(SK_ATTACK_FALCHION, 10+random(10));
    set_skill_rank(SK_WESTRON, 3);
    set_tell_flags(MSG_SEE | MSG_HEAR);

    add_equipment("/lib/weapon/one_h_s/falchion");
    next_act=call_out("scout_act", 2);
}

nomask int *
query_attack_data(int slot)
{
    switch (slot)
    {
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	  return ({ SK_ATTACK_BARE_FIST, 114, 159, 250, 290, 315,
		    ({ CRUSH }) });
    }
    return 0;
}

nomask int *
query_hitloc_data(int slot)
{
    switch (slot)
    {
      case (SLT_HEAD + 1):
	return ({ ATT_SKIN, 0, 70, 6 });
      case (SLT_CHEST + 1):
	return ({ ATT_SKIN, 0, 70, 21 });
      case (SLT_ARM + 1):
      case (SLT_ARM + 2):
	return ({ ATT_SKIN, 0, 70, 7 });
      case (SLT_HAND + 1):
      case (SLT_HAND + 2):
	return ({ ATT_SKIN, 0, 70, 3 });
      case (SLT_ABDOMEN + 1):
	return ({ ATT_SKIN, 0, 70, 16 });
      case (SLT_LEG + 1):
      case (SLT_LEG + 2):
	return ({ ATT_SKIN, 0, 70, 16 });
    }
    return 0;
}

varargs void
catch_tell(string msg, int no_prompt)
{
    debug("irmo", ">> Scout gets: "+msg);
}

/*
 * Function name: scout_approach
 * Description:   actually approach coordinates.
 *                if something blocks, try next directions.
 * Arguments:     co - an array of at lest two elements,
 *                	coordinates in inch.
 */
static void
scout_approach()
{
    int *me, *co, d, d2, step, state;
    string dir;
    me=query_coordinates();

    if (togosz)
    {
	co=query_coordinates();
	if ( co[0]>=x[togo[togostep][2]]
	     && co[1]>=y[togo[togostep][3]]
	     && co[0]<=x[togo[togostep][2]+1]
	     && co[1]<=y[togo[togostep][3]+1] )
	    togostep++;
	
	if (togostep==togosz)
	{
	    togosz=0;
	    co=({ });
	    debug("irmo", ">> Final approach.\n");
	}
	else
	    co=({togo[togostep][0], togo[togostep][1]});
    }

    if (!togosz && approach_obj!=this_object())
	co=approach_obj->query_coordinates();
    
    if (!sizeof(co))
	return;
    
    dir="";
    step=query_step();
    
    if (MAX(ABS(d = me[1] - co[1]), ABS(d2 = me[0] - co[0])) < step)
    {
	if (togosz)
	{
	    set_coordinates(({co[0], co[1], me[2]}));

	    call_out("scout_approach", 1);
	    if (psout)
		PS_OBJECT_MARK("/open/scout.ps", this_object());
	}
	else
	{
	    direct_approach(this_object(), approach_obj);
	    approach_obj=this_object();
	    if (psout)
		PS_CLOSE("/open/scout.ps");

	    next_act=call_out("scout_act", 20+random(20));
	    debug("irmo", ">> Scout arrived.\n");
	}
	return;
    }
    
    if (d < 0 && -d >= step)
	dir = "n";
    else if (d > 0 && d >= step)
	dir = "s";
    
    if (d2 < 0 && -d2 >= step)
	dir += "e";
    else if (d2 > 0 && d2 >= step)
	dir += "w";

    if (dir=="")
    {
	debug("irmo", ">> Dunno where to go.\n");
	togosz=0;
	approach_obj=this_object();
	if (psout)
	    PS_CLOSE("/open/scout.ps");
	next_act=call_out("scout_act", 20+random(20));
	return;
    }
    
    if (command(dir) == -1)
    {
	d=member_array(dir, ds);
	if (command(ds[(d+1) % 8]) == -1)
	    if (command(ds[(d+7) % 8]) == -1)
		debug("irmo", ">> Blub?\n");
    }

    call_out("scout_approach", 1);
    if (psout)
	PS_OBJECT_MARK("/open/scout.ps", this_object());
}


/* "brains" for approaching and pathfinding */

int
sort_int(int a, int b)
{
    return a > b;
}

/*
 * Function name: scout_find_way(object *ob)
 * Description:   Find a way to ob and start approaching it.
 * Arguments:     ob - object to approach.
 * Returns:       distance to travel in inches,
 *                0 if ob can't be reached,
 *                or the number of stations (> 0) in the way has.
 */
int
scout_find_way(object ob)
{
    object room, *blockers, me;
    int sz, i, j, k, l, xsz, ysz, maximum, dist;
    int mei, mej, obi, obj;
    int *me_co, *ob_co, *range, *co, *sco;
    int **todo, t, **newtodo, nt, **swap;
    mapping edge_x, edge_y;
    string out;
    
    if (psout)
	PS_OPEN("/open/scout.ps");
    
    /* me and ob are in different rooms? */
    if ((room=environment(me=this_object()))
	!= environment(ob))
	return 0;

    me_co = query_coordinates();
    ob_co = ob->query_coordinates();
    if (MAX(ABS(me_co[1] - ob_co[1]), ABS(me_co[0] - ob_co[0]))
	< query_step())
    {
	direct_approach(this_object(), ob);
	togosz=0;
	approach_obj=this_object();
	
	debug("irmo", ">> Scout arrived immediately.\n");
	return 1;
    }

    if (psout)
	PS_ROOM_SNAPSHOT("/open/scout.ps", room);

    /* are there any blockers at all? */
    if (!(sz = sizeof(blockers = map_indices(room->query_block_map()))))
	return 1;
    
    /* init: room boundary is primary boundary for mesh array */
    edge_x = ([ (co = room->query_coordinates())[0] : 1,
	      co[0] + (sco = room->query_size())[0] : 1  ]);
    edge_y = ([ co[1] : 1, co[1] + sco[1] : 1 ]);

    maximum = sco[0] + sco[1];


    /* look for more boundaries */
    for (i=0; i<sz; i++)
    {
	edge_x[ (co = blockers[i]->query_coordinates())[0] ] = 1;
	edge_x[ co[0] +
	      (sco = blockers[i]->query_block_range())[0] ] = 1;
	edge_y[ co[1] ] = 1;
	edge_y[ co[1] + sco[1] ] = 1;
    }

    x = map_indices(edge_x);
    y = map_indices(edge_y);

    /* maybe one obj fills the room? */
    if ((xsz = (sizeof(x) - 1)) == 1 || (ysz = (sizeof(y) - 1)) == 1)
	return 0;

    sort_array(x, "sort_int", me);
    sort_array(y, "sort_int", me);
    for (mei=xsz; x[mei]>me_co[0]; mei--);
    for (mej=ysz; y[mej]>me_co[1]; mej--);
    for (obi=xsz; x[obi]>ob_co[0]; obi--);
    for (obj=ysz; y[obj]>ob_co[1]; obj--);

    /* Target in next field? */
    if (ABS(obi-mei)<2 && ABS(obj-mej)<2)
    {
	togosz=0;
	approach_obj=ob;
	remove_call_out(next_act);
	call_out("scout_approach", 3);
	return 1;
    }

    todo = allocate(xsz*ysz);
    newtodo = allocate(xsz*ysz);

    /* now fill in size parameters */
    map = allocate( xsz );
    sco = ({ 0, 0 });
    co = ({ 0, 0 });
    for (i=0; i<xsz; i++)
    {
	map[i] = allocate( ysz );
	sco[0] = (x[i+1] - (co[0] = x[i])) / 2;
	co[0] += sco[0];
	for (j=0; j<ysz; j++)
	{
	    sco[1] = (y[j+1] - (co[1] = y[j])) / 2;
	    co[1] += sco[1];
	    map[i][j] = ({ maximum, MAX( sco[0], sco[1]) });
	    for (k=0; k<sz; k++)
	    {
		ob_co = blockers[k]->query_coordinates();
		range = blockers[k]->query_block_range();

		if (ob_co[0] < co[0]
		    && ob_co[1] < co[1]
		    && ob_co[0] + range[0] > co[0]
		    && ob_co[1] + range[1] > co[1])
		{
		    map[i][j][0] = -1;
		    k=sz;
		}
	    }
	}
    }
    
    /* mark target */
    co = ({ 0, 0 });
    sco = ob->query_coordinates();
    
    map[i=obi][j=obj][0] = 0;
    nt=0;
    for (k=0; k<8; k++)
    {
	co[0]=i + dx[k];
	co[1]=j + dy[k];
	if (co[0] >= 0 && co[0] < xsz
	    && co[1] >= 0 && co[1] < ysz
	    && (map[ co[0] ][ co[1] ][0] > 0))
	{
	    map[ co[0] ][ co[1] ][0]
		= MAX(ABS((x[co[0]]+x[co[0]+1])/2-sco[0]),
		      ABS((y[co[1]]+y[co[1]+1])/2-sco[1]));
	    newtodo[nt++] = ({ co[0], co[1] });
	}
    }
    swap = todo; todo = newtodo; newtodo = swap;
    
    while ( (t = nt) )
    {
	nt = 0;
	for (l=0; l<t; l++)
	{
	    i = todo[l][0];
	    j = todo[l][1];
	    for (k=0; k<8; k++)
	    {
		co[0]=i + dx[k];
		co[1]=j + dy[k];
		if (co[0] >= 0 && co[0] < xsz
		    && co[1] >= 0 && co[1] < ysz
		    && ((dist = (map[i][j][1] + map[i][j][0]
				 + map[ co[0] ][ co[1] ][1]))
			< map[ co[0] ][ co[1] ][0]))
		{
		    map[ co[0] ][ co[1] ][0] = dist;
		    newtodo[nt++] = ({ co[0], co[1] });
		}
	    }
	}
	swap = todo; todo = newtodo; newtodo = swap;
    }

    if (psout)
	remove_file("/d/Valar/irmo/blub");
    if (psout)
	for (j=0; j<ysz; j++)
	{
	    for (i=0; i<xsz; i++)
		write_file("/d/Valar/irmo/blub", map[i][j][0]+"\t", 0);
	    write_file("/d/Valar/irmo/blub", "\n", 0);
	}

    /* now look for the way */
    i=mei; j=mej;
    sco = ({ 0, 0 });
    co = ({ 0, 0 });
    t=0;
    
    sco = ({ -1, maximum});
    for (k=0; k<8; k++)
    {
	co[0]=i + dx[k];
	co[1]=j + dy[k];
	if (co[0] >= 0 && co[0] < xsz
	    && co[1] >= 0 && co[1] < ysz
	    && map[ co[0] ][ co[1] ][0] >= 0
	    && (dist =
		(MAX(ABS((x[co[0]]+x[co[0]+1])/2-me_co[0]),
		     ABS((y[co[1]]+y[co[1]+1])/2-me_co[1])))
		+ map[ co[0] ][ co[1] ][0]) < sco[1])
	    sco = ({ k, dist });	    
    }
    map[i][j][0]=sco[1];
    i += dx[sco[0]];
    j += dy[sco[0]];

    while (map[i][j][0])
    {
	todo[t++] = ({ (x[i] + x[i+1]) / 2, (y[j] + y[j+1]) / 2, i,j});

	if (psout)
	    write_file("/d/Valar/irmo/blub",
		       map[i][j][0]+" "+
		       i+" "+j+"\n", 0);
	
	sco = ({ -1, map[i][j][0]});
	for (k=0; k<8; k++)
	{
	    co[0]=i + dx[k];
	    co[1]=j + dy[k];
	    if (co[0] >= 0 && co[0] < xsz
		&& co[1] >= 0 && co[1] < ysz
		&& map[ co[0] ][ co[1] ][0] >= 0
		&& map[ co[0] ][ co[1] ][0] < sco[1])
		sco = ({ k, map[ co[0] ][ co[1] ][0] });	    
	}
	if (sco[0] < 0)
	    return 42;

	i += dx[sco[0]];
	j += dy[sco[0]];
    }

    if (psout)
	PS_LINE("/open/scout.ps", me_co[0], me_co[1], todo[0][0], todo[0][1]);
    togo=allocate(t);
    
    togo[0]=todo[0];
    for (i=1; i<t; i++)
    {
	togo[i]=todo[i];
	if (psout)
	    PS_LINE("/open/scout.ps",
		    todo[i-1][0], todo[i-1][1],
		    todo[i][0], todo[i][1]);
    }
    
    togostep=0;
    togosz=t;
    approach_obj=ob;

    remove_call_out(next_act);
    call_out("scout_approach", 3);
    return togosz;
}

/*
 * Function name: scout_act
 * Description:   do something
 */
static void
scout_act()
{
    int *co;

    if (!living(this_object()))
	return;

    next_act=call_out("scout_act", 20+random(20));
    
    switch (random(5))
    {
      case 0:
	  command("tap");
	return;
      case 1:
	  command("shrug");
	return;
      case 2:
	  command("ponder");
	return;
      case 3:
	  command("exa irmo");
	return;
      case 4:
	  command("l");
	return;
    }
}

