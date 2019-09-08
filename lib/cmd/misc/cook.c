/*
 * Function name: cook
 * Description:   cook something (like potions)
 * Arguments:     str - what to cook where
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
cook(string str, object tp)
{
    int     i, liquid;
    string  container;
    object  cont, *wood, *list;
    mapping known_recipes;
    mixed   recipe;

    if (!str || sscanf(str, "%s with %s", str, container) != 2)
    {
        tp->catch_tell("Usage: cook <name of recipe> with <container>\n");
        return -1;
    }
    if (!(recipe = RECIPES[str]) ||
	!(known_recipes = tp->query_prop(LIVE_KNOWN_RECIPES)) ||
	!known_recipes[str])
    {
	tp->catch_tell("You don't know the recipe to make '" + str + "'.\n");
	return -1;
    }
    list = tp->visible(I(E(tp), 1));
    list = parse(container, list, tp, tp->query_step());
    if (!check_size(list, "No '" + container + "' in range.\n", tp))
    	return -1;
    cont = list[0];
    if (cont->query_max_volume() < (recipe[RECIPE_WATER]*11/10))
    {
	tp->catch_tell("The "+ container + " is too small to cook " + str +
		       " in it.\n");
	return -1;
    }
    if (sizeof(tp->visible(I(cont, 1)) - cont->attached()))
    {
	tp->catch_tell("The " + container + " is not empty.\n");
	return -1;
    }
    if ((liquid = cont->query_liquid_type()) &&	liquid != LIQ_WATER)
    {
	tp->catch_tell("There is another liquid in the " + container + ".\n");
	return -1;
    }
    if (!(sizeof(wood = list[0]->attached())))
    {
	tp->catch_tell("You need some firewood or similar to make fire.\n");
	return -1;
    }
    list = I(tp, 4);
    for (i=sizeof(list); i--;)
	if (list[i]->query_burning())
	    break;
    if (i < 0)
    {
	tp->catch_tell("You need tinder and stone or something already " +
		       "burning.\n");
	return -1;
    }
    return cont->cook_cmd(str, recipe, wood, tp);
}
