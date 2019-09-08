inherit "/system/parse";

#include <macros.h>
#include <status.h>
#include <config.h>

#undef RUSAGE
#define VARS "_exec_vars_"

static mixed execute();

static object me, here, sysd;
static mixed  a, b, c, d, e, f, g, h, i, j, vars;

static nomask void
init_vars()
{
    if (sizeof(vars = this_interactive()->query_prop(VARS)) == 10)
    {
	a = vars[0]; b = vars[1]; c = vars[2]; d = vars[3]; e = vars[4]; 
	f = vars[5]; g = vars[6]; h = vars[7]; i = vars[8]; j = vars[9]; 
    }
    else
	vars = allocate(10);
    me = this_interactive();
    here = E(me);
    sysd = OBJ(_SYSTEMD);
}

static nomask object
p(mixed m, object o)
{
    return present(m, o);
}

static nomask object
fp(string n)
{
    return find_player(n);
}

static nomask object *
u()
{
    return users();
}

nomask void
show_vars()
{
    int    i;
    string tmp, str;

    init_vars();
    tmp = "  = ";
    str = "";
    for (i=0; i<10; i++)
    {
	tmp[0] = i+97;
	str += tmp + type_to_string(vars[i], 0) + "\n";
    }
    this_interactive()->catch_tell(str);
}

nomask void
exec_code()
{
    int   cost;
    mixed res;
    
#ifdef RUSAGE
    mixed ru1, ru2;
#endif
    
    init_vars();
    cost = status()[ST_TICKS];
    
#ifdef RUSAGE
    ru1 = rusage();
#endif
    
    res = execute();
#ifdef RUSAGE
    ru2 = rusage();
#endif
    cost -= status()[ST_TICKS];
    
    this_interactive()->catch_tell("Result:\n " + type_to_string(res, 0) +
				   "\n[" + cost + "]" +	  
#ifdef RUSAGE
				   " Rusage: " + ((ru2[0]+ru2[1]) -
						  (ru1[0]+ru1[1])) +
#endif
				   
				   "\n");
    vars[0] = a; vars[1] = b; vars[2] = c; vars[3] = d; vars[4] = e; 
    vars[5] = f; vars[6] = g; vars[7] = h; vars[8] = i; vars[9] = j; 
    this_interactive()->add_prop(VARS, vars);
}