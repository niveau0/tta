inherit "shada";

#include <combat.h>

string
query_desc()
{
    return "Strong protection shield against 'men' spells.";
}

mixed
query_similar_code()
{
    return "_ishda";
}

int
query_resist_realm()
{
    return CB_RESIST_MEN;
}

int
query_level()
{
    return 30;
}
