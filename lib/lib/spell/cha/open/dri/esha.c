inherit "shac";

#include <combat.h>

string
query_desc()
{
    return "Protection shield against 'ess' spells.";
}

mixed
query_similar_code()
{
    return "_esha";
}

int
query_shield_realm()
{
    return CB_RESIST_ESS;
}

int
query_level()
{
    return 11;
}
