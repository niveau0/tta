#include <combat.h>

int     i, max, *a1, *a2;
mixed   ind;
mapping map;

map = target->cb_bonus_map()[CB_STUNNED];

ind = map_indices(map);
for (i=sizeof(ind), a1=allocate(i), a2=allocate(i); i--;)
{
    if (!intp(ind[i]) || ind[i] < 1)
	continue;
    a1[i] = CB_STUNNED;
    a2[i] = ind[i];
    if (ind[i] > max)
	max = ind[i];
}

target->cb_remove_boni(a1, a2);
target->cb_add_boni(({ CB_STUNNED }), ({ ([ max - relief*CB_ROUND : 1 ]) }));
