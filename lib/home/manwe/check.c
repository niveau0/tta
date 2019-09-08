/* check ob alle skills vernueftig gesetzt sind */

#include <living.h>
#include <skills.h>
#include "/root/learn/learn.h"
#include "/root/learn/skill_costs.h"
#include "/root/learn/attribs.h"

void
create()
{
    mapping m;
    
    debug("manwe", map_sizeof(SK_DESC));
    debug("manwe", map_sizeof(SK_BASE_COSTS));
    debug("manwe", map_sizeof(LEARN_SKILL_GROUPS));
    debug("manwe", map_sizeof(LEARN_FACTORS));
    m = SKILL_ATTRIBUTES;
    debug("manwe", map_sizeof(m));
}
