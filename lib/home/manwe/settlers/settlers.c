#include <language.h>
#include <macros.h>
#include <slots.h>

static int     phase,       /* game phase we are in */
	       maxplayers,  /* maximum number of players */
	       thief,       /* thief position */
               hexnum,      /* how many hex in game */
               discardnum,  /* total of cards to discard in turn */
               freeroads,   /* roadbuild card -> 2 free roads */
               *devcards,   /* development card stack */
               *init;       /* initphase, ({ road, village }) */
static string  map,         /* the whole map */
               current,     /* current player color */
               trader,      /* the player current has chosen for trade */
               resdata,     /* resource data to send to client */     
               probdata,    /* probability data to send to client */     
               harbdata,    /* harbor data to send to client */     
               *resdesc,    /* resource description strings */
               *colors;     /* all colors */
static mapping hexpos,      /* hexnumber : position */
               players,     /* color : ({ playerobj, clientobj }) */
               playercolor, /* player : color */
	       cards,       /* color : ([ resource : amount ]) */
	       probability, /* probability : ({ hexnum, hexnum, ...}) */
	       resources,   /* hex : resource */
	       harbor,      /* pos : ({ hex, dir, resource value }) */
	       villages,    /* pos : color */
	       towns,       /* pos : color */
	       roads;       /* pos : color */
static mixed   develop;    /* ({ already used, ([ res:amount ]) }) */ 

#include "settlers.h"
#include "server.c"
#include "costs.c"
#include "build.c"
#include "cmds.c"
#include "creation.c"

void
create()
{
    colors = COLORS;
    hexpos = ([ ]);
    players = ([ ]);
    playercolor = ([ ]);
    cards = ([ ]);
    probability = ([ ]);
    resources = ([ ]);
    harbor = ([ ]);
    villages = ([ ]);
    towns = ([ ]);
    roads = ([ ]);
    develop = ({ 0, ([ ]) });
    init = ({ 0, 0 });
    build_history = "";
    resdata = "";
    probdata = "";
    harbdata = "";
    resdesc = RESDESC;
}

int
add_player(object pl)
{
    int    i, sz;
    string col, *ind;
    object *value;
    
    if (!pl)
	return 0;
    ind = map_indices(players);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	if ((value = players[ind[i]]) && value[0] == pl)
	    return 0;
	if (value[1] && value[1]->query_name() == pl->query_name())
	    break;
    }
    if (i == sz)
    {
	if (sz >= maxplayers)
	    return 0;
	players[col = colors[i]] = ({ pl, 0 });
	if (!phase && sz == maxplayers - 1)
	{
	    current = "red";
	    phase = INIT_PHASE;
	}
    }
    else
	players[col = ind[i]][0] = pl;
    playercolor[pl] = col;
    pl->define_primary("catch_cmd", this_object());
    pl->catch_tell("You are now player '" + col +
		   "' in the settlers game.\nTry 'help settlers'.\n");
    return 1;
}

int
add_client(object client, string name, string pw)
{
    int    i, sz, j, num;
    string password, col, str, *ind;
    object ob, *value;

    if (!client || !name || !pw)
	return 0;
    if (!(ob = _MASTER_OB->finger_player(name)))
	return 0;
    password = ob->query_password();
    if (crypt(pw, password) != password)
	return 0;
    ind = map_indices(players);
    for (i=0, sz=sizeof(ind); i<sz; i++)
	if ((value = players[ind[i]]) && value[0] &&
	    value[0]->query_name() == name)
	    break;
    if (i == sz)
    {
	if (sz >= maxplayers)
	    return 0;
	players[col = colors[i]] = ({ 0, client });
	if (!phase && sz == maxplayers - 1)
	{
	    current = "red";
	    phase = INIT_PHASE;
	}
    }
    else
    {
	if (ob = value[1])
	    destruct_object(ob);
	players[col = ind[i]][1] = client;
    }
    ind = map_indices(players);
    for (i=0, sz=sizeof(ind); i<sz; i++)
    {
	num = 0;
	if (cards[ind[i]])
	{
	    for (j=FOREST; j<DESERT; j++)
		num += cards[ind[i]][j];
	}
	send_string("player " + num + " " + ind[i] + "\n");
    }
    
    send_string("msg \nhex " + hexnum +
		"\nmsg You are now player '" + col +
		"' in the settlers game\n", col);
    str = "";
    if (sizeof(cards[col]) > 0)
    {
	for (i=FOREST; i<DESERT; i++)
	    str += "card " + cards[col][i] + " " + resdesc[i] + "\n";
    }
    else
    {
	for (i=FOREST; i<DESERT; i++)
	    str += "card 0 " + resdesc[i] + "\n";
    }
    send_string(str, col);
    call_out("send_string", 1, resdata, col);
    call_out("send_string", 1, probdata, col);
    call_out("send_string", 1, harbdata, col);
    call_out("send_string", 2, build_history +
	     (thief ? "thief " + thief + "\n" : ""), col);
    return 1;
}

int
catch_cmd(string cmd, string arg)
{
    int    i, sz, j, sz2, num, corner, pos, roll, quote, newid,
           *positions, *buildpos;
    string type, typeb, letter, *ind;
    object ob, tp, player, *cards;

    tp = this_player();

    switch (cmd)
    {
      case "cards":
	return cards(tp);
      case "costs":
	return costs(tp);
      case "help":
	if (arg != "settlers") return 0;
	return help(tp);
      case "legend":
	return legend(tp);
      case "map":
	return map(tp);
      case "players":
	return players(tp);
    }

    /* commands only allowed when it is tp's turn follow now */
    switch (cmd)
    {
      case "barter":
	return barter(arg, playercolor[tp]);
      case "build":
	return build(arg, playercolor[tp]);
      case "discard":
	return discard(arg, playercolor[tp]);
      case "end":
	if (arg != "turn") return 0;
	return end_turn(playercolor[tp]);
      case "play":
	return play(arg, playercolor[tp]);
      case "trade":
	return trade(arg, playercolor[tp]);
      case "roll":
	return roll(arg, playercolor[tp]);
      case "thief":
	return thief(arg, playercolor[tp]);
    }
    return 0;
}
