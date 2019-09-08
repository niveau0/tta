inherit "shac";

#include <combat.h>

string
query_desc()
{
    return "Protection shield against 'men' spells.";
}

mixed
query_similar_code()
{
    return "_ihac";
}

int
query_shield_realm()
{
    return CB_RESIST_MEN;
}

int
query_level()
{
    return 13;
}
