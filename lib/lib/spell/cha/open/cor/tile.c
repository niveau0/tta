inherit "hle";

string
query_desc()
{
    return "A healing spell for concussion wounds. The spell cannot heal " +
	"fractures or bloodloss but heals all concussion damage at once. " +
	"Power manipulation has no effect.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	tar[0]->catch_tell("You feel vitality flowing through your body.\n");
	tar[0]->set_hp(tar[0]->query_max_hp());
    }
}

int
query_level()
{
    return 30;
}
