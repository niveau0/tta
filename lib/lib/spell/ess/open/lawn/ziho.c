inherit "numb";

#include <skills.h>

string
query_desc()
{
    return "Creates a protection aura against essence magic around the " +
	"caster.";
}

void
do_this_effect(object t, int i)
{
    t->add_tmp_skill(SK_RESIST_ESS, i * 3);
}

int
query_level()
{
    return 8;
}
