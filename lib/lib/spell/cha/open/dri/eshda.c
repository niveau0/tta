inherit "shada";

#include <combat.h>

string
query_desc()
{
    return "Strong protection shield against 'ess' spells.";
}

mixed
query_similar_code()
{
    return "_eshda";
}

int
query_resist_realm()
{
    return CB_RESIST_ESS;
}

int
query_level()
{
    return 25;
}
