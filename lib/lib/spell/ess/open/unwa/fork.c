inherit "tork";

#include <skills.h>

string
query_desc()
{
    return "Give the target a better chance to pick a lock. Power " +
	" increases the lock pick skill."; 
}

int
query_s()
{
    return SK_PICK_LOCKS;
}

int
query_level()
{
    return 3;
}
