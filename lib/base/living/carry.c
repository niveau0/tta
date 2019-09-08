/* weight functions for livings */

static nomask void
carry_reset()
{
    add_prop(OBJ_NO_GET, -1);/* Lifeforms can't be picked up */
}

/*
 * Function name:   query_max_weight()
 * Description:	    Calculate the maximum weight this living can budge.
 * Returns:         The maximum weight.
 */
int
query_max_weight()
{
    int w;

    return (w = query_cont_weight()) +
		w * (query_stat_bonus(STAT_ST) + 80) / 50;
}

/*
 * Function name:   query_max_volume()
 * Description:	    Calculate the maximum volume this living can budge.
 * Returns:         The maximum volume.
 */
int
query_max_volume()
{
    return query_cont_volume() * 2;
}

/*
 * Function name:   weight_encumbrance()
 * Description:     Calculate how encumbered we are in weight
 * Returns:         integer, encumbrance in percent
 */
nomask int
weight_encumbrance()
{
    return 100 - weight_left() * 100 /
	(query_max_weight() - query_cont_weight());
}

/*
 * Function name:   volume_encumbrance()
 * Description:     Calculate how encumbered we are in volume
 * Returns:         integer, encumbrance in percent
 */
nomask int
volume_encumbrance()
{
    return 100 - volume_left() * 100 /
	(query_max_volume() - query_cont_volume());
}

/*
 * Function name: add_fill
 * Description:   here: prevent livings from being filled with liquid
 *                original function in /base/container
 * Arguments:     f - liquid amount
 * Returns:       0
 */
nomask int
add_fill(int f)
{
    return 0;
}
