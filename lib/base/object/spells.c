static mapping spells;

/*
 * Function name: add_spell
 * Description:   stores all spells active on this object for later usage
 * Arguments:     ob - spellfile clone
 */
nomask void
add_spell(object ob)
{
    if (!ob)
	return;
    
    if (!mappingp(spells))
	spells = ([ ]);
    
    spells[ob] = 1;
}

/*
 * Function name: query_spells
 * Description:   gives all connected spell objects
 * Returns:       mapping
 */
nomask object *
query_spells()
{
    if (!mappingp(spells))
	return ({ });
    return map_indices(spells);
}

/*
 * Function name: remove_spell
 * Description:   remove one spell on this object 
 * Arguments:     ob - the spellfile clone
 */
nomask void
remove_spell(object ob)
{
    if (mappingp(spells))
	spells[ob] = 0;
}
