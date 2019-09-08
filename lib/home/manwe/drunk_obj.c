inherit "/std/object";

#include <stdproperties.h>
#include <macros.h>

#define SUBLOC "_sys_drunk_obj"

object victim;

public void
create_object()
{
    set_name("drunk_obj");
    set_long("Drunken object. I wonder how you can see it.\n");
    add_prop(OBJ_I_NO_DROP, 1);
    add_prop(OBJ_I_VALUE, 0);
    add_prop(OBJ_I_WEIGHT, 0);
}

public void
init()
{
    set_no_show();
    if (victim)
	return;
    define_primary("communicate", this_object());
    victim = this_player();
    victim->catch_msg("Suddenly you feel very drunk.\n");
    victim->add_subloc(SUBLOC, this_object());
    call_out("drunk_effect", 60);
}

static void
drunk_effect()
{
    if (!victim ||
	victim->query_intoxicated() < victim->query_prop(LIVE_I_MAX_INTOX) / 2)
    {
	call_out("remove_object", 0);
	return;
    }

    if (!victim->query_prop(LIVE_I_IS_HELPLESS))   
    {
	switch (random(5))
	{
	  case 0..1:
	    victim->command("hiccup");
	    break;
	  case 2..3:
	    victim->command("burp");
	    break;
	  case 4:
	    victim->command("puke");
	    break;
	}
    }
    call_out("drunk_effect", random(120) + 60);
}

public int
communicate(string cmd, string str)
{
    int    i, j, sz, sz2;
    string s1, s2, *buf;
    object tp;
    
    tp = this_player();
    
    if (!strlen(cmd))
	return 0;
    if (!strlen(str))
	str == "";
        
    if (tp->query_prop(LIVE_I_RESIST_ALCOHOL))
	return tp->private_action(cmd,str);
    
    if (cmd[0] == '\'')
	str = cmd[1..] + " " +str;
    else switch(cmd)
    {
      case "shout":
	write("Umpf, *hic* you can't get your voice loud enough.\n");
	return 1;
      case "think":
	write("You can't get a real thought.\n");
	return 1;
      case "dream":
	write("Oh yeah, you get a lot of day-dreams.\n");
	return 1;
      case "emote":
	write("Yes you behave very emotional now.\n");
	    return 1;
      case "say":
      case "whisper":
      case "rsay":
	break;
      default:
	return 0;
    }
    
    for (i=0, sz=strlen(str); i<sz ;i++)
	str[i] |= 32;
    
	buf = explode(str," ");
    for (i=0, sz=sizeof(buf); i<sz ;i++)
    {
	switch (lower_case(buf[i]))
	{
	  case "you":
	    buf[i]="you damned";
	    break;
	  case "love":
	    buf[i]="LOVE";
	    break;
	      case "damned":
	    buf[i]="damned damned damned";
	  case "your":
	    buf[i]="my";
	    break;
	  case "yours":
	    buf[i]="mine";
	    break;
	  case "oh":
	    buf[i]="ohhhhhhhhhh!!!";
		break;
	  default:
	    for (j=0, sz2=strlen(buf[i]); j<sz2; j++)
		if (buf[i][j] == 's')
		    buf[i] = buf[i][..j] + "h" + buf[i][j+1..];
	    while (sscanf(buf[i], "%sth%s", s1, s2))
		buf[i] = s1 + "ss" + s2;
	    while (sscanf(buf[i], "%st%s", s1, s2))
		buf[i] = s1 + "d" + s2;
	}
    }
    for (i=0; i<sz ;i++)
    {
	if (!random(10))
	{
	    if (strlen(buf[i]) > 3)
		    buf[i] = "*hic*" + buf[i][3..];
	    else
		buf[i] = "*hic*";
	    }
    }
    
    s1 = implode(buf," ");
    say( QCTNAME(tp) + " slurs: " + s1 + "\n");
    
    tp->remove_prop(OBJ_I_HIDE);
    tp->remove_prop(OBJ_S_HIDDEN);
    tp->set_show();

    if (query_prop(LIVE_I_GET_ECHO))
        write("You say: " + str + "\n");
    else    
        write("Ok.\n");
    return 1;
}

public void
leave_env(object from, object dest)
{
    if (!dest || !living(dest))
	call_out("remove_object", 0);
}

public int
remove_object()
{
    if (victim)
    {
	victim->remove_subloc(SUBLOC);
	set_this_player(victim);
	undefine_primary("communicate", this_object());
    }
    return ::remove_object();
}

public string
show_subloc(string subloc, object on, object for_obj)
{
    if (subloc == SUBLOC && for_obj != victim)
	return capitalize(victim->query_pronoun()) + " looks very drunk.\n";
    return "";
}
