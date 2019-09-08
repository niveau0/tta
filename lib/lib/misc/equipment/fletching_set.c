inherit "/base/object";

#include <language.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

#define MAX_CHARGES 50

static int charges;

nomask int query_charges();

static nomask void
create_object()
{
    charges = MAX_CHARGES;
    set_name("fletching set");
    add_name("set");
    set_pname("fletching sets");
    set_short("fletching set");
    set_pshort("fletching sets");
    set_long("This is a set for making arrows and crossbolts.\n");
    set_lwvs(0, 350, 500, 4);
    set_value(250);
    add_prop(OBJ_MATERIAL, M_IRON);
}

string long()
{
    string str;

    switch (charges)
    {
      case 1..3:
	  str=LANG_NUMW(charges);
	  break;
      case 4..9:
	  str="some";
	  break;
      case 10..15:
	  str="several";
	  break;
      case 16..25:
	  str="many";
	  break;
      case 26..MAX_CHARGES:
	  str="a lot of";
	  break;
      default:
	  return ::long()+
	      "It is empty and useless.\n";
    }
    return ::long()+
	     "It contains some feathers, a sharp knife, some arrowheads "+
	     "and some yarn.\n"+
	     "You think you could use it "+str+" times.\n";
}

nomask int
change_charges()
{
    if(charges<=0)
    {
	charges=0;
	return -1;
    }
    charges--;
    return 0;
}

nomask int
query_charges()
{
    return charges;
}
