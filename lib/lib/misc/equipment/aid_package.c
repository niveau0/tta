inherit "/base/object";

#include <macros.h>
#include <material.h>
#include <properties.h>

#define MAX_CHARGES 100

static int charges;

nomask int query_charges();

static nomask void
create_object()
{
    charges = MAX_CHARGES;
    set_name("aid package");
    add_name("package");
    set_pname("aid packages");
    set_short("aid package");
    set_pshort("aid packages");
    set_long("This is a package that helps you to stop bleeding wounds or "+
	     "even\nbandage broken limbs.\n");
    set_lwvs(0, 1150, 1000, 8);
    set_value(950);
    add_prop(OBJ_MATERIAL, M_IRON);
}

string long()
{
    string str;

    switch (charges)
    {
      case 1..6:
	  str="only very few";
	  break;
      case 7..15:
	  str="only a few";
	  break;
      case 16..30:
	  str="some";
	  break;
      case 31..45:
	  str="several";
	  break;
      case 46..65:
	  str="many";
	  break;
      case 66..MAX_CHARGES:
	  str="a lot of";
	  break;
      default:
	  return ::long()+
	      "It is empty and useless.\n";
    }
    return ::long()+
	     "You think you could use it "+str+" times.\n";
}

nomask void
change_charges(int change)
{
    charges += change;
}

nomask int
query_charges()
{
    return charges;
}
