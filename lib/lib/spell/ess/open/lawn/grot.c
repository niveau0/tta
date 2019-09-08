inherit "numb";

#include <skills.h>

string
query_desc()
{
    return "Creates a protection aura against essence, channeling and " +
	"mentalism magic around the caster.";
}

void
do_this_effect(object t, int i)
{
    t->add_tmp_skill(SK_RESIST_ESS, i * 3);
    t->add_tmp_skill(SK_RESIST_MEN, i * 3);
    t->add_tmp_skill(SK_RESIST_CHA, i * 3);
}

int
query_level()
{
    return 20;
}
